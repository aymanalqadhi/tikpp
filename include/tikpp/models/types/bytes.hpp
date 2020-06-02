#ifndef TIKPP_MODELS_TYPES_BYTES_HPP
#define TIKPP_MODELS_TYPES_BYTES_HPP

#include "tikpp/detail/convert.hpp"

#include <fmt/format.h>
#include <fmt/printf.h>

#include <cmath>
#include <cstdint>
#include <string>

namespace tikpp::models::types {

struct bytes {
    bytes() = default;

    bytes(std::uint64_t b) : bytes_ {b} {
    }

    inline operator std::uint64_t &() noexcept {
        return bytes_;
    }

    inline auto value() const noexcept -> std::uint64_t {
        return bytes_;
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

    auto to_string() const noexcept -> std::string {
        switch (static_cast<int>(floor(log2l(bytes_)) / 10)) {
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

    inline auto operator+(const bytes &b) const noexcept -> bytes {
        return {value() + b.value()};
    }

    inline auto operator+=(const bytes &b) noexcept -> bytes & {
        bytes_ += b.value();
        return *this;
    }

    inline auto operator-(const bytes &b) const noexcept -> bytes {
        return {value() - b.value()};
    }

    inline auto operator-=(const bytes &b) noexcept -> bytes & {
        bytes_ -= b.value();
        return *this;
    }

    inline auto operator*(const bytes &b) const noexcept -> bytes {
        return {value() * b.value()};
    }

    inline auto operator*=(const bytes &b) noexcept -> bytes & {
        bytes_ *= b.value();
        return *this;
    }

    inline auto operator/(const bytes &b) const noexcept -> bytes {
        return {value() / b.value()};
    }

    inline auto operator/=(const bytes &b) noexcept -> bytes & {
        bytes_ /= b.value();
        return *this;
    }

    inline auto operator==(const bytes &b) const noexcept -> bool {
        return bytes_ == b.bytes_;
    }

    inline auto operator!=(const bytes &b) const noexcept -> bool {
        return bytes_ != b.bytes_;
    }

    inline auto operator<(const bytes &b) const noexcept -> bool {
        return bytes_ < b.bytes_;
    }

    inline auto operator>(const bytes &b) const noexcept -> bool {
        return bytes_ > b.bytes_;
    }

    inline auto operator<=(const bytes &b) const noexcept -> bool {
        return bytes_ <= b.bytes_;
    }

    inline auto operator>=(const bytes &b) const noexcept -> bool {
        return bytes_ >= b.bytes_;
    }

    friend std::ostream &operator<<(std::ostream &os, const bytes &b) {
        return os << b.to_string();
    }

  private:
    std::uint64_t bytes_;
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

} // namespace tikpp::models::types

template <>
struct fmt::formatter<tikpp::models::types::bytes> {
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const tikpp::models::types::bytes &b, FormatContext &ctx) {
        return format_to(ctx.out(), "{}", b.value());
    }
};

#endif
