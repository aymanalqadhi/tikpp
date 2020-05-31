#ifndef TIKPP_MODELS_MODEL_HPP
#define TIKPP_MODELS_MODEL_HPP

#include "tikpp/models/convert.hpp"
#include "tikpp/models/types/identity.hpp"
#include "tikpp/request.hpp"

#include <string>
#include <type_traits>

namespace tikpp::models {

struct model {
    tikpp::models::types::identity id;

    template <template <typename> typename Converter, typename Map>
    inline void convert(Converter<Map> &c) {
        if constexpr (!std::is_same_v<Converter<Map>,
                                      tikpp::models::dissolver<Map>>) {
            c[".id"] % id;
        }
    }
};

} // namespace tikpp::models

#endif
