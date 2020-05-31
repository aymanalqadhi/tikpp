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

    template <typename Converter>
    inline void convert(Converter &c) {
        c[".id"] % id;
    }
};

} // namespace tikpp::models

#endif
