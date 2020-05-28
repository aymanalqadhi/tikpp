#ifndef TIKPP_DETAIL_OPERATIONS_ASYNC_CONNECT_HPP
#define TIKPP_DETAIL_OPERATIONS_ASYNC_CONNECT_HPP

#include "tikpp/detail/async_result.hpp"

#include <boost/asio/ip/address.hpp>
#include <boost/system/error_code.hpp>

#include <string>
#include <type_traits>

namespace tikpp::detail::operations {

template <typename AsyncReadStream, typename CompletionToken>
decltype(auto) async_connect(AsyncReadStream &  sock,
                             const std::string &host,
                             std::uint16_t      port,
                             CompletionToken && token) {
    GENERATE_COMPLETION_HANDLER(void(const boost::system::error_code &), token,
                                handler, result);

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
