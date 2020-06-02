#include "tikpp/error_code.hpp"
#include "tikpp/response.hpp"

#include <boost/system/error_code.hpp>

#include <cassert>
#include <regex>
#include <string>

namespace {

constexpr auto normal_type_word = "!done";
constexpr auto data_type_word   = "!re";
constexpr auto trap_type_word   = "!trap";
constexpr auto fatal_type_word  = "!fatal";

constexpr auto tag_key      = ".tag";
constexpr auto category_key = "category";
constexpr auto message_key  = "message";

constexpr auto login_failure_message     = "cannot log in";
constexpr auto already_existing_message  = "already have";
constexpr auto unknown_parameter_message = "unknown parameter";

const std::regex param_pattern {R"(=([^=]+)=(.*))", std::regex::extended};
const std::regex attribute_pattern {R"((\.[^=]+)=(.*))", std::regex::extended};

void set_error_code(tikpp::response &resp, boost::system::error_code &error) {
    assert(resp.type() == tikpp::response_type::fatal ||
           resp.type() == tikpp::response_type::trap);

    auto set_error = [&error](tikpp::error_code err) {
        error = tikpp::make_error_code(err);
    };

    if (resp.type() == tikpp::response_type::fatal) {
        return set_error(tikpp::error_code::fatal_response);
    }

    if (resp.contains(::category_key)) {
        switch (resp.get<int>(::category_key)) {
        case 0:
            return set_error(tikpp::error_code::no_such_item);
        case 1:
            return set_error(tikpp::error_code::invalid_argument);
        case 2:
            return set_error(tikpp::error_code::interrupted);
        case 3:
            return set_error(tikpp::error_code::script_failure);
        case 4:
            return set_error(tikpp::error_code::general_failure);
        case 5:
            return set_error(tikpp::error_code::api_failure);
        case 6:
            return set_error(tikpp::error_code::tty_failure);
        case 7:
            return set_error(tikpp::error_code::return_value);
        default:
            return set_error(tikpp::error_code::unknown_error_category);
        }
    }

    if (resp.contains(::message_key)) {
        const auto &message = resp[message_key];

        if (message.find(::login_failure_message) != std::string::npos) {
            return set_error(tikpp::error_code::login_failure);
        } else if (message.find(::already_existing_message) !=
                   std::string::npos) {
            return set_error(tikpp::error_code::item_already_exists);
        } else if (message.find(::unknown_parameter_message) !=
                   std::string::npos) {
            return set_error(tikpp::error_code::unknown_parameter);
        }
    }

    set_error(tikpp::error_code::unknown_error);
}

} // namespace

namespace tikpp {

response::response(const std::vector<std::string> &words) {
    assert(is_valid_response(words));

    std::smatch matches {};

    for (const auto &word : words) {
        if (!std::regex_match(word, matches, ::param_pattern) &&
            !std::regex_match(word, matches, ::attribute_pattern)) {
            continue;
        }

        assert(matches.size() == 3);
        words_.emplace(std::make_pair(matches[1].str(), matches[2].str()));
    }

    if (contains(::tag_key)) {
        tag_.emplace(get<std::uint32_t>(::tag_key));
        words_.erase(::tag_key);
    }

    const auto &type_word = words[0];

    if (type_word == ::normal_type_word) {
        type_ = response_type::normal;
    } else if (type_word == ::data_type_word) {
        type_ = response_type::data;
    } else if (type_word == ::trap_type_word) {
        type_ = response_type::trap;
        ::set_error_code(*this, error_);
    } else if (type_word == ::fatal_type_word) {
        type_ = response_type::fatal;
        ::set_error_code(*this, error_);
    } else {
        type_ = response_type::unknown;
        error_ =
            tikpp::make_error_code(tikpp::error_code::unknown_response_type);
    }
}

} // namespace tikpp
