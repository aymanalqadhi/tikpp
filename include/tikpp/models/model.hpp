#ifndef TIKPP_MODELS_MODEL_HPP
#define TIKPP_MODELS_MODEL_HPP

#include "tikpp/models/converters.hpp"
#include "tikpp/models/types/identity.hpp"
#include "tikpp/models/types/readonly.hpp"
#include "tikpp/request.hpp"

#include <string>
#include <type_traits>

namespace tikpp::models {

struct model {
    tikpp::models::types::readonly<tikpp::models::types::identity> id;

    template <template <typename> typename Converter, typename Map>
    inline void convert(Converter<Map> &c) {
        c[".id"] % id;
    }
};

} // namespace tikpp::models

#endif
