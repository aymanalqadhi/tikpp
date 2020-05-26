#ifndef TIKPP_BASIC_API_HPP
#define TIKPP_BASIC_API_HPP

#include "tikpp/detail/operations/async_connect.hpp"
#include "tikpp/detail/operations/async_read_response.hpp"

#include "tikpp/error_code.hpp"
#include "tikpp/request.hpp"
#include "tikpp/response.hpp"

#include <boost/asio/io_context.hpp>
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

enum class api_version { v1, v2 };

enum class api_state { closed, connecting, connected, reading };

template <typename AsyncStream, typename ErrorHandler>
class basic_api : public std::enable_shared_from_this<
                      basic_api<AsyncStream, ErrorHandler>> {

    using connect_handler =
        std::function<void(const boost::system::error_code &)>;
    using read_handler = std::function<bool(const boost::system::error_code &,
                                            tikpp::response &&)>;

  public:
    void async_open(const std::string &host,
                    std::uint16_t      port,
                    connect_handler && handler) {
        if (state_.load() == api_state::connecting) {
            return handler(boost::asio::error::in_progress);
        }

        assert(state_.load() == api_state::closed);
        state_.store(api_state::connecting);

        tikpp::detail::operations::async_connect(
            sock_, host, port,
            [self = this->shared_from_this(),
             handler {std::move(handler)}](const auto &err) {
                assert(self->state_.load() == api_state::connecting);
                self->state_.store(err ? api_state::closed
                                       : api_state::connected);
                handler(err);
            });
    }

    inline void start() {
        assert(is_open());
        assert(state_.load() != api_state::reading);

        state_.store(api_state::reading);
        read_next_response();
    }

    inline void close() {
        assert(is_open());
        sock_.close();
        state_.store(api_state::closed);
    }

    [[nodiscard]] inline auto make_request(std::string command)
        -> std::shared_ptr<tikpp::request> {
        return std::make_shared<tikpp::request>(std::move(command),
                                                current_tag_.fetch_add(1));
    }

    void send(std::shared_ptr<request> req, read_handler &&cb) {
        assert(req != nullptr);

        io_.post([self = this->shared_from_this(), req {std::move(req)},
                  cb {std::move(cb)}]() mutable {
            self->send_queue_.emplace_back(
                std::make_pair(std::move(req), std::move(cb)));
            self->send_next();
        });
    }

    [[nodiscard]] inline auto version() -> api_version {
        return version_;
    }

    [[nodiscard]] inline auto socket() noexcept -> AsyncStream & {
        return sock_;
    }

    [[nodiscard]] inline auto is_open() const noexcept -> bool {
        return state_.load() != api_state::closed && sock_.is_open();
    }

    static inline auto create(boost::asio::io_context &io,
                              api_version              ver,
                              ErrorHandler &           error_handler)
        -> std::shared_ptr<basic_api<AsyncStream, ErrorHandler>> {
        return std::shared_ptr<basic_api<AsyncStream, ErrorHandler>>(
            new basic_api<AsyncStream, ErrorHandler> {io, ver, error_handler});
    }

  protected:
    explicit basic_api(boost::asio::io_context &io,
                       api_version              version,
                       ErrorHandler &           error_handler)
        : io_ {io},
          sock_ {io},
          version_ {version},
          error_handler_ {error_handler},
          state_ {api_state::closed},
          current_tag_ {0} {
    }

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
        if (!is_open() || state_.load() != api_state::reading) {
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
        if (read_cbs_.find(resp.tag().value()) == read_cbs_.end()) {
            return on_error(tikpp::error_code::invalid_response_tag);
        }

        if (!read_cbs_[resp.tag().value()]({}, std::move(resp))) {
            read_cbs_.erase(resp.tag().value());
        }

        read_next_response();
    }

    inline void on_error(const boost::system::error_code &err) {
        close();
        error_handler_.on_error(err);
    }

  private:
    boost::asio::io_context &io_;
    AsyncStream              sock_;

    const api_version      version_;
    std::atomic<api_state> state_;

    std::atomic_uint32_t                                          current_tag_;
    std::deque<std::pair<std::shared_ptr<request>, read_handler>> send_queue_;
    std::map<std::uint32_t, read_handler>                         read_cbs_;

    ErrorHandler &error_handler_;
};

} // namespace tikpp

#endif
