#ifndef TIKPP_TESTS_FAKES_SOCKET_HPP
#define TIKPP_TESTS_FAKES_SOCKET_HPP

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/system_error.hpp>

#include <cassert>
#include <sstream>
#include <stdexcept>

namespace tikpp::tests::fakes {

struct socket final {
    using executor_type = boost::asio::io_context::executor_type;

    socket(boost::asio::io_context &io)
        : io_ {io}, connected_ {false}, fails_ {false} {
    }

    inline auto get_executor() -> executor_type {
        return io_.get_executor();
    }

    template <typename Handler>
    inline void async_connect(const boost::asio::ip::tcp::endpoint &ep,
                              Handler &&                            handler) {
        assert(!connected_);

        if (fails_) {
            handler(boost::asio::error::fault);
            return;
        }

        connected_ = true;
        ep_        = ep;

        handler(boost::system::error_code {});
    }

    inline void close() {
        assert(connected_);

        if (fails_) {
            throw std::runtime_error {"Cannot close socket"};
        }

        connected_ = false;
    }

    template <typename Handler>
    inline void async_read_some(boost::asio::mutable_buffer buf,
                                Handler &&                  handler) {
        assert(connected_);

        if (fails_) {
            handler(boost::asio::error::fault, 0);
            return;
        }

        auto nread =
            ibuf_.readsome(static_cast<char *>(buf.data()), buf.size());

        if (nread < 0) {
            handler(boost::asio::error::eof, 0);
        } else {
            handler(boost::system::error_code {}, nread);
        }
    }

    template <typename Handler>
    inline void async_write_some(boost::asio::const_buffer buf,
                                 Handler &&                handler) {
        assert(connected_);

        if (fails_) {
            handler(boost::asio::error::fault, 0);
            return;
        }

        obuf_.write(static_cast<const char *>(buf.data()), buf.size());
        handler(boost::system::error_code {}, buf.size());
    }

    [[nodiscard]] inline auto is_open() const noexcept -> bool {
        return connected_;
    }

    [[nodiscard]] inline auto input_buffer() noexcept -> std::stringstream & {
        return ibuf_;
    }

    [[nodiscard]] inline auto output_buffer() noexcept -> std::stringstream & {
        return obuf_;
    }

    [[nodiscard]] inline auto remote_endpoint() noexcept
        -> boost::asio::ip::tcp::endpoint & {
        return ep_;
    }

    inline void always_fail(bool val) noexcept {
        fails_ = val;
    }

  private:
    boost::asio::io_context &      io_;
    bool                           connected_, fails_;
    std::stringstream              ibuf_, obuf_;
    boost::asio::ip::tcp::endpoint ep_;
};

} // namespace tikpp::tests::fakes

#endif
