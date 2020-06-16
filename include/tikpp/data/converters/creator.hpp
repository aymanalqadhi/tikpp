#ifndef TIKPP_DATA_CONVERTERS_CREATOR_HPP
#define TIKPP_DATA_CONVERTERS_CREATOR_HPP

#include "tikpp/detail/convert.hpp"

#include <utility>

namespace tikpp::data::converters {

template <typename HashMap>
struct creator {
    struct [[nodiscard]] item_wrapper {
        template <typename T>
        inline void operator%(T &rhs) {
            rhs = tikpp::detail::convert<T>(value);
        }

        template <template <typename> typename Wrapper, typename T>
        void operator%(Wrapper<T> &rhs) {
            rhs = Wrapper<T> {tikpp::detail::convert<T>(value)};
        }

        const std::string &value;
    };

    inline auto operator()(const std::string &key,
                           const std::string &default_value) const noexcept
        -> item_wrapper {
        return item_wrapper {data.find(key) != data.end() ? data[key]
                                                          : default_value};
    }

    inline auto operator()(const std::string &key) -> item_wrapper {
        static std::string empty_string {};
        return             operator()(key, empty_string);
    }

    template <typename Model>
    inline auto create() noexcept -> Model {
        Model model {};
        model.convert(*this);
        return model;
    }

    HashMap &data;
};

} // namespace tikpp::data::converters

#endif
