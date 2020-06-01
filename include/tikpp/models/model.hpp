#ifndef TIKPP_MODELS_MODEL_HPP
#define TIKPP_MODELS_MODEL_HPP

#include "tikpp/models/types/identity.hpp"
#include "tikpp/request.hpp"

#include <string>
#include <type_traits>

namespace tikpp::models {

struct model {
    auto id() const noexcept -> const tikpp::models::types::identity & {
        return id_;
    }

    template <typename Converter>
    inline void convert(Converter &c) {
        c[".id"] % id_;
    }

  private:
    tikpp::models::types::identity id_;
};

} // namespace tikpp::models

#endif
