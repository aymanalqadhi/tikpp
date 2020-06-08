#ifndef TIKPP_DATA_TYPES_INDENTITY_HPP
#define TIKPP_DATA_TYPES_INDENTITY_HPP

#include "fmt/format.h"

#include <cstdint>
#include <string>

namespace tikpp::data::types {

struct identity {
    identity() = default;

    identity(std::uint32_t id) : value_ {id} {
    }

    identity(const std::string &str) {
        if (str.empty() || str[0] != '*') {
            value_ = 0;
        } else {
            value_ = std::strtoul(str.c_str() + 1, nullptr, 16);
        }
    }

    inline operator std::uint32_t() const noexcept {
        return value_;
    }

    inline auto value() const noexcept -> std::uint32_t {
        return value_;
    }

    inline auto to_string() const noexcept -> std::string {
        return fmt::format("*{:X}", value_);
    }

    friend std::ostream &operator<<(std::ostream &os, const identity &id) {
        return os << id.to_string();
    }

  private:
    std::uint32_t value_;
};

std::istream &operator>>(std::istream &in, identity &id) {
    std::string id_str {};
    in >> id_str;

    id = identity {id_str};
    return in;
}

namespace literals {

auto operator""_i(unsigned long long int id) -> identity {
    return identity {static_cast<std::uint32_t>(id)};
}

} // namespace literals

} // namespace tikpp::data::types

template <>
struct fmt::formatter<tikpp::data::types::identity> {
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const tikpp::data::types::identity &id, FormatContext &ctx) {
        return format_to(ctx.out(), "*{:X}", id.value());
    }
};

#endif
