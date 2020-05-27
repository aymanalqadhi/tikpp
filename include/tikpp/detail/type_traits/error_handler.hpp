#ifndef TIKPP_DETAIL_TYPE_TRAITS_ERROR_HANDLER_HPP
#define TIKPP_DETAIL_TYPE_TRAITS_ERROR_HANDLER_HPP

#include "tikpp/detail/type_traits/macros.hpp"

#include <boost/system/error_code.hpp>

namespace tikpp::detail::type_traits {

HAS_MEMBER_FUNCTION(on_error,
                    (std::declval<const boost::system::error_code &>()));

} // namespace tikpp::detail::type_traits

#endif
