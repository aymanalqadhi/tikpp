#ifndef TIKPP_DETAIL_TYPE_TRAITS_STREAM_HPP
#define TIKPP_DETAIL_TYPE_TRAITS_STREAM_HPP

#include "tikpp/detail/type_traits/macros.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/system/error_code.hpp>

#include <type_traits>

namespace tikpp::detail::type_traits {

struct async_transfer_handler {
    void operator()(const boost::system::error_code & /* err */,
                    std::size_t /* tx */) {
    }
};

HAS_MEMBER_FUNCTION(async_write_some,
                    (std::declval<boost::asio::const_buffer>(),
                     std::declval<async_transfer_handler>()));

HAS_MEMBER_FUNCTION(async_read_some,
                    (std::declval<boost::asio::mutable_buffer>(),
                     std::declval<async_transfer_handler>()));

HAS_MEMBER_FUNCTION(get_executor, ());

template <typename T>
constexpr auto                  is_async_write_stream_v =
    has_async_write_some_v<T> &&has_get_executor_v<T>;

template <typename T>
constexpr auto                 is_async_read_stream_v =
    has_async_read_some_v<T> &&has_get_executor_v<T>;

template <typename T>
constexpr auto                   is_async_stream_v =
    is_async_write_stream_v<T> &&is_async_read_stream_v<T>;

} // namespace tikpp::detail::type_traits

#endif
