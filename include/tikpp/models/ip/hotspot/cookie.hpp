#ifndef TIKPP_MODELS_IP_HOTSPOT_COOKIE_HPP
#define TIKPP_MODELS_IP_HOTSPOT_COOKIE_HPP

#include "tikpp/data/model.hpp"
#include "tikpp/data/types/duration.hpp"

#include <string>

namespace tikpp::models::ip::hotspot {

/*!
 * \brief A data model to represent `/ip/hotspot/cookie' list
 */
struct cookie : tikpp::data::model {
    static constexpr auto api_path = "/ip/hotspot/cookie";

    using duration = tikpp::data::types::duration<std::chrono::seconds>;

    /*!
     * \brief Domain name (if split from username).
     */
    read_write<std::string> domain;

    /*!
     * \brief How long the cookie is valid.
     */
    read_write<std::string> expires_in;

    /*!
     * \brief Client's MAC-address.
     */
    read_write<std::string> mac_address;

    /*!
     * \brief Hostspot username.
     */
    read_write<std::string> user;

    template <typename Converter>
    void convert(Converter &c) {
        model::convert(c);

        c("domain", domain);
        c("expires-in", expires_in);
        c("mac-address", mac_address);
        c("user", user);
    }
};

} // namespace tikpp::models::ip::hotspot

#endif
