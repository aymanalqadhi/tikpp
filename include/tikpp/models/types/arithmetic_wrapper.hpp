#ifndef TIKPP_MODELS_TYPES_ARTHIMETIC_WRAPPER_HPP
#define TIKPP_MODELS_TYPES_ARTHIMETIC_WRAPPER_HPP

#include "tikpp/detail/type_traits/operators.hpp"

#include <type_traits>
#include <utility>

namespace tikpp::models::types {

template <typename W, typename T>
struct arithmetic_wrapper {
    explicit arithmetic_wrapper(T &value) : value_ {value} {
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_plus_operator_v<U>>>
    inline auto operator+(const T &rhs) const noexcept -> W {
        static_assert(std::is_same_v<U, T>);
        return W {value_ + rhs};
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_minus_operator_v<U>>>
    inline auto operator-(const T &rhs) const noexcept -> W {
        static_assert(std::is_same_v<U, T>);
        return W {value_ - rhs};
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_multiply_operator_v<U>>>
    inline auto operator*(const T &rhs) const noexcept -> W {
        static_assert(std::is_same_v<U, T>);
        return W {value_ * rhs};
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_divide_operator_v<U>>>
    inline auto operator/(const T &rhs) const noexcept -> W {
        static_assert(std::is_same_v<U, T>);
        return W {value_ / rhs};
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_mod_operator_v<U>>>
    inline auto operator%(const arithmetic_wrapper<W, U> &rhs) const noexcept
        -> W {
        static_assert(std::is_same_v<U, T>);
        return W {value_ % rhs};
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_assign_operator_v<U>>>
    inline auto operator=(const W &rhs) noexcept -> W & {
        static_assert(std::is_same_v<U, T>);
        value_ = rhs.value();
        return *static_cast<W *>(this);
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_assign_operator_v<U>>>
    inline auto operator=(W &&rhs) noexcept -> W & {
        value_ = std::move(rhs.value());
        return *static_cast<W *>(this);
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_assign_operator_v<U>>>
    inline auto operator+=(const T &rhs) noexcept -> W & {
        static_assert(std::is_same_v<U, T>);
        value_ += rhs;
        return *static_cast<W *>(this);
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_minus_assign_operator_v<U>>>
    inline auto operator-=(const T &rhs) noexcept -> W & {
        static_assert(std::is_same_v<U, T>);
        value_ -= rhs;
        return *static_cast<W *>(this);
    }

    template <
        typename U = T,
        typename   = std::enable_if_t<
            tikpp::detail::type_traits::has_multiply_assign_operator_v<U>>>
    inline auto operator*=(const T &rhs) noexcept -> W & {
        static_assert(std::is_same_v<U, T>);
        value_ *= rhs;
        return *static_cast<W *>(this);
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_divide_assign_operator_v<U>>>
    inline auto operator/=(const T &rhs) noexcept -> W & {
        static_assert(std::is_same_v<U, T>);
        value_ /= rhs;
        return *static_cast<W *>(this);
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_mod_assign_operator_v<U>>>
    inline auto operator%=(const T &rhs) noexcept -> W & {
        static_assert(std::is_same_v<U, T>);
        value_ %= rhs;
        return *static_cast<W *>(this);
    }

    inline operator T &() noexcept {
        return value_;
    }

    inline operator const T &() const noexcept {
        return value_;
    }

    inline auto value() noexcept -> T & {
        return value_;
    }

    inline auto value() const noexcept -> const T & {
        return value_;
    }

  private:
    T &value_;
};

} // namespace tikpp::models::types

#endif
