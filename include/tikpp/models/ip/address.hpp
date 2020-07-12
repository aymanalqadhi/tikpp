#ifndef TIKPP_MODELS_IP_ADDRESS_HPP
#define TIKPP_MODELS_IP_ADDRESS_HPP

#include "tikpp/data/model.hpp"

#include <cstdint>
#include <string>

namespace tikpp::models::ip {

/*!
 * \brief A data model to represent `/ip/address' list
 */
struct address : tikpp::data::model {
    static constexpr auto api_path = "/ip/address";

    /*!
     * \brief IP Address.
     */
    read_write<std::string> address;

    /*!
     * \brief Broadcasting IP address.
     *
     * Broadcasting IP address, calculated by default from an IP address and a
     * network mask. Starting from v5RC6 this parameter is removed
     */
    read_write<std::string> broadcast;

    /*!
     * \brief Interface name the IP address is assigned to.
     */
    read_write<std::string> interface;

    /*!
     * \brief Delimits network address part of the IP address from the host
     *        part.
     */
    read_write<std::string> netmask;

    /*!
     * \brief IP address for the network.
     *
     * IP address for the network. For point-to-point links it should be the
     * address of the remote end. Starting from v5RC6 this parameter is
     * configurable only for addresses with /32 netmask (point to point links)
     */
    read_write<std::string> network;

    /*!
     * \brief Name of the actual interface the logical one is bound to.
     *
     * 	Name of the actual interface the logical one is bound to. For example,
     * if the physical interface you assigned the address to, is included in a
     * bridge, the actual interface will show that bridge
     */
    read_only<std::string> actual_interface;

    template <typename Converter>
    void convert(Converter &c) {
        tikpp::data::model::convert(c);

        c("address", address, empty_string);
        c("broadcast", broadcast, "255.255.255.255");
        c("interface", interface, empty_string);
        c("netmask", netmask, "0.0.0.0");
        c("network", network, "0.0.0.0");
    }
};

} // namespace tikpp::models::ip

#endif
