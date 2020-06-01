#ifndef TIKPP_DETAIL_CONVERT_HPP
#define TIKPP_DETAIL_CONVERT_HPP

#include "fmt/format.h"
#include <boost/lexical_cast/try_lexical_convert.hpp>

#include <string>
#include <type_traits>

namespace tikpp::detail {

template <typename T>
inline auto convert(const std::string &str) -> std::decay_t<T> {
    using type = std::decay_t<T>;

    if constexpr (std::is_same_v<type, std::string>) {
        return str;
    } else if constexpr (std::is_same_v<type, bool>) {
        return str == "true" || str == "yes";
    } else if constexpr (std::is_constructible_v<T, decltype(str)>) {
        return T {str};
    }

    if constexpr (std::is_same_v<type, std::uint8_t>) {
        std::uint16_t ret {};
        boost::conversion::try_lexical_convert(str, ret);
        return ret & 0xFF;
    }

    type ret {};

    if (!str.empty()) {
        boost::conversion::try_lexical_convert(str, ret);
    }

    return ret;
}

template <typename T>
inline auto convert_back(T value) -> std::string {
    return fmt::to_string(value);
}

} // namespace tikpp::detail

#endif
