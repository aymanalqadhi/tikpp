#ifndef TIKPP_DATA_TYPES_BYTES_HPP
#define TIKPP_DATA_TYPES_BYTES_HPP

#include "tikpp/data/types/wrapper.hpp"
#include "tikpp/detail/convert.hpp"

#include "fmt/format.h"

#include <cstdint>
#include <string>

namespace tikpp::data::types {

struct bytes : tikpp::data::types::stateless_wrapper<std::uint64_t> {

    bytes(std::uint64_t b) : stateless_wrapper<std::uint64_t> {std::move(b)} {
    }

    bytes() : bytes {0UL} {
    }

    inline auto kb() const noexcept -> double {
        return value() / 1024.0;
    }

    inline auto mb() const noexcept -> double {
        return kb() / 1024.0;
    }

    inline auto gb() const noexcept -> double {
        return mb() / 1024.0;
    }

    inline auto tb() const noexcept -> double {
        return gb() / 1024.0;
    }

    inline auto pb() const noexcept -> double {
        return tb() / 1024.0;
    }

    auto to_human_readable_string() const noexcept -> std::string {
        switch (static_cast<int>(floor(log2l(value())) / 10)) {
        case 1:
            return fmt::format("{:.2f} KB", kb());
        case 2:
            return fmt::format("{:.2f} MB", mb());
        case 3:
            return fmt::format("{:.2f} GB", gb());
        case 4:
            return fmt::format("{:.2f} TB", tb());
        case 5:
            return fmt::format("{:.2f} PB", pb());
        default:
            return fmt::format("{} B", value());
        }
    }

    inline auto to_string() const noexcept -> std::string {
        return fmt::to_string(value());
    }

    friend std::ostream &operator<<(std::ostream &os, const bytes &b) {
        return os << b.to_human_readable_string();
    }

    using stateless_wrapper<std::uint64_t>::operator=;
};

std::istream &operator>>(std::istream &in, bytes &b) {
    std::uint64_t value {};
    in >> value;

    b = bytes {value};
    return in;
}

namespace literals {

inline auto operator""_b(unsigned long long int b) -> bytes {
    return bytes {b};
}

inline auto operator""_kb(unsigned long long int b) -> bytes {
    return bytes {b * 1024};
}

inline auto operator""_mb(unsigned long long int b) -> bytes {
    return bytes {b * 1024 * 1024};
}

inline auto operator""_gb(unsigned long long int b) -> bytes {
    return bytes {b * 1024 * 1024 * 1024};
}

inline auto operator""_tb(unsigned long long int b) -> bytes {
    return bytes {b * 1024 * 1024 * 1024 * 1024};
}

inline auto operator""_pb(unsigned long long int b) -> bytes {
    return bytes {b * 1024 * 1024 * 1024 * 1024 * 1024};
}

} // namespace literals

} // namespace tikpp::data::types

namespace fmt {

template <>
struct fmt::formatter<tikpp::data::types::bytes> {
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const tikpp::data::types::bytes &b, FormatContext &ctx) {
        return format_to(ctx.out(), b.to_string());
    }
};

} // namespace fmt

#endif
