#ifndef TIKPP_DETAIL_TYPE_TRAITS_ERROR_HANDLER_HPP
#define TIKPP_DETAIL_TYPE_TRAITS_ERROR_HANDLER_HPP

#include "tikpp/detail/type_traits/macros.hpp"

#include <boost/system/error_code.hpp>

#include <type_traits>

namespace tikpp::detail::type_traits {

template <typename T, typename = void>
struct is_error_handler : std::false_type {};

template <typename T>
struct is_error_handler<
    T,
    std::void_t<decltype(
        std::declval<T>()(std::declval<const boost::system::error_code &>()))>>
    : std::true_type {};

template <typename T>
constexpr auto is_error_handler_v = is_error_handler<T>::value;

} // namespace tikpp::detail::type_traits

#endif
