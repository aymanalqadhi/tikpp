#ifndef TIKPP_DATA_CONVERTERS_CREATOR_HPP
#define TIKPP_DATA_CONVERTERS_CREATOR_HPP

#include "tikpp/detail/convert.hpp"

#include <utility>

namespace tikpp::data::converters {

template <typename HashMap>
struct creator {
    struct item_wrapper {
        template <typename T>
        inline void operator%(T &rhs) {
            rhs = tikpp::detail::convert<T>(value);
        }

        template <template <typename> typename Wrapper, typename T>
        void operator%(Wrapper<T> &rhs) {
            rhs = Wrapper<T> {std::move(tikpp::detail::convert<T>(value))};
        }

        const std::string &value;
    };

    inline auto operator[](const std::string &key) const noexcept
        -> item_wrapper {
        static std::string empty_str {};

        return item_wrapper {contains(key) ? data[key] : empty_str};
    }

    inline auto contains(const std::string &key) const noexcept -> bool {
        return data.find(key) != data.end();
    }

    HashMap &data;
};

} // namespace tikpp::data::converters

#endif
