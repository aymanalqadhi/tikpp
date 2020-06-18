#ifndef TIKPP_MODELS_IP_HOTSPOT_IP_BINDING_HPP
#define TIKPP_MODELS_IP_HOTSPOT_IP_BINDING_HPP

#include "tikpp/data/model.hpp"

#include <string>

namespace tikpp::models::ip::hotspot {

/*!
 * \brief A data model to represent `/ip/hotspot/ip-binding' list
 */
struct ip_binding : tikpp::data::model {
    static constexpr auto api_path = "/ip/hotspot/ip-binding";

    /*!
     * \brief The original IP address of the client.
     */
    read_write<std::string> address;

    /*!
     * \brief MAC address of the client.
     */
    read_write<std::string> mac_address;

    /*!
     * \brief Name of the HotSpot server.
     *
     * Name of the HotSpot server.
     * - all: will be applied to all hotspot servers
     */
    read_write<std::string> server;

    /*!
     * \brief New IP address of the client.
     *
     * New IP address of the client, translation occurs on the router (client
     * does not know anything about the translation)
     */
    read_write<std::string> to_address;

    /*!
     * \brief Type of the IP-binding action.
     *
     * Type of the IP-binding action
     * - regular: performs One-to-One NAT according to the rule, translates
     *            address to \see to_address
     * - bypassed: performs the translation, but excludes client from login
     *             to the HotSpot
     * - blocked: translation is not performed and packets from host are
     *            dropped
     */
    read_write<std::string> type;

    template <typename Converter>
    void convert(Converter &c) {
        model::convert(c);

        c("address", address, empty_string);
        c("mac-address", mac_address, empty_string);
        c("server", server, "all");
        c("to-address", to_address, empty_string);
        c("type", type, empty_string);
    }
};

} // namespace tikpp::models::ip::hotspot

#endif
