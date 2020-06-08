#ifndef TIKPP_DETAIL_TYPE_TRAITS_OPERATORS_HPP
#define TIKPP_DETAIL_TYPE_TRAITS_OPERATORS_HPP

#include "tikpp/detail/type_traits/macros.hpp"

namespace tikpp::detail::type_traits {

HAS_OPERATOR(assign, =)
HAS_OPERATOR(plus, +)
HAS_OPERATOR(minus, -)
HAS_OPERATOR(multiply, *)
HAS_OPERATOR(divide, /)
HAS_OPERATOR(mod, %)

} // namespace tikpp::detail::type_traits

#endif
