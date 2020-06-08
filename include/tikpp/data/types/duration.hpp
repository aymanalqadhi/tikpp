#ifndef TIKPP_DATA_TYPES_DURATION_HPP
#define TIKPP_DATA_TYPES_DURATION_HPP

#include "tikpp/data/types/wrapper.hpp"
#include "tikpp/detail/convert.hpp"

#include "fmt/format.h"

#include <chrono>
#include <string>

namespace tikpp::data::types {

template <typename Duration = std::chrono::seconds>
struct duration : tikpp::data::types::stateless_wrapper<Duration> {
    using rep_type = typename Duration::rep;

    duration(std::chrono::duration<rep_type> value)
        : stateless_wrapper<Duration> {std::move(value)} {
    }

    duration(const std::string &str)
        : duration {(parse_time_unit<std::chrono::hours>(str, 'w') * 24 * 7) +
                    (parse_time_unit<std::chrono::hours>(str, 'd') * 24) +
                    parse_time_unit<std::chrono::hours>(str, 'h') +
                    parse_time_unit<std::chrono::minutes>(str, 'm') +
                    parse_time_unit<std::chrono::seconds>(str, 's') +
                    parse_hhmmss(str)} {
    }

    duration(rep_type value) : duration(Duration {value}) {
    }

    duration() : duration(Duration {0UL}) {
    }

    inline auto to_string() const noexcept -> std::string {
        return fmt::format(
            "{}s",
            std::chrono::duration_cast<std::chrono::seconds>(value()).count());
    }

    inline auto to_human_readable_string() const noexcept -> std::string {
        constexpr auto weeks_mul   = 60 * 60 * 24 * 7;
        constexpr auto days_mul    = 60 * 60 * 24;
        constexpr auto hours_mul   = 60 * 60;
        constexpr auto minutes_mul = 60;
        constexpr auto seconds_mul = 1;

        std::string ret {};

        auto secs =
            std::chrono::duration_cast<std::chrono::seconds>(value()).count();

        if (secs > 0) {
            const auto append_unit = [&secs, &ret](const std::string &label,
                                                   std::size_t mul) noexcept {
                auto amount = secs / mul;

                if (amount > 0) {
                    if (amount > 1) {
                        fmt::format_to(std::back_inserter(ret), "{} {}s",
                                       amount, label);
                    } else {
                        fmt::format_to(std::back_inserter(ret), "{} {}", amount,
                                       label);
                    }

                    secs -= amount * mul;

                    if (secs > 0) {
                        fmt::format_to(std::back_inserter(ret), ", ");
                    }
                }
            };

            append_unit("Week", weeks_mul);
            append_unit("Day", days_mul);
            append_unit("Hour", hours_mul);
            append_unit("Minute", minutes_mul);
            append_unit("Second", seconds_mul);

            return ret;
        }

        return "0 Seconds";
    }

    template <typename Unit>
    static auto parse_time_unit(const std::string &str, char c) -> Duration {
        std::size_t pos;

        if ((pos = str.find(c)) == std::string::npos) {
            return Duration {};
        }

        return std::chrono::duration_cast<Duration>(
            Unit {tikpp::detail::rparse_uint<rep_type>(str, pos)});
    }

    static inline auto parse_hhmmss(const std::string &str) noexcept
        -> Duration {
        std::size_t pos1, pos2;

        if ((pos1 = str.find(':')) == std::string::npos ||
            (pos2 = str.find(':', pos1 + 1)) == std::string::npos) {
            return Duration {};
        }

        return std::chrono::duration_cast<Duration>(std::chrono::hours(
                   tikpp::detail::rparse_uint<rep_type>(str, pos1))) +
               std::chrono::duration_cast<Duration>(std::chrono::minutes {
                   tikpp::detail::parse_uint<rep_type>(str, pos1 + 1)}) +
               std::chrono::duration_cast<Duration>(std::chrono::seconds {
                   tikpp::detail::parse_uint<rep_type>(str, pos2 + 1)});
    }

    friend std::ostream &operator<<(std::ostream &            os,
                                    const duration<Duration> &dur) {
        return os << dur.to_human_readable_string();
    }

    using stateless_wrapper<Duration>::operator=;
    using stateless_wrapper<Duration>::value;
};

template <typename Duration>
std::istream &operator>>(std::istream &in, duration<Duration> &d) {
    std::string str {};
    in >> str;

    d = duration<Duration> {str};
    return in;
}

} // namespace tikpp::data::types

namespace fmt {

template <typename Rep>
struct fmt::formatter<tikpp::data::types::duration<Rep>> {
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const tikpp::data::types::duration<Rep> &d,
                FormatContext &                          ctx) {
        return format_to(ctx.out(), d.to_string());
    }
};

} // namespace fmt

#endif
