#ifndef TIKPP_DATA_CONVERTERS_DISSOLVER_HPP
#define TIKPP_DATA_CONVERTERS_DISSOLVER_HPP

#include "tikpp/detail/convert.hpp"
#include "tikpp/data/types/wrapper.hpp"

#include <string>
#include <utility>

namespace tikpp::data::converters {

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
        void operator%(const tikpp::data::types::readonly<T> &w) {
            if constexpr (!is_creating) {
                dissolve(w.value());
            }
        }

        template <typename T>
        void operator%(const tikpp::data::types::one_way<T> &w) {
            if constexpr (is_creating) {
                dissolve(w.value());
            }
        }

        template <typename T>
        void operator%(tikpp::data::types::two_way<T> &w) {
            if constexpr (!is_creating) {
                if (!w.changed()) {
                    return;
                }
            }

            dissolve(w.value());
            w.changed(false);
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
struct creation_dissolver : dissolver<HashMap, true> {};

template <typename HashMap>
struct updating_dissolver : dissolver<HashMap, false> {};

} // namespace tikpp::data::converters

#endif
