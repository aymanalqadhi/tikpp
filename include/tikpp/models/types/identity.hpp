#ifndef TIKPP_MODELS_TYPES_INDENTITY_HPP
#define TIKPP_MODELS_TYPES_INDENTITY_HPP

#include "fmt/format.h"

#include <cstdint>
#include <string>

namespace tikpp::models::types {

struct identity {
    identity() = default;

    identity(std::uint32_t id) : value {id} {
    }

    identity(const std::string &str) {
        if (str.empty() || str[0] != '*') {
            value = 0;
        } else {
            value = std::strtoul(str.c_str() + 1, nullptr, 16);
        }
    }

    inline auto to_string() const noexcept -> std::string {
        return fmt::format("*{:X}", value);
    }

    inline operator std::uint32_t() const noexcept {
        return value;
    }

    std::uint32_t value;
};

std::istream &operator>>(std::istream &in, identity &id) {
    std::string id_str {};
    in >> id_str;

    id = identity {id_str};
    return in;
}

namespace literals {

auto operator""_i(unsigned long long int id) -> identity {
    identity ret {};
    ret.value = id;

    return ret;
}

} // namespace literals

} // namespace tikpp::models::types

template <>
struct fmt::formatter<tikpp::models::types::identity> {
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const tikpp::models::types::identity &id, FormatContext &ctx) {
        return format_to(ctx.out(), "*{:X}", id.value);
    }
};

#endif
