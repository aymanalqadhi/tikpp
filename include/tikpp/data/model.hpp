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
    using sticky = tikpp::data::types::sticky<T>;

    template <typename T>
    using read_only = tikpp::data::types::read_only<T>;

    template <typename T>
    using read_write = tikpp::data::types::read_write<T>;

    sticky<tikpp::data::types::identity> id;
    read_write<bool>                     is_disabled;

    template <template <typename> typename Converter, typename Map>
    inline void convert(Converter<Map> &c) {
        c[".id"] % id;
        c["disabled"] % is_disabled;
    }
};

} // namespace tikpp::data

#endif
