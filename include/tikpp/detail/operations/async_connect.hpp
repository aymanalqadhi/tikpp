#ifndef TIKPP_DETAIL_OPERATIONS_ASYNC_CONNECT_HPP
#define TIKPP_DETAIL_OPERATIONS_ASYNC_CONNECT_HPP

#include <boost/asio/async_result.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/system/error_code.hpp>

#include <string>
#include <type_traits>

namespace tikpp::detail::operations {

template <typename AsyncReadStream, typename Token>
decltype(auto) async_connect(AsyncReadStream &  sock,
                             const std::string &host,
                             std::uint16_t      port,
                             Token &&           token) {
    using signature_type = void(const boost::system::error_code &);
    using result_type =
        boost::asio::async_result<std::decay_t<Token>, signature_type>;
    using handler_type = typename result_type::completion_handler_type;

    handler_type handler {std::forward<Token>(token)};
    result_type  result {handler};

    boost::system::error_code ec {};
    auto address = boost::asio::ip::address::from_string(host, ec);

    if (ec) {
        return handler(ec);
    }

    sock.async_connect({address, port}, std::move(handler));
    return result.get();
}

} // namespace tikpp::detail::operations

#endif
