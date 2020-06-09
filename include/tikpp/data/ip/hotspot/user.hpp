#ifndef TIKPP_DATA_IP_HOTSPOT_USER_HPP
#define TIKPP_DATA_IP_HOTSPOT_USER_HPP

#include "tikpp/data/model.hpp"

#include "tikpp/data/types/bytes.hpp"
#include "tikpp/data/types/duration.hpp"
#include "tikpp/data/types/wrapper.hpp"

#include <chrono>
#include <cstdint>
#include <string>

namespace tikpp ::data::ip::hotspot {

struct user : tikpp::data::model {
    static constexpr auto api_path = "/ip/hotspot/user";

    two_way<std::string> name;
    two_way<std::string> password;
    two_way<std::string> profile;
    two_way<std::string> comment;

    template <typename Converter>
    inline void convert(Converter &c) {
        model::convert(c);

        c["name"] % name;
        c["password"] % password;
        c["profile"] % profile;
        c["comment"] % comment;
    }
};

struct user_detail : user {
    using bytes    = tikpp::data::types::bytes;
    using duration = tikpp::data::types::duration<std::chrono::seconds>;

    one_way<bytes>         bytes_in;
    one_way<bytes>         bytes_out;
    one_way<std::uint32_t> packets_in;
    one_way<std::uint32_t> packets_out;
    one_way<duration>      uptime;
    one_way<bool>          is_dynamic;

    two_way<bytes>    limit_bytes_in;
    two_way<bytes>    limit_bytes_out;
    two_way<bytes>    limit_bytes_total;
    two_way<duration> limit_uptime;

    two_way<std::string> address;
    two_way<std::string> mac_address;
    two_way<std::string> email;

    template <typename Converter>
    inline void convert(Converter &c) {
        user::convert(c);

        c["bytes-in"] % bytes_in;
        c["bytes-out"] % bytes_out;
        c["packets-in"] % packets_in;
        c["packets-out"] % packets_out;
        c["uptime"] % uptime;
        c["dynamic"] % is_dynamic;

        c["limit-bytes-in"] % limit_bytes_in;
        c["limit-bytes-out"] % limit_bytes_out;
        c["limit-bytes-total"] % limit_bytes_total;
        c["limit-uptime"] % limit_uptime;

        c["address"] % address;
        c["mac-address"] % mac_address;
        c["email"] % email;
    }
};

} // namespace tikpp::data::ip::hotspot

#endif
