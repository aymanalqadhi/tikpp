#ifndef TIKPP_DATA_INTERFACE_HPP
#define TIKPP_DATA_INTERFACE_HPP

#include "tikpp/data/model.hpp"

#include <cstdint>
#include <string>

namespace tikpp::data {

struct interface_model : tikpp::data::model {
    static constexpr auto api_path = "/interface";

    read_write<std::string>   name;
    read_write<std::uint32_t> l2mtu;
    read_write<std::uint32_t> mtu;

    read_only<bool>          is_dynamic;
    read_only<bool>          fast_path;
    read_only<std::uint32_t> if_id;
    read_only<std::uint32_t> if_index;
    read_only<std::string>   if_name;
    read_only<std::string>   mac_address;
    read_only<std::uint32_t> max_l2mtu;
    read_only<bool>          is_running;
    read_only<std::uint32_t> rx_byte;
    read_only<std::uint32_t> rx_drop;
    read_only<std::uint32_t> rx_errors;
    read_only<std::uint32_t> rx_packet;
    read_only<bool>          is_slave;
    read_only<std::string>   status;
    read_only<std::uint32_t> tx_byte;
    read_only<std::uint32_t> tx_drop;
    read_only<std::uint32_t> tx_errors;
    read_only<std::uint32_t> tx_packet;

    template <typename Converter>
    inline void convert(Converter &c) {
        tikpp::data::model::convert(c);

        c("name", name);
        c("l2mtu", l2mtu);
        c("mtu", mtu);

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

} // namespace tikpp::data

#endif
