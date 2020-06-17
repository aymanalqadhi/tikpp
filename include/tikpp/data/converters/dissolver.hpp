#ifndef TIKPP_DATA_CONVERTERS_DISSOLVER_HPP
#define TIKPP_DATA_CONVERTERS_DISSOLVER_HPP

#include "tikpp/data/types/wrapper.hpp"
#include "tikpp/detail/convert.hpp"

#include <string>
#include <utility>

namespace tikpp::data::converters {

template <typename HashMap, bool is_creating>
struct dissolver {
    template <typename T>
    inline void dissolve(const std::string &key, const T &value) {
        data["=" + key] = tikpp::detail::convert_back(value);
    }

    template <typename T>
    inline void operator()(const std::string &key, const T &value) {
        dissolve(key, value);
    }

    template <typename T>
    inline void operator()(const std::string &key,
                           const T &          value,
                           [[maybe_unused]] const T &) {
        operator()<T>(key, value);
    }

    template <template <typename> typename Wrapper, typename T>
    inline void operator()(const std::string &key, const Wrapper<T> &w) {
        dissolve(key, w.value());
    }

    template <typename T>
    inline void operator()(const std::string &                  key,
                           const tikpp::data::types::sticky<T> &w) {
        if constexpr (!is_creating) {
            dissolve(key, w.value());
        }
    }

    template <typename T>
    inline void operator()(const std::string &                     key,
                           const tikpp::data::types::read_only<T> &w) {
        if constexpr (is_creating) {
            dissolve(key, w.value());
        }
    }

    template <typename T>
    void operator()(const std::string &                key,
                    tikpp::data::types::read_write<T> &w) {
        if constexpr (!is_creating) {
            if (!w.changed()) {
                return;
            }
        }

        dissolve(key, w.value());
        w.changed(false);
    }

    template <template <typename> typename Wrapper, typename T>
    inline void operator()(const std::string &key,
                           const Wrapper<T> & w,
                           [[maybe_unused]] const T &) {
        operator()<Wrapper, T>(key, w.value());
    }

    HashMap &data;
};

template <typename HashMap>
struct creation_dissolver : dissolver<HashMap, true> {};

template <typename HashMap>
struct updating_dissolver : dissolver<HashMap, false> {};

} // namespace tikpp::data::converters

#endif
