#ifndef TIKPP_BASIC_API_HPP
#define TIKPP_BASIC_API_HPP

#include "tikpp/detail/async_result.hpp"
#include "tikpp/detail/operations/async_connect.hpp"
#include "tikpp/detail/operations/async_read_response.hpp"
#include "tikpp/detail/type_traits/error_handler.hpp"
#include "tikpp/detail/type_traits/stream.hpp"

#include "tikpp/commands/login.hpp"
#include "tikpp/error_code.hpp"
#include "tikpp/io_context.hpp"
#include "tikpp/request.hpp"
#include "tikpp/response.hpp"

#include <boost/asio/ip/address.hpp>
#include <boost/asio/write.hpp>
#include <boost/system/error_code.hpp>

#include <atomic>
#include <cassert>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <string>

namespace tikpp {

enum class api_state { closed, connecting, connected };

template <typename AsyncStream,
          typename ErrorHandler,
          typename = std::enable_if_t<
              tikpp::detail::type_traits::is_async_stream_v<AsyncStream> &&
              tikpp::detail::type_traits::is_error_handler_v<ErrorHandler>>>
struct basic_api
    : std::enable_shared_from_this<basic_api<AsyncStream, ErrorHandler>> {
    using read_handler = std::function<bool(const boost::system::error_code &,
                                            tikpp::response &&)>;

    template <typename CompletionToken>
    decltype(auto) async_open(const std::string &host,
                              std::uint16_t      port,
                              CompletionToken && token) {
        GENERATE_COMPLETION_HANDLER(void(const boost::system::error_code &),
                                    token, handler, result);

        if (state_.load() == api_state::connecting) {
            io_.post([handler {std::move(handler)}]() mutable {
                handler(boost::asio::error::make_error_code(
                    boost::asio::error::in_progress));
            });
            return result.get();
        }

        assert(state_.load() == api_state::closed);
        state_.store(api_state::connecting);

        tikpp::detail::operations::async_connect(
            sock_, host, port,
            [self = this->shared_from_this(),
             handler {std::move(handler)}](const auto &err) mutable {
                assert(self->state_.load() == api_state::connecting);

                if (err) {
                    self->state_.store(api_state::closed);
                    return handler(err);
                }

                self->state_.store(api_state::connected);
                handler(err);

                self->read_next_response();
            });

        return result.get();
    }

    template <typename CompletionToken>
    decltype(auto) async_open(const std::string &host,
                              std::uint16_t      port,
                              const std::string &name,
                              const std::string &password,
                              CompletionToken && token) {
        GENERATE_COMPLETION_HANDLER(void(const boost::system::error_code &),
                                    token, handler, result);

        async_open(host, port,
                   [this, handler {std::move(handler)}, name,
                    password](const auto &err) mutable {
                       if (err) {
                           return handler(err);
                       }

                       async_login(name, password,
                                   [this, handler {std::move(handler)}](
                                       const auto &err) mutable {
                                       if (err) {
                                           if (is_open()) {
                                               close();
                                           }

                                           handler(err);
                                       } else {
                                           handler(
                                               boost::system::error_code {});
                                       }
                                   });
                   });

        return result.get();
    }

    inline void close() {
        assert(is_open());
        sock_.close();
        state_.store(api_state::closed);
        logged_in_.store(false);
    }

    [[nodiscard]] inline auto aquire_unique_tag() noexcept -> std::uint32_t {
        return current_tag_.fetch_add(1);
    }

    template <
        typename Command,
        typename... Args,
        typename = std::enable_if_t<std::is_base_of_v<tikpp::request, Command>>>
    [[nodiscard]] inline auto make_request(Args &&... args)
        -> std::shared_ptr<tikpp::request> {
        return std::make_shared<Command>(aquire_unique_tag(),
                                         std::forward<Args>(args)...);
    }

    [[nodiscard]] inline auto make_request(std::string command)
        -> std::shared_ptr<tikpp::request> {
        return std::make_shared<tikpp::request>(std::move(command),
                                                aquire_unique_tag());
    }

    template <typename CompletionToken>
    void async_send(std::shared_ptr<request> req, CompletionToken &&token) {
        assert(req != nullptr);

        GENERATE_COMPLETION_HANDLER(
            void(const boost::system::error_code &, tikpp::response &&), token,
            handler, result);

        io_.post([self = this->shared_from_this(), req {std::move(req)},
                  handler {std::move(handler)}]() mutable {
            self->send_queue_.emplace_back(
                std::make_pair(std::move(req), std::move(handler)));
            self->send_next();
        });

        return result.get();
    }

    template <typename CompletionToken>
    decltype(auto) async_login(const std::string &name,
                               const std::string &password,
                               CompletionToken && token) {
        GENERATE_COMPLETION_HANDLER(void(const boost::system::error_code &),
                                    token, handler, result);

        async_send(
            make_request<tikpp::commands::v2::login>(name, password),
            [this, name, password, handler {std::move(handler)}](
                const auto &err, auto &&resp) mutable {
                if (err) {
                    handler(err);
                } else if (resp.error()) {
                    handler(resp.error());
                } else if (resp.contains(
                               tikpp::commands::v1::login::challenge_param)) {
                    auto req = make_request<tikpp::commands::v1::login>(
                        name, password,
                        resp[tikpp::commands::v1::login::challenge_param]);
                    async_send(std::move(req),
                               [this, handler {std::move(handler)}](
                                   const auto &err, auto &&resp) mutable {
                                   if (err) {
                                       handler(err);
                                   } else if (resp.error()) {
                                       handler(resp.error());
                                   } else {
                                       logged_in_.store(true);
                                       handler(boost::system::error_code {});
                                   }

                                   return false;
                               });
                } else {
                    handler(boost::system::error_code {});
                }
                return false;
            });

        return result.get();
    }

    [[nodiscard]] inline auto socket() noexcept -> AsyncStream & {
        return sock_;
    }

    [[nodiscard]] inline auto is_open() const noexcept -> bool {
        return state_.load() == api_state::connected && sock_.is_open();
    }

    [[nodiscard]] inline auto is_logged_in() const noexcept -> bool {
        return logged_in_.load();
    }

  protected:
    explicit basic_api(tikpp::io_context &io, ErrorHandler &&handler)
        : io_ {io},
          sock_ {io},
          error_handler_ {std::move(handler)},
          state_ {api_state::closed},
          current_tag_ {0},
          logged_in_ {false} {
    }

  private:
    inline void send_next() {
        assert(!send_queue_.empty());

        auto [req, cb] = std::move(send_queue_.front());
        send_queue_.pop_front();

        if (!is_open()) {
            cb(boost::asio::error::not_connected, {});
            return;
        }

        auto buf = std::make_shared<std::vector<std::uint8_t>>();
        req->encode(*buf);

        boost::asio::async_write(
            sock_, boost::asio::buffer(*buf),
            [self = this->shared_from_this(), buf, req {std::move(req)},
             cb {std::move(cb)}](const auto &err, const auto &sent) mutable {
                if (!self->is_open()) {
                    cb(boost::asio::error::not_connected, {});
                    return;
                }

                if (err) {
                    self->close();
                    cb(err, {});
                } else {
                    self->read_cbs_.emplace(
                        std::make_pair(req->tag(), std::move(cb)));
                }

                if (!self->send_queue_.empty()) {
                    self->send_next();
                }
            });
    }

    inline void read_next_response() {
        if (!is_open()) {
            return;
        }

        tikpp::detail::operations::async_read_response(
            sock_, [self = this->shared_from_this()](const auto &err,
                                                     auto &&     resp) mutable {
                if (!self->is_open()) {
                    return;
                }

                if (err) {
                    return self->on_error(err);
                }

                self->on_response(std::move(resp));
            });
    }

    inline void on_response(tikpp::response &&resp) {
        if (auto itr = read_cbs_.find(resp.tag().value());
            itr != read_cbs_.end()) {
            if (!itr->second({}, std::move(resp))) {
                read_cbs_.erase(itr);
            }
        }

        read_next_response();
    }

    inline void on_error(const boost::system::error_code &err) {
        close();
        error_handler_(err);
    }

    tikpp ::io_context &io_;
    AsyncStream         sock_;
    ErrorHandler        error_handler_;

    std::atomic<api_state> state_;
    std::atomic_uint32_t   current_tag_;
    std::atomic_bool       logged_in_;

    std::deque<std::pair<std::shared_ptr<request>, read_handler>> send_queue_;
    std::map<std::uint32_t, read_handler>                         read_cbs_;
};

template <typename AsyncStream>
using basic_api_te =
    basic_api<AsyncStream,
              std::function<void(const boost::system::error_code &)>>;

namespace detail {

template <typename AsyncStream, typename ErrorHandler>
struct basic_api_creator : tikpp::basic_api<AsyncStream, ErrorHandler> {
    template <typename... Arg>
    basic_api_creator(Arg &&... args)
        : tikpp::basic_api<AsyncStream, ErrorHandler> {
              std::forward<Arg>(args)...} {
    }
};

} // namespace detail

template <typename AsyncStream, typename ErrorHandler>
[[nodiscard]] inline auto make_basic_api(tikpp::io_context &io,
                                         ErrorHandler &&    handler)
    -> std::shared_ptr<tikpp::basic_api<AsyncStream, ErrorHandler>> {
    return std::make_shared<
        tikpp::detail::basic_api_creator<AsyncStream, ErrorHandler>>(
        io, std::forward<ErrorHandler>(handler));
}

}; // namespace tikpp

#endif
