#ifndef TIKPP_DETAIL_TYPE_TRAITS_MODEL_HPP
#define TIKPP_DETAIL_TYPE_TRAITS_MODEL_HPP

#include "tikpp/detail/type_traits/macros.hpp"
#include "tikpp/response.hpp"

#include <map>
#include <string>

namespace tikpp::detail::type_traits {

HAS_MEMBER_FUNCTION_FULL(
    is_hash_map, operator[], std::string &,(std::declval<const std::string &>()));

HAS_MEMBER_FUNCTION_FULL(has_create,
                         create,
                         T,
                         (std::declval<std::map<std::string, std::string>>()));

HAS_MEMBER_FUNCTION(dissolve,
                    (std::declval<std::map<std::string, std::string>>()));

} // namespace tikpp::detail::type_traits

#endif
