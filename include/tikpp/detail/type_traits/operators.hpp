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

HAS_OPERATOR(plus_assign, +=)
HAS_OPERATOR(minus_assign, -=)
HAS_OPERATOR(multiply_assign, *=)
HAS_OPERATOR(divide_assign, /=)
HAS_OPERATOR(mod_assign, %=)

HAS_OPERATOR(equals, ==)
HAS_OPERATOR(not_equals, !=)
HAS_OPERATOR(less_than, <)
HAS_OPERATOR(greater_than, >)
HAS_OPERATOR(less_than_or_equals, <=)
HAS_OPERATOR(greater_than_or_equals, >=)

} // namespace tikpp::detail::type_traits

#endif
