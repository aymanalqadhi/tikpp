#ifndef TIKPP_API_HPP
#define TIKPP_API_HPP

#include "tikpp/basic_api.hpp"
#include "tikpp/io_context.hpp"

#include <boost/asio/ip/tcp.hpp>

#include <memory>
#include <type_traits>

namespace tikpp {

template <typename ErrorHandle>
using api = tikpp::basic_api<boost::asio::ip::tcp::socket, ErrorHandle>;

template <typename AsyncStream = boost::asio::ip::tcp::socket,
          typename ErrorHandler>
[[nodiscard]] inline decltype(auto) make_api(tikpp::io_context &io,
                                             ErrorHandler &&    handler) {
    return tikpp::make_basic_api<AsyncStream, ErrorHandler>(
        io, std::forward<ErrorHandler>(handler));
}

template <typename AsyncStream = boost::asio::ip::tcp::socket,
          typename ErrorHandler>
[[nodiscard]] inline decltype(auto) make_api(tikpp::io_context &io,
                                             ErrorHandler &     handler) {
    using wrapper_type = std::reference_wrapper<std::decay_t<ErrorHandler>>;
    return tikpp::make_basic_api<AsyncStream, wrapper_type>(
        io, wrapper_type {handler});
}

} // namespace tikpp

#endif
