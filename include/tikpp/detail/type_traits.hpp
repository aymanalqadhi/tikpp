#ifndef TIKPP_DETAIL_TYPE_TRAITS_HPP
#define TIKPP_DETAIL_TYPE_TRAITS_HPP

#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <functional>
#include <type_traits>

namespace tikpp::detail {

using boost::system::error_code;

using connect_handler = std::function<void(const error_code &)>;
using write_handler   = std::function<void(const error_code &, std::size_t)>;
using read_handler    = std::function<void(const error_code &, std::size_t)>;

template <typename T, typename = void>
struct accepts_io_context : std::false_type {};

template <typename T>
struct accepts_io_context<
    T,
    std::enable_if_t<std::is_constructible_v<T, boost::asio::io_context &>>>
    : std::true_type {};

template <typename T, typename = void>
struct has_async_connect_method : std::false_type {};

template <typename T>
struct has_async_connect_method<
    T,
    std::void_t<decltype(std::declval<T>().async_connect(
        std::declval<boost::asio::ip::tcp::endpoint>(),
        std::declval<connect_handler>()))>> : std::true_type {};

template <typename T, typename = void>
struct has_close_method : std::false_type {};

template <typename T>
struct has_close_method<T, std::void_t<decltype(std::declval<T>().close())>>
    : std::true_type {};

template <typename T, typename = void>
struct has_async_read_some_method : std::false_type {};

template <typename T>
struct has_async_read_some_method<
    T,
    std::void_t<decltype(std::declval<T>().async_read_some(
        std::declval<boost::asio::mutable_buffer>(),
        std::declval<read_handler>()))>> : std::true_type {};

template <typename T, typename = void>
struct has_async_write_some_method : std::false_type {};

template <typename T>
struct has_async_write_some_method<
    T,
    std::void_t<decltype(std::declval<T>().async_write_some(
        std::declval<boost::asio::const_buffer>(),
        std::declval<write_handler>()))>> : std::true_type {};

template <typename T, typename = void>
struct is_valid_socket : std::false_type {};

template <typename T>
struct is_valid_socket<
    T,
    std::enable_if_t<
        accepts_io_context<T>::value && has_async_connect_method<T>::value &&
        has_close_method<T>::value && has_async_read_some_method<T>::value &&
        has_async_write_some_method<T>::value>> : std::true_type {};

static_assert(accepts_io_context<boost::asio::ip::tcp::socket>::value);
static_assert(has_async_connect_method<boost::asio::ip::tcp::socket>::value);
static_assert(has_close_method<boost::asio::ip::tcp::socket>::value);
static_assert(has_async_read_some_method<boost::asio::ip::tcp::socket>::value);
static_assert(has_async_write_some_method<boost::asio::ip::tcp::socket>::value);
static_assert(is_valid_socket<boost::asio::ip::tcp::socket>::value);

} // namespace tikpp::detail

#endif
