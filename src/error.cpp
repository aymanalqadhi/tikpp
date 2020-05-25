#include "tikpp/error_code.hpp"

#include <boost/system/error_code.hpp>

#include <string>

namespace {

tikpp::detail::error_category category {};

}

namespace tikpp {

namespace detail {

auto error_category::message(int c) const -> std::string {
    switch (static_cast<tikpp::error_code>(c)) {
    case tikpp::error_code::success:
        return "Success";
    case tikpp::error_code::invalid_response:
        return "Invalid response";
    case tikpp::error_code::fatal_response:
        return "Fatal response";
    case tikpp::error_code::untagged_response:
        return "Untagged response";
    case tikpp::error_code::invalid_response_tag:
        return "Invalid response tag";
    case tikpp::error_code::invalid_login_credentials:
        return "Invalid login credentails";
    default:
        return "Unknoww error";
    }
}

} // namespace detail

auto detail::error_category::default_error_condition(int c) const noexcept
    -> boost::system::error_condition {
    switch (static_cast<tikpp::error_code>(c)) {
    case tikpp::error_code::success:
        return make_error_condition(boost::system::errc::success);
    case tikpp::error_code::invalid_response:
    case tikpp::error_code::fatal_response:
        return make_error_condition(boost::system::errc::invalid_argument);
    default:
        return boost::system::error_condition(c, *this);
    }
}

auto error_category() -> const boost::system::error_category & {
    return category;
}

auto make_error_code(tikpp::error_code e) -> boost::system::error_code {
    return {static_cast<int>(e), ::category};
}

auto make_error_condition(tikpp::error_code e) -> std::error_condition {
    return std::error_condition(static_cast<int>(e), ::category);
}

} // namespace tikpp
