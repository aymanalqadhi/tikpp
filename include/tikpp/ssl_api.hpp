#ifndef TIKPP_SSL_API_HPP
#define TIKPP_SSL_API_HPP

#include "tikpp/api.hpp"
#include "tikpp/basic_api.hpp"
#include "tikpp/detail/ssl_wrapper.hpp"
#include "tikpp/io_context.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <functional>
#include <memory>
#include <type_traits>

namespace tikpp {

/*!
 * \brief Creates a new instance of \see api with SSL/TLS support
 *
 * \param [in] io      The IO object to be used by the API connection
 * \param [in] handler A callable object to be called on fatal errors
 *
 * \return The create \see basic_api instance
 */
template <typename AsyncStream = boost::asio::ip::tcp::socket,
          typename ErrorHandler>
[[nodiscard]] inline decltype(auto) make_ssl_api(tikpp::io_context &io,
                                                 ErrorHandler &&    handler) {
    return tikpp::make_api<tikpp::detail::ssl_wrapper<AsyncStream>,
                           ErrorHandler>(io,
                                         std::forward<ErrorHandler>(handler));
}

/*!
 * \brief Creates a new instance of \see api with SSL/TLS support
 *
 * \param [in] io      The IO object to be used by the API connection
 * \param [in] handler A reference to a callable object to be called on fatal
 *                     errors
 *
 * \return The create \see basic_api instance
 */
template <typename AsyncStream = boost::asio::ip::tcp::socket,
          typename ErrorHandler>
[[nodiscard]] inline decltype(auto) make_ssl_api(tikpp::io_context &io,
                                                 ErrorHandler &     handler) {
    return tikpp::make_api<tikpp::detail::ssl_wrapper<AsyncStream>,
                           ErrorHandler>(io, handler);
}

/*!
 * \brief Creates a new instance of \see basic_api_te with SSL support
 *
 * \param [in] io      The IO object to be used by the API connection
 * \param [in] handler A callable object to be called on fatal errors
 *
 * \return The create \see basic_api instance
 */
template <typename AsyncStream = boost::asio::ip::tcp::socket>
[[nodiscard]] inline decltype(auto) make_ssl_api_te(
    tikpp::io_context &                                    io,
    std::function<void(const boost::system::error_code &)> handler) {
    return tikpp::make_api_te<tikpp::detail::ssl_wrapper<AsyncStream>>(
        io, std::move(handler));
}

} // namespace tikpp

#endif
