#ifndef TIKPP_TESTS_UTIL_RANDOM_HPP
#define TIKPP_TESTS_UTIL_RANDOM_HPP

#include "fmt/format.h"

#include <chrono>
#include <cstddef>
#include <limits>
#include <random>
#include <string>
#include <type_traits>

namespace tikpp::tests::util {

enum class random_string_options : int {
    alpha      = 1,
    num        = 2,
    sym        = 4,
    alphanum   = alpha | num,
    lower_case = 8,
    upper_case = 16,
    mixed_case = lower_case | upper_case,
    mixed      = alpha | num | lower_case | upper_case | sym,
};

inline auto operator&(random_string_options lhs, random_string_options rhs) {
    return static_cast<int>(lhs) & static_cast<int>(rhs);
}

inline auto operator|(random_string_options lhs, random_string_options rhs) {
    return static_cast<random_string_options>(static_cast<int>(lhs) |
                                              static_cast<int>(rhs));
}

std::string random_string(std::size_t len, random_string_options opts) {
    std::mt19937 rng {std::random_device {}()};

    constexpr auto lower_case_alpha = "abcdefghijklmnopqrstuvwxyz";
    constexpr auto upper_case_alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    constexpr auto num              = "1234567890";
    constexpr auto sym              = "`~!@#$%^&*()-_=+\"\\/?.>,<";

    std::string pool, ret;

    if (opts & random_string_options::alpha) {
        if (!(opts & random_string_options::lower_case) &&
            !(opts & random_string_options::upper_case)) {
            fmt::format_to(std::back_inserter(pool), lower_case_alpha);
        } else {
            if (opts & random_string_options::lower_case) {
                fmt::format_to(std::back_inserter(pool), lower_case_alpha);
            }

            if (opts & random_string_options::upper_case) {
                fmt::format_to(std::back_inserter(pool), upper_case_alpha);
            }
        }
    }

    if (opts & random_string_options::num) {
        fmt::format_to(std::back_inserter(pool), num);
    }

    if (opts & random_string_options::sym) {
        fmt::format_to(std::back_inserter(pool), sym);
    }

    std::uniform_int_distribution<std::size_t> dist(0, pool.size() - 1);

    while (len--) {
        fmt::format_to(std::back_inserter(ret), "{}", pool[dist(rng)]);
    }

    return ret;
}

template <typename T,
          typename = std::enable_if_t<std::is_integral_v<T> ||
                                      std::is_floating_point_v<T>>>
T random(T min = std::numeric_limits<T>::min(),
         T max = std::numeric_limits<T>::max()) {
    std::mt19937 rng {std::random_device {}()};

    if constexpr (std::is_integral_v<T>) {
        return std::uniform_int_distribution<T> {min, max}(rng);
    } else {
        return std::uniform_real_distribution<T> {min, max}(rng);
    }
}

} // namespace tikpp::tests::util

#endif
