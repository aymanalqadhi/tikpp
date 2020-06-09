#include "tikpp/error_code.hpp"
#include "tikpp/response.hpp"

#include <boost/system/error_code.hpp>

#include <array>
#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

constexpr auto normal_type_word = "!done";
constexpr auto data_type_word   = "!re";
constexpr auto trap_type_word   = "!trap";
constexpr auto fatal_type_word  = "!fatal";

constexpr auto login_failure_message     = "cannot log in";
constexpr auto already_existing_message  = "already have";
constexpr auto unknown_parameter_message = "unknown parameter";

constexpr std::array error_categories {
    tikpp::error_code::no_such_item,    tikpp::error_code::invalid_argument,
    tikpp::error_code::interrupted,     tikpp::error_code::script_failure,
    tikpp::error_code::general_failure, tikpp::error_code::api_failure,
    tikpp::error_code::tty_failure,     tikpp::error_code::return_value,
};

void set_error_code(tikpp::response &          resp,
                    boost::system::error_code &error) noexcept {
    assert(resp.type() == tikpp::response_type::fatal ||
           resp.type() == tikpp::response_type::trap);

    const auto set_error = [&error](tikpp::error_code err) noexcept {
        error = tikpp::make_error_code(err);
    };

    if (resp.contains("category")) {
        if (auto c = resp.get<unsigned int>("category"); c < 7) {
            return set_error(error_categories[c]);
        }

        return set_error(tikpp::error_code::unknown_error_category);
    }

    if (resp.contains("message")) {
        const auto &message = resp["message"];

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

bool extract_param(
    const std::string &                           word,
    std::unordered_map<std::string, std::string> &words) noexcept {
    std::size_t pos;

    if (word.size() == 0 || word[0] != '=' ||
        (pos = word.find('=', 1)) == std::string::npos) {
        return false;
    }

    words.emplace(
        std::make_pair(word.substr(1, pos - 1), word.substr(pos + 1)));
    return true;
}

bool extract_attribute(
    const std::string &                           word,
    std::unordered_map<std::string, std::string> &words) noexcept {
    std::size_t pos;

    if (word.size() == 0 || word[0] != '.' ||
        (pos = word.find('=', 1)) == std::string::npos) {
        return false;
    }

    words.emplace(std::make_pair(word.substr(0, pos), word.substr(pos + 1)));
    return true;
}

} // namespace

namespace tikpp {

response::response(const std::vector<std::string> &words) {
    assert(is_valid_response(words));

    for (const auto &word : words) {
        ::extract_param(word, words_) || ::extract_attribute(word, words_);
    }

    if (contains(".tag")) {
        tag_.emplace(get<std::uint32_t>(".tag"));
        words_.erase(".tag");
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
        type_  = response_type::fatal;
        error_ = tikpp::make_error_code(tikpp::error_code::fatal_response);
    } else {
        type_ = response_type::unknown;
        error_ =
            tikpp::make_error_code(tikpp::error_code::unknown_response_type);
    }
}

} // namespace tikpp
