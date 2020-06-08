#ifndef TIKPP_MODELS_TYPES_WRAPPER_HPP
#define TIKPP_MODELS_TYPES_WRAPPER_HPP

#include "tikpp/detail/type_traits/operators.hpp"

#include <type_traits>
#include <utility>

namespace tikpp::models::types {

template <typename T>
struct stateless_value_wrapper {
    stateless_value_wrapper(T &&val) : value_ {std::move(val)} {
    }

    stateless_value_wrapper(const T &val) : value_ {val} {
    }

    stateless_value_wrapper() = default;

    inline operator T &() noexcept {
        return value_;
    }

    inline operator const T &() const noexcept {
        return value_;
    }

    inline auto value() const noexcept -> const T & {
        return value_;
    }

    template <typename U = T,
              typename   = std::enable_if_t<std::is_copy_assignable_v<T>>>
    inline void value(const T &v) {
        value_ = v;
    }

    template <typename U = T,
              typename   = std::enable_if_t<std::is_move_assignable_v<T>>>
    inline void value(T &&v) {
        value_ = std::move(v);
    }

  private:
    T value_;
};

template <typename T>
struct stateful_value_wrapper : stateless_value_wrapper<T> {
    stateful_value_wrapper(T &&val)
        : stateless_value_wrapper<T> {std::move(val)}, changed_ {false} {
    }

    stateful_value_wrapper(const T &val)
        : stateless_value_wrapper<T> {val}, changed_ {false} {
    }

    stateful_value_wrapper() = default;

    template <typename U = T,
              typename   = std::enable_if_t<std::is_copy_assignable_v<T>>>
    inline void value(const T &v) noexcept {
        changed_ = true;
        stateless_value_wrapper<T>::value(v);
    }

    template <typename U = T,
              typename   = std::enable_if_t<std::is_move_assignable_v<T>>>
    inline void value(T &&v) noexcept {
        changed_ = true;
        stateless_value_wrapper<T>::value(std::move(v));
    }

    inline auto changed() const noexcept -> bool {
        return changed_;
    }

    inline void changed(bool val) noexcept {
        changed_ = val;
    }

    using stateless_value_wrapper<T>::value;

  private:
    bool changed_;
};

template <template <typename> typename Wrapper, typename T>
struct type_wrapper : Wrapper<T> {
    type_wrapper(T &&val) : Wrapper<T> {std::move(val)} {
    }

    type_wrapper(const T &val) : Wrapper<T> {val} {
    }

    type_wrapper() = default;

    template <typename U = T,
              typename   = std::enable_if_t<std::is_copy_assignable_v<T>>>
    inline auto operator=(const T &rhs) -> type_wrapper<Wrapper, T> & {
        Wrapper<T>::value(rhs);
        return *this;
    }

    template <typename U = T,
              typename   = std::enable_if_t<std::is_move_assignable_v<T>>>
    inline auto operator=(T &&rhs) -> type_wrapper<Wrapper, T> & {
        Wrapper<T>::value(std::move(rhs));
        return *this;
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_plus_operator_v<U>>>
    inline auto operator+=(const T &rhs) -> type_wrapper<Wrapper, T> & {
        Wrapper<T>::value(Wrapper<T>::value() + rhs);
        return *this;
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_minus_operator_v<U>>>
    inline auto operator-=(const T &rhs) -> type_wrapper<Wrapper, T> & {
        Wrapper<T>::value(Wrapper<T>::value() - rhs);
        return *this;
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_multiply_operator_v<U>>>
    inline auto operator*=(const T &rhs) -> type_wrapper<Wrapper, T> & {
        Wrapper<T>::value(Wrapper<T>::value() * rhs);
        return *this;
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_divide_operator_v<U>>>
    inline auto operator/=(const T &rhs) -> type_wrapper<Wrapper, T> & {
        Wrapper<T>::value(Wrapper<T>::value() / rhs);
        return *this;
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_mod_operator_v<U>>>
    inline auto operator%=(const T &rhs) -> type_wrapper<Wrapper, T> & {
        Wrapper<T>::value(Wrapper<T>::value() % rhs);
        return *this;
    }
};

template <typename T>
using stateless_wrapper = type_wrapper<stateless_value_wrapper, T>;

template <typename T>
using stateful_wrapper = type_wrapper<stateful_value_wrapper, T>;

template <typename T>
struct readonly {
    readonly() = default;

    readonly(T &&val) : value_ {std::forward<T>(val)} {
    }

    readonly(const T &val) : value_ {static_cast<T>(val)} {
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

template <typename T>
struct one_way : stateless_wrapper<T> {
    using stateless_wrapper<T>::operator=;
};

template <typename T>
struct two_way : stateful_wrapper<T> {
    using stateful_wrapper<T>::operator=;
};

} // namespace tikpp::models::types

#endif
