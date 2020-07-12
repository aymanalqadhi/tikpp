#ifndef TIKPP_MODELS_IP_ARP_HPP
#define TIKPP_MODELS_IP_ARP_HPP

#include "tikpp/data/model.hpp"

#include <string>

namespace tikpp::models::ip {

/*!
 * \brief A data model to represent `/ip/arp' list
 */
struct arp : tikpp::data::model {
    static constexpr auto api_path = "/ip/arp";

    /*!
     * \brief IP address to be mapped
     */
    read_write<std::string> address;

    /*!
     * \brief Interface name the IP address is assigned to
     */
    read_write<std::string> interface;

    /*!
     * \brief MAC address to be mapped to
     */
    read_write<std::string> mac_address;

    /*!
     * \brief Static proxy-arp entry for individual IP address.
     *
     * 	Static proxy-arp entry for individual IP address. When an ARP query is
     * received for the specific IP address, the device will respond with its
     * own MAC address. No need to set proxy-arp on the interface itself for all
     * the MAC addresses to be proxied. The interface will respond to an ARP
     * request only when the device has an active route towards the destination
     */
    read_write<bool> published;

    /*!
     * \brief Whether ARP entry is added by DHCP server
     */
    read_only<bool> is_dhcp;

    /*!
     * \brief Whether entry is dynamically created
     */
    read_only<bool> is_dynamic;

    /*!
     * \brief Whether entry is not valid
     */
    read_only<bool> is_invalid;

    template <typename Converter>
    void convert(Converter &c) {
        tikpp::data::model::convert(c);

        c("address", address, empty_string);
        c("interface", interface, empty_string);
        c("mac-address", mac_address, "00:00:00:00:00:00");
        c("published", published, false);
        c("dhcp", is_dhcp);
        c("dynamic", is_dynamic);
        c("invalid", is_invalid);
    }
};

} // namespace tikpp::models::ip

#endif
