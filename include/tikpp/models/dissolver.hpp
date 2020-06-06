#ifndef TIKPP_MODELS_DISSOLVER_HPP
#define TIKPP_MODELS_DISSOLVER_HPP

#include "tikpp/detail/convert.hpp"
#include "tikpp/models/types/one_way.hpp"
#include "tikpp/models/types/readonly.hpp"
#include "tikpp/models/types/two_way.hpp"

#include <string>
#include <type_traits>
#include <utility>

namespace tikpp::models {

template <typename HashMap, bool is_creating>
struct dissolver {
    struct item_wrapper {
        template <typename T>
        inline void dissolve(const T &value) {
            data["=" + key] = tikpp::detail::convert_back(value);
        }

        template <typename T>
        void operator%(const T &value) {
            dissolve(value);
        }

        template <template <typename> typename Wrapper, typename T>
        void operator%(const Wrapper<T> &w) {
            dissolve(w.value());
        }

        template <typename T>
        void operator%(const tikpp::models::types::readonly<T> &w) {
        }

        template <typename T>
        void operator%(const tikpp::models::types::one_way<T> &w) {
            if (is_creating) {
                dissolve(w.value());
            }
        }

        template <typename T>
        void operator%(const tikpp::models::types::two_way<T> &w) {
            if constexpr (!is_creating) {
                if (!w.changed()) {
                    return;
                }
            }

            dissolve(w.value());
        }

        std::string key;
        HashMap &   data;
    };

    inline auto operator[](std::string key) noexcept -> item_wrapper {
        return item_wrapper {std::move(key), data};
    }

    HashMap &data;
};

template <typename HashMap>
using creation_dissolver = dissolver<HashMap, true>;

template <typename HashMap>
using updating_dissolver = dissolver<HashMap, false>;

} // namespace tikpp::models

#endif
