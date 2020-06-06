#ifndef TIKPP_MODELS_PROPLIST_COLLECTOR_HPP
#define TIKPP_MODELS_PROPLIST_COLLECTOR_HPP

#include <string>
#include <utility>

namespace tikpp::models {

template <typename Container>
struct proplist_collector {
    struct dummy_wrapper {
        template <typename T>
        void operator%(T &&value) {
        }
    };

    inline auto operator[](std::string key) -> dummy_wrapper {
        proplist.emplace_back(std::move(key));
        return dummy_wrapper {};
    }

    Container proplist;
};

} // namespace tikpp::models

#endif
