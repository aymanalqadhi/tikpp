#ifndef TIKPP_DETAIL_OPERATIONS_ASYNC_READ_RESPONSE_HPP
#define TIKPP_DETAIL_OPERATIONS_ASYNC_READ_RESPONSE_HPP

#include "tikpp/detail/operations/async_read_word.hpp"

#include "tikpp/error_code.hpp"
#include "tikpp/response.hpp"

#include <boost/asio/async_result.hpp>
#include <boost/system/error_code.hpp>

#include <string>
#include <type_traits>
#include <vector>

namespace tikpp::detail::operations {

template <typename AsyncReadStream, typename Handler>
struct async_read_response_op final {
    inline async_read_response_op(AsyncReadStream &sock, Handler &&handler)
        : sock_ {sock}, handler_ {std::forward<Handler>(handler)} {
    }

    void operator()(const boost::system::error_code &err, std::string &&word) {
        if (err) {
            return handler_(err, tikpp::response {});
        }

        if (word.empty()) {
            if (!tikpp::response::is_valid_response(words_)) {
                return handler_(
                    tikpp::make_error_code(tikpp::error_code::invalid_response),
                    tikpp::response {});
            }

            tikpp::response resp {words_};

            if (resp.type() == tikpp::response_type::fatal) {
                handler_(
                    tikpp::make_error_code(tikpp::error_code::fatal_response),
                    tikpp::response {});
            } else if (!resp.tag().has_value()) {
                handler_(tikpp::make_error_code(
                             tikpp::error_code::untagged_response),
                         tikpp::response {});
            } else {
                handler_(boost::system::error_code {}, std::move(resp));
            }

            return;
        }

        words_.emplace_back(std::move(word));
        async_read_word(sock_, std::move(*this));
    }

    inline void initiate() {
        async_read_word(sock_, std::move(*this));
    }

  private:
    AsyncReadStream &        sock_;
    Handler                  handler_;
    std::vector<std::string> words_;
};

template <typename AsyncReadStream, typename Token>
decltype(auto) async_read_response(AsyncReadStream &sock, Token &&token) {
    using signature_type =
        void(const boost::system::error_code &, tikpp::response &&);
    using result_type =
        boost::asio::async_result<std::decay_t<Token>, signature_type>;
    using handler_type = typename result_type::completion_handler_type;

    handler_type handler {std::forward<Token>(token)};
    result_type  result {handler};

    async_read_response_op<AsyncReadStream, handler_type> {sock,
                                                           std::move(handler)}
        .initiate();

    return result.get();
}

} // namespace tikpp::detail::operations

#endif
