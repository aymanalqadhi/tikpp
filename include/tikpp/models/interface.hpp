#ifndef TIKPP_MODELS_INTERFACE_HPP
#define TIKPP_MODELS_INTERFACE_HPP

#include "tikpp/data/model.hpp"

#include <cstdint>
#include <string>

namespace tikpp::models {

/*!
 * \brief A data model to represent `/interface' list
 */
struct interface_model : tikpp::data::model {
    static constexpr auto api_path = "/interface";

    /*!
     * \brief Layer 2 Maximum transmission unit.
     *
     * Layer 2 Maximum transmission unit. Note that this property can not be
     * configured on all interfaces.
     */
    read_write<std::uint32_t> l2mtu;

    /*!
     * \brief Layer3 Maximum transmission unit
     */
    read_write<std::uint32_t> mtu;

    /*!
     * \brief Name of an interface
     */
    read_write<std::string> name;

    /*!
     * \brief Whether interface is dynamically created
     */
    read_only<bool> is_dynamic;

    /*!
     * \brief Fast path
     */
    read_only<bool> fast_path;

    /*!
     * \brief Interface id
     */
    read_only<std::uint32_t> if_id;

    /*!
     * \brief Interface index
     */
    read_only<std::uint32_t> if_index;

    /*!
     * \brief Interface name in Linux kernel
     */
    read_only<std::string> if_name;

    /*!
     * \brief Interface Mac-Address
     */
    read_only<std::string> mac_address;

    /*!
     * \brief Max supported L2MTU
     */
    read_only<std::uint32_t> max_l2mtu;

    /*!
     * \brief Whether interface is running.
     *
     * 	Whether interface is running. Note that some interfaces may not have a
     * 'running check' and they will always be reported as "running" (e.g. EoIP)
     */
    read_only<bool> is_running;

    /*!
     * \brief Number of received bytes.
     */
    read_only<std::uint32_t> rx_byte;

    /*!
     * \brief Number of received packets being dropped
     */
    read_only<std::uint32_t> rx_drop;

    /*!
     * \brief Packets received with some kind of an error.
     */
    read_only<std::uint32_t> rx_errors;

    /*!
     * \brief Number of packets received.
     */
    read_only<std::uint32_t> rx_packet;

    /*!
     * \brief Whether interface is configured as a slave of another interface.
     */
    read_only<bool> is_slave;

    /*!
     * \brief The interface status
     */
    read_only<std::string> status;

    /*!
     * \brief Number of transmitted bytes.
     */
    read_only<std::uint32_t> tx_byte;

    /*!
     * \brief Number of transmitted packets being dropped.
     */
    read_only<std::uint32_t> tx_drop;

    /*!
     * \brief Packets transmitted with some kind of an error.
     */
    read_only<std::uint32_t> tx_errors;

    /*!
     * \brief Number of transmitted packets.
     */
    read_only<std::uint32_t> tx_packet;

    template <typename Converter>
    void convert(Converter &c) {
        tikpp::data::model::convert(c);

        c("l2mtu", l2mtu, empty_string);
        c("mtu", mtu, empty_string);
        c("name", name, empty_string);
        c("dynamic", is_dynamic);
        c("fast-path", fast_path);
        c("id", if_id);
        c("ifindex", if_index);
        c("ifname", if_name);
        c("mac-address", mac_address);
        c("max-l2mtu", max_l2mtu);
        c("running", is_running);
        c("rx-byte", rx_byte);
        c("rx-drop", rx_drop);
        c("rx-errors", rx_errors);
        c("rx-packet", rx_packet);
        c("slave", is_slave);
        c("status", status);
        c("tx-byte", tx_byte);
        c("tx-drop", tx_drop);
        c("tx-errors", tx_errors);
        c("tx-packet", tx_packet);
    }
};

} // namespace tikpp::models

#endif
