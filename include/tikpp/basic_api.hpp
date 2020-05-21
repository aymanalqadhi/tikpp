#ifndef TIKPP_BASIC_API_HPP
#define TIKPP_BASIC_API_HPP

#include "tikpp/detail/type_traits.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <atomic>
#include <cassert>
#include <functional>
#include <memory>
#include <string>

namespace tikpp {

enum class api_version { v1, v2 };

enum class api_state {
    closed,
    connecting,
    connected,
    reading_length,
    reading_word,
    closing
};

template <typename TSocket,
          typename =
              std::enable_if_t<tikpp::detail::is_valid_socket<TSocket>::value>>
class basic_api : public std::enable_shared_from_this<basic_api<TSocket>> {

    using connect_handler =
        std::function<void(const boost::system::error_code &)>;

  public:
    void async_open(const std::string &host,
                    std::uint16_t      port,
                    connect_handler && cb) {
        if (state() == api_state::connecting) {
            cb(boost::asio::error::in_progress);
            return;
        }

        assert(state() == api_state::closed);

        boost::system::error_code ec {};
        auto address = boost::asio::ip::address::from_string(host, ec);

        if (ec) {
            cb(ec);
            return;
        }

        state(api_state::connecting);

        sock_.async_connect({address, port},
                            [self = this->shared_from_this(),
                             cb {std::move(cb)}](const auto &err) {
                                if (self->state() == api_state::closed) {
                                    return;
                                }

                                assert(self->state() == api_state::connecting);

                                if (!err) {
                                    self->state(api_state::connected);
                                    self->start();
                                } else {
                                    self->state(api_state::closed);
                                }

                                cb(err);
                            });
    }

    inline void close() {
        assert(is_open());
        sock_.close();
        state(api_state::closed);
    }

    [[nodiscard]] inline auto state() const noexcept -> api_state {
        return state_.load();
    }

    inline void state(api_state state) {
        state_.store(state);
    }

    [[nodiscard]] inline auto version() -> api_version {
        return version_;
    }

    [[nodiscard]] inline auto socket() noexcept -> TSocket & {
        return sock_;
    }

    [[nodiscard]] inline auto is_open() const noexcept -> bool {
        return state() != api_state::closed && sock_.is_open();
    }

    static inline auto create(boost::asio::io_context &io, api_version ver)
        -> std::shared_ptr<basic_api<TSocket>> {
        return std::shared_ptr<basic_api<TSocket>>(
            new basic_api<TSocket> {io, ver});
    }

  protected:
    explicit basic_api(boost::asio::io_context &io, api_version version)
        : io_ {io}, sock_ {io}, version_ {version}, state_ {api_state::closed} {
    }

    void start() {
        assert(state() == api_state::connected);
    }

  private:
    boost::asio::io_context &io_;
    TSocket                  sock_;

    const api_version      version_;
    std::atomic<api_state> state_;
};

} // namespace tikpp

#endif
