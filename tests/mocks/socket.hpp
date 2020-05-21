#ifndef TIKPP_TESTS_MOCKS_SOCKET_HPP
#define TIKPP_TESTS_MOCKS_SOCKET_HPP

#include "tikpp/detail/type_traits.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/system_error.hpp>

#include <cassert>
#include <sstream>
#include <stdexcept>

namespace tikpp::tests::mocks {

struct socket final {

    socket(boost::asio::io_context & /* io */)
        : connected_ {false}, fails_ {false} {
    }

    inline void async_connect(const boost::asio::ip::tcp::endpoint &ep,
                              tikpp::detail::connect_handler &&     cb) {
        assert(!connected_);

        if (fails_) {
            cb(boost::asio::error::fault);
            return;
        }

        connected_ = true;
        ep_        = ep;

        cb({});
    }

    inline void close() {
        assert(connected_);

        if (fails_) {
            throw std::runtime_error {"Cannot close socket"};
        }

        connected_ = false;
    }

    inline void async_read_some(boost::asio::mutable_buffer   buf,
                                tikpp::detail::read_handler &&cb) {
        assert(connected_);

        if (fails_) {
            cb(boost::asio::error::fault, 0);
            return;
        }

        auto nread =
            ibuf_.readsome(static_cast<char *>(buf.data()), buf.size());

        if (nread < 0) {
            cb(boost::asio::error::eof, 0);
        } else {
            cb({}, nread);
        }
    }

    inline void async_write_some(boost::asio::const_buffer      buf,
                                 tikpp::detail::write_handler &&cb) {
        assert(connected_);

        if (fails_) {
            cb(boost::asio::error::fault, 0);
            return;
        }

        obuf_.write(static_cast<const char *>(buf.data()), buf.size());
        cb({}, buf.size());
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
    bool                           connected_, fails_;
    std::stringstream              ibuf_, obuf_;
    boost::asio::ip::tcp::endpoint ep_;
};

static_assert(tikpp::detail::is_valid_socket<socket>::value);

} // namespace tikpp::tests::mocks

#endif
