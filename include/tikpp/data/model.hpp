#ifndef TIKPP_DATA_MODEL_HPP
#define TIKPP_DATA_MODEL_HPP

#include "tikpp/data/types/identity.hpp"
#include "tikpp/data/types/wrapper.hpp"
#include "tikpp/request.hpp"

#include <string>
#include <type_traits>

namespace tikpp::data {

struct model {
    template <typename T>
    using readonly = tikpp::data::types::readonly<T>;

    template <typename T>
    using one_way = tikpp::data::types::one_way<T>;

    template <typename T>
    using two_way = tikpp::data::types::two_way<T>;

    readonly<tikpp::data::types::identity> id;
    two_way<bool>                            is_disabled;

    template <template <typename> typename Converter, typename Map>
    inline void convert(Converter<Map> &c) {
        c[".id"] % id;
        c["disabled"] % is_disabled;
    }
};

} // namespace tikpp::data

#endif
