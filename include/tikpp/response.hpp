#ifndef TIKPP_RESPONSE_HPP
#define TIKPP_RESPONSE_HPP

#include "tikpp/sentence.hpp"

#include <boost/system/error_code.hpp>

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace tikpp {

enum class response_type { normal, data, trap, fatal, unknown };

struct response : sentence {
    response() = default;

    response(const std::vector<std::string> &words);

    [[nodiscard]] inline auto type() const noexcept -> response_type {
        return type_;
    }

    [[nodiscard]] inline auto tag() const noexcept
        -> const std::optional<std::uint32_t> & {
        return tag_;
    }

    [[nodiscard]] inline auto error() const noexcept
        -> const boost::system::error_code & {
        return error_;
    }

    [[nodiscard]] static inline auto
    is_valid_response(const std::vector<std::string> &words) -> bool {
        return !words.empty() && words[0][0] == '!';
    }

  protected:
    response_type                type_ {};
    std::optional<std::uint32_t> tag_;
    boost::system::error_code    error_ {};
};

} // namespace tikpp

#endif
