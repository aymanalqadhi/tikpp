#ifndef TIKPP_DATA_CONVERTERS_PROPLIST_COLLECTOR_HPP
#define TIKPP_DATA_CONVERTERS_PROPLIST_COLLECTOR_HPP

#include <string>
#include <utility>

namespace tikpp::data::converters {

template <typename Container>
struct proplist_collector {
    template <typename T, typename U>
    inline void operator()(std::string key,
                           [[maybe_unused]] T &,
                           [[maybe_unused]] const U &) {
        proplist.emplace_back(std::move(key));
    }

    template <typename T>
    inline void operator()(std::string key, [[maybe_unused]] T &) {
        proplist.emplace_back(std::move(key));
    }

    Container proplist;
};

} // namespace tikpp::data::converters

#endif
