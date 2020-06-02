#ifndef TIKPP_MODELS_TYPES_READ_ONLY_HPP
#define TIKPP_MODELS_TYPES_READ_ONLY_HPP

#include <utility>

namespace tikpp::models::types {

template <typename T>
struct one_way {
    using type = T;

    one_way() = default;

    one_way(T &&val) : value_ {std::forward<T>(val)} {
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
