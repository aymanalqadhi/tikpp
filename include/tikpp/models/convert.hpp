#ifndef TIKPP_MODELS_CONVERT_HPP
#define TIKPP_MODELS_CONVERT_HPP

#include "tikpp/detail/type_traits/model.hpp"

#include "fmt/format.h"
#include <boost/lexical_cast/try_lexical_convert.hpp>

#include <cassert>
#include <string>
#include <unordered_map>

namespace tikpp::models {

template <typename T>
inline auto convert(const std::string &str) -> std::decay_t<T> {
    using type = std::decay_t<T>;

    if constexpr (std::is_same_v<type, std::string>) {
        return str;
    } else if constexpr (std::is_same_v<type, bool>) {
        return str == "true" || str == "yes";
    }

    type ret {};
    boost::conversion::try_lexical_convert(str, ret);

    return ret;
}

template <typename T>
inline auto convert_back(T value) -> std::string {
    return fmt::to_string(value);
}

template <typename HashMap,
          typename = std::enable_if_t<
              tikpp::detail::type_traits::is_hash_map_v<HashMap>>>
struct hash_map_wrapper {
    inline auto contains(const std::string &key) const noexcept -> bool {
        return data.find(key) != data.end();
    }

    HashMap &data;
};

template <typename HashMap>
struct creator : hash_map_wrapper<HashMap> {
    struct item_wrapper {
        template <typename T>
        inline void operator%(T &rhs) {
            rhs = convert<T>(value);
        }

        const std::string &value;
    };

    inline auto operator[](const std::string &key) const noexcept
        -> item_wrapper {
        static std::string empty_str {};

        return item_wrapper {hash_map_wrapper<HashMap>::contains(key)
                                 ? hash_map_wrapper<HashMap>::data[key]
                                 : empty_str};
    }
};

template <typename HashMap>
struct dissolver : hash_map_wrapper<HashMap> {
    struct item_wrapper {
        template <typename T>
        void operator%(T &&value) {
            data["=" + std::move(key)] = convert_back(std::forward<T>(value));
        }

        std::string key;
        HashMap &   data;
    };

    inline auto operator[](std::string key) noexcept -> item_wrapper {
        return item_wrapper {std::move(key), hash_map_wrapper<HashMap>::data};
    }
};

template <typename Container>
struct proplist_collector {
    struct dummy_wrapper {
        template <typename T>
        void operator%(T &&value) {
        }
    };

    inline auto operator[](std::string key) -> dummy_wrapper {
        proplist.emplace_back(std::move(key));
        return dummy_wrapper {};
    }

    Container proplist;
};

} // namespace tikpp::models

#endif
