#ifndef TIKPP_MODELS_TYPES_DURATION_HPP
#define TIKPP_MODELS_TYPES_DURATION_HPP

#include "tikpp/detail/convert.hpp"
#include "tikpp/models/types/arithmetic_wrapper.hpp"
#include "tikpp/models/types/logic_wrapper.hpp"

#include "fmt/format.h"

#include <chrono>
#include <string>

namespace tikpp::models::types {

template <typename Duration = std::chrono::seconds>
struct duration
    : tikpp::models::types::arithmetic_wrapper<duration<Duration>, Duration>,
      tikpp::models::types::logic_wrapper<Duration> {
    using rep_type = typename Duration::rep;

    duration(std::chrono::duration<rep_type> value)
        : value_ {std::chrono::duration_cast<Duration>(value)},
          arithmetic_wrapper<duration<Duration>, Duration> {value_},
          logic_wrapper<Duration> {value_} {
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

    inline auto to_string() const noexcept -> std::string {
        return fmt::format(
            "{}s",
            std::chrono::duration_cast<std::chrono::seconds>(value_).count());
    }

    inline auto to_human_readable_string() const noexcept -> std::string {
        constexpr auto weeks_mul   = 60 * 60 * 24 * 7;
        constexpr auto days_mul    = 60 * 60 * 24;
        constexpr auto hours_mul   = 60 * 60;
        constexpr auto minutes_mul = 60;
        constexpr auto seconds_mul = 1;

        std::string ret {};

        auto secs =
            std::chrono::duration_cast<std::chrono::seconds>(value_).count();

        const auto append_unit = [&secs, &ret](const std::string &label,
                                               std::size_t mul) noexcept {
            auto amount = secs / mul;

            if (amount > 0) {
                if (amount > 1) {
                    fmt::format_to(std::back_inserter(ret), "{} {}s", amount,
                                   label);
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

    using arithmetic_wrapper<duration<Duration>, Duration>::operator=;

  private:
    Duration value_;
};

} // namespace tikpp::models::types

#endif
