#ifndef TIKPP_DATA_QUERY_HPP
#define TIKPP_DATA_QUERY_HPP

#include "fmt/format.h"

#include <string>
#include <vector>

namespace tikpp::data {

struct query {
    query(std::string &&word) : words {std::move(word)} {
    }

    query(std::vector<std::string> w) : words {std::move(w)} {
    }

    inline auto operator==(const query &q) const noexcept -> bool {
        return std::equal(q.words.begin(), q.words.end(), words.begin(),
                          words.end());
    }

    inline auto operator!() -> query {
        query ret {words};

        if (ret.words.back() == "?#!") {
            ret.words.pop_back();
        } else {
            ret.words.push_back("?#!");
        }

        return ret;
    }

    inline auto operator&&(query q) -> query & {
        if (*this == q) {
            return *this;
        }

        extend(q);
        words.push_back("?#&");

        return *this;
    }

    inline auto operator||(query q) -> query & {
        if (*this == q) {
            return *this;
        }

        extend(q);
        words.push_back("?#|");

        return *this;
    }

    inline auto operator^(query q) -> query & {
        query tmp {words};
        return (*this && !q) || (!tmp && q);
    }

    inline operator std::vector<std::string> &() {
        return words;
    }

    inline void extend(const query &q) {
        for (const auto &w : q.words) {
            words.push_back(w);
        }
    }

    std::vector<std::string> words;
};

struct query_token {
    inline operator query() {
        return query {"?" + name};
    }

    inline auto operator!() -> query {
        return query {"?-" + name};
    }

    template <typename T>
    inline auto operator==(const T &t) -> query {
        return fmt::format("?={}={}", name, t);
    }

    template <typename T>
    inline auto operator!=(const T &t) -> query {
        return !(*this == t);
    }

    template <typename T>
    inline auto operator<(const T &t) -> query {
        return fmt::format("?<{}={}", name, t);
    }

    template <typename T>
    inline auto operator>(const T &t) -> query {
        return fmt::format("?>{}={}", name, t);
    }

    template <typename T>
    inline auto operator<=(const T &t) -> query {
        return *this < t || *this == t;
    }

    template <typename T>
    inline auto operator>=(const T &t) -> query {
        return *this > t || *this == t;
    }

    std::string name;
};

template <typename... Token>
inline decltype(auto) make_tokens(Token &&... tokens) {
    return std::make_tuple(query_token {std::forward<Token>(tokens)}...);
}

namespace literals {
auto operator""_t(const char *str, std::size_t len)
    -> tikpp::data::query_token {
    return {std::string {str, len}};
}

} // namespace literals

} // namespace tikpp::data

#endif
