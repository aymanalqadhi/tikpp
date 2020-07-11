#ifndef TIKPP_DETAIL_SSL_SOCKET_HPP
#define TIKPP_DETAIL_SSL_SOCKET_HPP

#include "tikpp/detail/async_result.hpp"
#include "tikpp/io_context.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>

namespace tikpp::detail {

template <typename AsyncStream, bool ssl_verify = false>
struct ssl_wrapper final {
    using executor_type = boost::asio::io_context::executor_type;

    ssl_wrapper(tikpp::io_context &io)
        : ctx_ {boost::asio::ssl::context::sslv23_client}, stream_ {io, ctx_} {

        using ctx = std::decay_t<decltype(ctx_)>;

        if constexpr (ssl_verify) {
            ctx_.set_verify_mode(boost::asio::ssl::context::verify_peer);
            ctx_.set_default_verify_paths();

        } else {
            ctx_.set_verify_mode(boost::asio::ssl::context::verify_none);
        }
    }

    inline auto get_executor() const noexcept -> executor_type {
        return stream_.get_executor();
    }

    template <typename CompletionToken>
    inline decltype(auto)
    async_connect(const boost::asio::ip::tcp::endpoint &ep,
                  CompletionToken &&                    token) {
        GENERATE_COMPLETION_HANDLER(
            void(const boost::system::error_code &, std::size_t), token,
            handler, result);

        return stream_.lowest_layer().async_connect(
            ep, [this, handler {std::move(handler)}](const auto &err) mutable {
                if (err) {
                    handler(err);
                } else {
                    stream_.async_handshake(decltype(stream_)::client,
                                            std::move(handler));
                }
            });
    }

    inline void close() {
        stream_.lowest_layer().close();
    }

    inline bool is_open() const noexcept {
        return stream_.lowest_layer().is_open();
    }

    template <typename CompletionToken>
    inline void async_read_some(boost::asio::mutable_buffer buf,
                                CompletionToken &&          token) {
        return stream_.async_read_some(buf,
                                       std::forward<CompletionToken>(token));
    }

    template <typename CompletionToken>
    inline void async_write_some(boost::asio::const_buffer buf,
                                 CompletionToken &&        token) {
        return stream_.async_write_some(std::move(buf),
                                        std::forward<CompletionToken>(token));
    }

  private:
    boost::asio::ssl::context             ctx_;
    boost::asio::ssl::stream<AsyncStream> stream_;
};

} // namespace tikpp::detail

#endif
