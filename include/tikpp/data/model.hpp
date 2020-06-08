#ifndef TIKPP_DATA_MODEL_HPP
#define TIKPP_DATA_MODEL_HPP

#include "tikpp/models/types/identity.hpp"
#include "tikpp/models/types/wrapper.hpp"
#include "tikpp/request.hpp"

#include <string>
#include <type_traits>

namespace tikpp::data {

struct model {
    template <typename T>
    using readonly = tikpp::models::types::readonly<T>;

    template <typename T>
    using one_way = tikpp::models::types::one_way<T>;

    template <typename T>
    using two_way = tikpp::models::types::two_way<T>;

    readonly<tikpp::models::types::identity> id;
    two_way<bool>                            is_disabled;

    template <template <typename> typename Converter, typename Map>
    inline void convert(Converter<Map> &c) {
        c[".id"] % id;
        c["disabled"] % is_disabled;
    }
};

} // namespace tikpp::data

#endif
