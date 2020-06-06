#ifndef TIKPP_MODELS_TYPES_READ_ONLY_HPP
#define TIKPP_MODELS_TYPES_READ_ONLY_HPP

#include "tikpp/models/types/arithmetic_wrapper.hpp"
#include "tikpp/models/types/logic_wrapper.hpp"

#include <utility>

namespace tikpp::models::types {

template <typename T>
struct one_way : tikpp::models::types::arithmetic_wrapper<one_way<T>, T>,
                 tikpp::models::types::logic_wrapper<T> {
    one_way(T &&value)
        : value_ {std::move(value)},
          arithmetic_wrapper<one_way<T>, T> {value_},
          logic_wrapper<T> {value_} {
    }

    one_way(const T &value) : one_way(static_cast<T>(value)) {
    }

    one_way() : one_way {T {}} {
    }

    using arithmetic_wrapper<one_way<T>, T>::operator=;

  private:
    T value_;
};

} // namespace tikpp::models::types

#endif
