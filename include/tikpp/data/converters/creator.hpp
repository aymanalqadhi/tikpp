#ifndef TIKPP_DATA_CONVERTERS_CREATOR_HPP
#define TIKPP_DATA_CONVERTERS_CREATOR_HPP

#include "tikpp/detail/convert.hpp"

#include <utility>

namespace tikpp::data::converters {

template <typename HashMap>
struct creator {
    template <typename T,
              typename = std::enable_if_t<!std::is_same_v<T, std::string>>>
    inline void assign(T &lhs, const T &rhs) const noexcept {
        lhs = rhs;
    }

    template <template <typename> typename Wrapper,
              typename T,
              typename = std::enable_if_t<!std::is_same_v<T, std::string>>>
    inline void assign(Wrapper<T> &lhs, const T &rhs) const noexcept {
        lhs = Wrapper<T> {rhs};
    }

    template <typename T>
    inline void assign(T &lhs, const std::string &rhs) const noexcept {
        lhs = tikpp::detail::convert<T>(rhs);
    }

    template <template <typename> typename Wrapper, typename T>
    inline void assign(Wrapper<T> &lhs, const std::string &rhs) const noexcept {
        lhs = Wrapper<T> {tikpp::detail::convert<T>(rhs)};
    }

    template <typename T>
    inline void operator()(const std::string &key, T &value) {
        if (data.find(key) != data.end()) {
            assign(value, data[key]);
        }
    }

    template <typename T, typename U>
    inline void
    operator()(const std::string &key, T &value, const U &default_value) {
        if (data.find(key) != data.end()) {
            assign(value, data[key]);
        } else {
            assign(value, default_value);
        }
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
