#ifndef TIKPP_DATA_IP_HOTSPOT_USER_HPP
#define TIKPP_DATA_IP_HOTSPOT_USER_HPP

#include "tikpp/data/model.hpp"
#include "tikpp/data/types/bytes.hpp"
#include "tikpp/data/types/duration.hpp"

#include <chrono>
#include <cstdint>
#include <string>

namespace tikpp::data::ip::hotspot {

struct user : tikpp::data::model {
    static constexpr auto api_path = "/ip/hotspot/user";

    read_write<std::string> name;
    read_write<bool>        is_disabled;
    read_write<std::string> password;
    read_write<std::string> profile;
    read_write<std::string> comment;

    template <typename Converter>
    inline void convert(Converter &c) {
        model::convert(c);

        c["name"] % name;
        c["disabled"] % is_disabled;
        c["password"] % password;
        c["profile"] % profile;
        c["comment"] % comment;
    }
};

struct user_detail : user {
    using bytes    = tikpp::data::types::bytes;
    using duration = tikpp::data::types::duration<std::chrono::seconds>;

    read_only<bytes>         bytes_in;
    read_only<bytes>         bytes_out;
    read_only<std::uint32_t> packets_in;
    read_only<std::uint32_t> packets_out;
    read_only<duration>      uptime;

    read_write<std::string> address;
    read_write<std::string> email;
    read_write<bytes>       limit_bytes_in;
    read_write<bytes>       limit_bytes_out;
    read_write<bytes>       limit_bytes_total;
    read_write<duration>    limit_uptime;
    read_write<std::string> mac_address;
    read_write<std::string> routes;
    read_write<std::string> server;

    template <typename Converter>
    inline void convert(Converter &c) {
        user::convert(c);

        c["bytes-in"] % bytes_in;
        c["bytes-out"] % bytes_out;
        c["packets-in"] % packets_in;
        c["packets-out"] % packets_out;
        c["uptime"] % uptime;

        c["address"] % address;
        c["email"] % email;
        c["limit-bytes-in"] % limit_bytes_in;
        c["limit-bytes-out"] % limit_bytes_out;
        c["limit-bytes-total"] % limit_bytes_total;
        c["limit-uptime"] % limit_uptime;
        c["mac-address"] % mac_address;
        c["routes"] % routes;
        c["server"] % server;
    }
};

} // namespace tikpp::data::ip::hotspot

#endif
