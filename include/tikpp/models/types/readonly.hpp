#ifndef TIKPP_MODELS_TYPES_ONE_WAY_HPP
#define TIKPP_MODELS_TYPES_ONE_WAY_HPP

#include <utility>

namespace tikpp::models::types {

template <typename T>
struct readonly {
    using type = T;

    readonly() = default;

    readonly(T &&val) : value_ {std::forward<T>(val)} {
    }

    inline operator const T &() const noexcept {
        return value_;
    }

    inline auto value() const noexcept -> const T & {
        return value_;
    }

  private:
    T value_;
};

} // namespace tikpp::models::types

#endif
