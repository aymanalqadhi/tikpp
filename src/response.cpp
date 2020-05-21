#include "tikpp/response.hpp"

#include <regex>
#include <string>

namespace {

constexpr auto normal_type_word = "!done";
constexpr auto data_type_word   = "!re";
constexpr auto trap_type_word   = "!trap";
constexpr auto fatal_type_word  = "!fatal";

constexpr auto tag_key = ".tag";

const std::regex param_pattern {R"(=([^=]+)=(.*))", std::regex::extended};
const std::regex attribute_pattern {R"((\.[^=]+)=(.*))", std::regex::extended};

} // namespace

namespace tikpp {

response::response(const std::vector<std::string> &words) {
    assert(is_valid_response(words));

    const auto &type_word = words[0];

    if (type_word == ::normal_type_word) {
        type_ = response_type::normal;
    } else if (type_word == ::data_type_word) {
        type_ = response_type::data;
    } else if (type_word == ::trap_type_word) {
        type_ = response_type::trap;
    } else if (type_word == ::fatal_type_word) {
        type_ = response_type::fatal;
    } else {
        type_ = response_type::unknown;
    }

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
    }
}

} // namespace tikpp
