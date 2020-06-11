#ifndef TIKPP_TESTS_FAKES_SOCKET_HPP
#define TIKPP_TESTS_FAKES_SOCKET_HPP

#include "tikpp/io_context.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/process/async_pipe.hpp>
#include <boost/system/system_error.hpp>

#include <cassert>
#include <stdexcept>

namespace tikpp::tests::fakes {

struct socket final {
    using executor_type = boost::asio::io_context::executor_type;

    inline auto get_executor() -> executor_type {
        return io_.get_executor();
    }

    socket(boost::asio::io_context &io)
        : io_ {io},
          input_pipe_ {io_},
          output_pipe_ {io_},
          connected_ {false},
          fails_ {false} {
    }

    template <typename CompletionHandler>
    inline void async_connect(const boost::asio::ip::tcp::endpoint &ep,
                              CompletionHandler &&                  handler) {
        assert(!connected_.load());

        if (fails_.load()) {
            handler(boost::asio::error::fault);
            return;
        }

        connected_.store(true);
        ep_ = ep;

        handler(boost::system::error_code {});
    }

    inline void close() {
        assert(connected_);

        if (fails_.load()) {
            throw std::logic_error {"Cannot close socket"};
        }

        connected_.store(false);
    }

    template <typename CompletionHandler>
    inline void async_read_some(boost::asio::mutable_buffer buf,
                                CompletionHandler &&        handler) {
        assert(connected_);

        if (fails_) {
            handler(boost::asio::error::fault, 0);
            return;
        }

        input_pipe_.async_read_some(buf, std::move(handler));
    }

    template <typename Handler>
    inline void async_write_some(boost::asio::const_buffer buf,
                                 Handler &&                handler) {
        assert(connected_);

        if (fails_) {
            handler(boost::asio::error::fault, 0);
            return;
        }

        output_pipe_.async_write_some(buf, std::move(handler));
    }

    [[nodiscard]] inline auto is_open() const noexcept -> bool {
        return connected_;
    }

    [[nodiscard]] inline auto input_pipe() noexcept
        -> boost::process::async_pipe & {
        return input_pipe_;
    }

    [[nodiscard]] inline auto output_pipe() noexcept
        -> boost::process::async_pipe & {
        return output_pipe_;
    }

    [[nodiscard]] inline auto remote_endpoint() noexcept
        -> boost::asio::ip::tcp::endpoint & {
        return ep_;
    }

  private:
    tikpp::io_context &        io_;
    boost::process::async_pipe input_pipe_;
    boost::process::async_pipe output_pipe_;

    boost::asio::ip::tcp::endpoint ep_;
    std::atomic_bool               connected_, fails_;
};

} // namespace tikpp::tests::fakes

#endif
