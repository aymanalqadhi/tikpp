#ifndef TIKPP_DETAIL_TYPE_TRAITS_MODEL_HPP
#define TIKPP_DETAIL_TYPE_TRAITS_MODEL_HPP

#include "tikpp/detail/type_traits/macros.hpp"

#include <map>
#include <string>

namespace tikpp::detail::type_traits {

HAS_MEMBER_FUNCTION_FULL(
    is_hash_map, operator[], std::string &,(std::declval<const std::string &>()))

template <template <typename> typename Wrapper, typename T, typename = void>
struct is_value_wrapper : std::false_type {};

template <template <typename> typename Wrapper, typename T>
struct is_value_wrapper<
    Wrapper,
    T,
    std::enable_if_t<std::is_same_v<
        std::decay_t<decltype(std::declval<Wrapper<T>>().value())>,
        T>>> : std::true_type {};

template <template <typename> typename Wrapper, typename T>
constexpr auto is_value_wrapper_v = is_value_wrapper<Wrapper, T>::value;

} // namespace tikpp::detail::type_traits

#endif
