#ifndef TIKPP_MODELS_TYPES_TWO_WAY_HPP
#define TIKPP_MODELS_TYPES_TWO_WAY_HPP

#include "tikpp/detail/type_traits/operators.hpp"
#include "tikpp/models/types/arithmetic_wrapper.hpp"
#include "tikpp/models/types/logic_wrapper.hpp"

#include <type_traits>
#include <utility>

namespace tikpp::models::types {

template <typename T>
struct two_way : tikpp::models::types::arithmetic_wrapper<two_way<T>, T>,
                 tikpp::models::types::logic_wrapper<T> {
    two_way(T &&value)
        : value_ {std::move(value)},
          arithmetic_wrapper<two_way<T>, T> {value_},
          logic_wrapper<T> {value_},
          changed_ {false} {
    }

    two_way(const T &value) : two_way(static_cast<T>(value)) {
    }

    two_way() : two_way {T {}} {
    }

    inline auto changed() const noexcept -> bool {
        return changed_;
    }

    inline void changed(bool value) noexcept {
        changed_ = value;
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_assign_operator_v<U>>>
    inline auto operator=(const two_way<U> &rhs) -> two_way<U> & {
        static_assert(std::is_same_v<U, T>);

        value_   = rhs.value_;
        changed_ = true;

        return *this;
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_assign_operator_v<U>>>
    inline auto operator=(two_way<U> &&rhs) -> two_way<U> & {
        static_assert(std::is_same_v<U, T>);

        value_   = std::move(rhs.value_);
        changed_ = true;

        return *this;
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_plus_assign_operator_v<U>>>
    inline auto operator+=(const two_way<U> &rhs) -> two_way<U> & {
        static_assert(std::is_same_v<U, T>);

        value_ += rhs.value_;
        changed_ = true;

        return *this;
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_minus_assign_operator_v<U>>>
    inline auto operator-=(const two_way<U> &rhs) -> two_way<U> & {
        static_assert(std::is_same_v<U, T>);

        value_ -= rhs.value_;
        changed_ = true;

        return *this;
    }

    template <
        typename U = T,
        typename   = std::enable_if_t<
            tikpp::detail::type_traits::has_multiply_assign_operator_v<U>>>
    inline auto operator*=(const two_way<U> &rhs) -> two_way<U> & {
        static_assert(std::is_same_v<U, T>);

        value_ *= rhs.value_;
        changed_ = true;

        return *this;
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_divide_assign_operator_v<U>>>
    inline auto operator/=(const two_way<U> &rhs) -> two_way<U> & {
        static_assert(std::is_same_v<U, T>);

        value_ /= rhs.value_;
        changed_ = true;

        return *this;
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_mod_assign_operator_v<U>>>
    inline auto operator%=(const two_way<U> &rhs) -> two_way<U> & {
        static_assert(std::is_same_v<U, T>);

        value_ %= rhs;
        changed_ = true;

        return *this;
    }

  private:
    T    value_;
    bool changed_;
};

} // namespace tikpp::models::types

#endif
