#ifndef TIKPP_DETAIL_OPERATIONS_ASYNC_READ_WORD_HPP
#define TIKPP_DETAIL_OPERATIONS_ASYNC_READ_WORD_HPP

#include "tikpp/detail/operations/async_read_word_length.hpp"

#include <boost/asio/async_result.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/system/error_code.hpp>

#include <cstdint>
#include <string>
#include <type_traits>

namespace tikpp::detail::operations {

template <typename AsyncReadStream, typename Handler>
struct async_read_word_op final {
    inline async_read_word_op(AsyncReadStream &sock, Handler &&handler)
        : sock_ {sock}, handler_ {std::forward<Handler>(handler)} {
    }

    void operator()(const boost::system::error_code &err, std::uint32_t len) {
        if (err) {
            return handler_(err, std::string {});
        }

        auto buf = std::make_shared<std::string>();
        buf->resize(len);

        auto cb = [buf, handler {std::move(handler_)}](const auto &err,
                                                       auto        rx) mutable {
            if (err) {
                return handler(err, std::string {});
            }

            handler(boost::system::error_code {}, std::move(*buf));
        };

        boost::asio::async_read(sock_, boost::asio::buffer(*buf, len),
                                std::move(cb));
    }

    inline void initiate() {
        async_read_word_length(sock_, std::move(*this));
    }

  private:
    AsyncReadStream &sock_;
    Handler          handler_;
}; // namespace tikpp::detail::operations

template <typename AsyncReadStream, typename Token>
decltype(auto) async_read_word(AsyncReadStream &sock, Token &&token) {
    using signature_type =
        void(const boost::system::error_code &, std::string &&);
    using result_type =
        boost::asio::async_result<std::decay_t<Token>, signature_type>;
    using handler_type = typename result_type::completion_handler_type;

    handler_type handler {std::forward<Token>(token)};
    result_type  result {handler};

    async_read_word_op<AsyncReadStream, handler_type> {sock, std::move(handler)}
        .initiate();

    return result.get();
}

} // namespace tikpp::detail::operations

#endif
