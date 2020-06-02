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
    case tikpp::error_code::untagged_response:
        return "Untagged response";
    case tikpp::error_code::unknown_response_type:
        return "Unknown response type";
    case tikpp::error_code::fatal_response:
        return "Fatal response";
    case tikpp::error_code::no_such_item:
        return "No such item";
    case tikpp::error_code::invalid_argument:
        return "Invalid argument";
    case tikpp::error_code::interrupted:
        return "Command execution was interrupted";
    case tikpp::error_code::script_failure:
        return "Script related failure";
    case tikpp::error_code::general_failure:
        return "General failure";
    case tikpp::error_code::api_failure:
        return "API related failure";
    case tikpp::error_code::tty_failure:
        return "TTY related failure";
    case tikpp::error_code::unknown_parameter:
        return "Unknown parameter";
    case tikpp::error_code::login_failure:
        return "Could not login";
    case tikpp::error_code::item_already_exists:
        return "Item already exists";
    case tikpp::error_code::list_end:
        return "End of list reached";
    case tikpp::error_code::unknown_error_category:
        return "Unknoww error category";

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
