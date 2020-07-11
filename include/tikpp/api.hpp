#ifndef TIKPP_API_HPP
#define TIKPP_API_HPP

#include "tikpp/basic_api.hpp"
#include "tikpp/io_context.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <functional>
#include <memory>
#include <type_traits>

namespace tikpp {

/*!
 * \brief An alias for \see basic_api which uses boost::asio::ip::tcp::socket
 *         as the connection socket
 */
template <typename ErrorHandler>
using api = tikpp::basic_api<boost::asio::ip::tcp::socket, ErrorHandler>;

/*!
 * \brief A type-erase alias for \see basic_api which uses
 *         boost::asio::ip::tcp::socket as the connection socket
 */
using api_te = tikpp::basic_api_te<boost::asio::ip::tcp::socket>;

/*!
 * \brief Creates a new instance of \see api
 *
 * \param [in] io      The IO object to be used by the API connection
 * \param [in] handler A callable object to be called on fatal errors
 *
 * \return The create \see basic_api instance
 */
template <typename AsyncStream = boost::asio::ip::tcp::socket,
          typename ErrorHandler>
[[nodiscard]] inline decltype(auto) make_api(tikpp::io_context &io,
                                             ErrorHandler &&    handler) {
    return tikpp::make_basic_api<AsyncStream, ErrorHandler>(
        io, std::forward<ErrorHandler>(handler));
}

/*!
 * \brief Creates a new instance of \see api
 *
 * \param [in] io      The IO object to be used by the API connection
 * \param [in] handler A reference to a callable object to be called on fatal
 *                     errors
 *
 * \return The create \see basic_api instance
 */
template <typename AsyncStream = boost::asio::ip::tcp::socket,
          typename ErrorHandler>
[[nodiscard]] inline decltype(auto) make_api(tikpp::io_context &io,
                                             ErrorHandler &     handler) {
    using wrapper_type = std::reference_wrapper<std::decay_t<ErrorHandler>>;
    return tikpp::make_basic_api<AsyncStream, wrapper_type>(
        io, wrapper_type {handler});
}

/*!
 * \brief Creates a new instance of \see basic_api_te
 *
 * \param [in] io      The IO object to be used by the API connection
 * \param [in] handler A callable object to be called on fatal errors
 *
 * \return The create \see basic_api instance
 */
template <typename AsyncStream = boost::asio::ip::tcp::socket>
[[nodiscard]] inline auto
make_api_te(tikpp::io_context &                                    io,
            std::function<void(const boost::system::error_code &)> handler)
    -> std::shared_ptr<basic_api_te<AsyncStream>> {
    return tikpp::make_basic_api<AsyncStream, std::decay_t<decltype(handler)>>(
        io, std::move(handler));
}

} // namespace tikpp

#endif
