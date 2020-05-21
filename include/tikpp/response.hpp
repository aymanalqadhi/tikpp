#ifndef TIKPP_RESPONSE_HPP
#define TIKPP_RESPONSE_HPP

#include "tikpp/sentence.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace tikpp {

enum class response_type { normal, data, trap, fatal, unknown };

struct response : sentence {
    response(const std::vector<std::string> &words);

    [[nodiscard]] inline auto type() const noexcept -> response_type {
        return type_;
    }

    [[nodiscard]] inline auto tag() const noexcept
        -> const std::optional<std::uint32_t> & {
        return tag_;
    }

    [[nodiscard]] static inline auto
    is_valid_response(const std::vector<std::string> &words) -> bool {
        return !words.empty() && words[0][0] == '!';
    }

  protected:
    response_type                type_;
    std::optional<std::uint32_t> tag_;
};

} // namespace tikpp

#endif
