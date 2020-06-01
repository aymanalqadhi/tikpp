#ifndef TIKPP_DETAIL_TYPE_TRAITS_MODEL_HPP
#define TIKPP_DETAIL_TYPE_TRAITS_MODEL_HPP

#include "tikpp/detail/type_traits/macros.hpp"
#include "tikpp/response.hpp"

#include <map>
#include <string>

namespace tikpp::detail::type_traits {

HAS_MEMBER_FUNCTION_FULL(
    is_hash_map, operator[], std::string &,(std::declval<const std::string &>()))

} // namespace tikpp::detail::type_traits

#endif
