#ifndef TIKPP_DATA_IP_HOTSPOT_HPP
#define TIKPP_DATA_IP_HOTSPOT_HPP

#include "tikpp/data/model.hpp"
#include "tikpp/data/types/duration.hpp"

#include <chrono>
#include <cstdint>
#include <string>

namespace tikpp::data::ip {

struct hotspot_model : tikpp::data::model {
    static constexpr auto api_path = "/ip/hotspot";

    using duration = tikpp::data::types::duration<std::chrono::seconds>;

    read_write<std::string>   name;
    read_write<std::string>   address_pool;
    read_write<duration>      idle_timeout;
    read_write<duration>      keepalive_timeout;
    read_write<duration>      login_timeout;
    read_write<std::string>   interface;
    read_write<std::uint32_t> addresses_per_mac;
    read_write<std::string>   profile;

    template <typename Converter>
    inline void convert(Converter &c) {
        tikpp::data::model::convert(c);

        c("name") % name;
        c("address-pool") % address_pool;
        c("idle-timeout") % idle_timeout;
        c("keepalive-timeout") % keepalive_timeout;
        c("login-timeout") % login_timeout;
        c("interface") % interface;
        c("addresses-per-mac") % addresses_per_mac;
        c("profile") % profile;
    }
};

} // namespace tikpp::data::ip

#endif
