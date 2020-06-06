#ifndef TIKPP_MODELS_TYPES_LOGIC_WRAPPER_HPP
#define TIKPP_MODELS_TYPES_LOGIC_WRAPPER_HPP

#include "tikpp/detail/type_traits/operators.hpp"

#include <type_traits>

namespace tikpp::models::types {

template <typename T>
struct logic_wrapper {
    explicit logic_wrapper<T>(T &value) : value_ {value} {
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_equals_operator_v<U>>>
    inline auto operator==(const logic_wrapper<T> &rhs) const noexcept -> bool {
        return value_ == rhs.value_;
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_not_equals_operator_v<U>>>
    inline auto operator!=(const logic_wrapper<T> &rhs) const noexcept -> bool {
        return value_ != rhs.value_;
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_less_than_operator_v<U>>>
    inline auto operator<(const logic_wrapper<T> &rhs) const noexcept -> bool {
        return value_ < rhs.value_;
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::has_greater_than_operator_v<U>>>
    inline auto operator>(const logic_wrapper<T> &rhs) const noexcept -> bool {
        return value_ > rhs.value_;
    }

    template <
        typename U = T,
        typename   = std::enable_if_t<
            tikpp::detail::type_traits::has_less_than_or_equals_operator_v<U>>>
    inline auto operator<=(const logic_wrapper<T> &rhs) const noexcept -> bool {
        return value_ <= rhs.value_;
    }

    template <typename U = T,
              typename   = std::enable_if_t<
                  tikpp::detail::type_traits::
                      has_greater_than_or_equals_operator_v<U>>>
    inline auto operator>=(const logic_wrapper<T> &rhs) const noexcept -> bool {
        return value_ >= rhs.value_;
    }

  private:
    T &value_;
};

} // namespace tikpp::models::types

#endif
