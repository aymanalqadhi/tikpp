#ifndef TIKPP_DATA_MODEL_HPP
#define TIKPP_DATA_MODEL_HPP

#include "tikpp/data/types/identity.hpp"
#include "tikpp/data/types/wrapper.hpp"
#include "tikpp/request.hpp"

#include <string>
#include <type_traits>

namespace tikpp::data {

struct model {
    /*!
     * \brief A constant that contains an empty string literal
     */
    static constexpr auto empty_string = "";

    template <typename T>
    using sticky = tikpp::data::types::sticky<T>;

    template <typename T>
    using read_only = tikpp::data::types::read_only<T>;

    template <typename T>
    using read_write = tikpp::data::types::read_write<T>;

    /*!
     * \brief The item API ID
     */
    sticky<tikpp::data::types::identity> id;

    template <typename Converter>
    inline void convert(Converter &c) {
        c(".id", id);
    }
};

struct disableable_model : model {
    /*!
     * \brief Whether the model is currently disabled or not.
     */
    read_write<bool> is_disabled;

    template <typename Converter>
    inline void convert(Converter &c) {
        model::convert(c);

        c("disabled", is_disabled);
    }
};

} // namespace tikpp::data

#endif
