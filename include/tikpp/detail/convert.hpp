#ifndef TIKPP_DETAIL_CONVERT_HPP
#define TIKPP_DETAIL_CONVERT_HPP

#include "fmt/format.h"
#include <boost/lexical_cast/try_lexical_convert.hpp>

#include <string>
#include <type_traits>

namespace tikpp::detail {

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
auto integeral_pow(T base, T exp) -> T {
    T result {1};

    while (true) {
        if (exp & 1) {
            result *= base;
        }

        if (!(exp >>= 1)) {
            break;
        }

        base *= base;
    }

    return result;
}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
static inline auto rparse_uint(const std::string &str, std::size_t pos) noexcept
    -> T {
    T    ret {};
    char c;

    for (std::size_t i {pos}; i > 0; --i) {
        c = str[i - 1];

        if (!std::isdigit(c)) {
            break;
        }

        ret += integeral_pow<T>(10, pos - i) * (c - '0');
    }

    if constexpr (std::is_unsigned_v<T>) {
        if (c == '-') {
            ret = ~ret + 1;
        }
    }

    return ret;
}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
static inline auto parse_uint(const std::string &str,
                              std::size_t        pos = 0) noexcept -> T {
    T    ret {};
    char c;

    std::size_t idx {pos};

    while ((c = str[idx++])) {
        if (!std::isdigit(c)) {
            break;
        }

        ret = ret * 10 + (c - '0');
    }

    if constexpr (std::is_unsigned_v<T>) {
        if (str[pos] == '-') {
            ret = ~ret + 1;
        }
    }

    return ret;
}

template <typename T>
inline auto convert(const std::string &str) -> std::decay_t<T> {
    using type = std::decay_t<T>;

    if constexpr (std::is_same_v<type, std::string>) {
        return str;
    } else if constexpr (std::is_constructible_v<type, decltype(str)>) {
        return T {str};
    }

    if constexpr (std::is_same_v<type, bool>) {
        return str == "true" || str == "yes";
    } else if constexpr (std::is_integral_v<type> && std::is_unsigned_v<type>) {
        return parse_uint<type>(str);
    }

    type ret {};

    if (!str.empty()) {
        boost::conversion::try_lexical_convert(str, ret);
    }

    return ret;
}

template <typename T>
inline auto convert_back(const T &value) -> std::string {
    using type = std::decay_t<T>;

    if constexpr (std::is_same_v<type, std::string>) {
        return value;
    } else if constexpr (std::is_constructible_v<std::string, type>) {
        return std::string {value};
    }

    return fmt::to_string(value);
}

} // namespace tikpp::detail

#endif
