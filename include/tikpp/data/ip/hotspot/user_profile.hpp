#ifndef TIKPP_DATA_IP_HOTSPOT_USER_PROFILE_HPP
#define TIKPP_DATA_IP_HOTSPOT_USER_PROFILE_HPP

#include "tikpp/data/model.hpp"
#include "tikpp/data/types/bytes.hpp"
#include "tikpp/data/types/duration.hpp"

#include <chrono>
#include <string>

namespace tikpp::data::ip::hotspot {

struct user_profile_detail : tikpp::data::model {
    static constexpr auto api_path = "/ip/hotspot/user/profile";

    using duration = tikpp::data::types::duration<std::chrono::seconds>;

    read_write<std::string>   address_list;
    read_write<std::string>   address_pool;
    read_write<std::string>   name;
    read_write<std::string>   rate_limit;
    read_write<std::uint32_t> shared_users;
    read_write<bool>          advertise;
    read_write<std::string>   advertise_interval;
    read_write<duration>      advertise_timeout;
    read_write<std::string>   advertise_url;
    read_write<duration>      idle_timeout;
    read_write<std::string>   incoming_filter;
    read_write<std::string>   incoming_packet_mark;
    read_write<duration>      keepalive_timeout;
    read_write<std::string>   on_login;
    read_write<std::string>   on_logout;
    read_write<std::string>   open_status_page;
    read_write<std::string>   outgoing_filter;
    read_write<std::string>   outgoing_packet_mark;
    read_write<duration>      session_timeout;
    read_write<duration>      status_autorefresh;
    read_write<bool>          transparent_proxy;

    template <typename Converter>
    inline void convert(Converter &c) {
        tikpp::data::model::convert(c);

        c("address-list", address_list);
        c("address-pool", address_pool);
        c("name", name);
        c("rate-limit", rate_limit);
        c("shared-users", "1", shared_users);
        c("advertise", "false", advertise);
        c("advertise-interval", "30m,10m", advertise_interval);
        c("advertise-timeout", "1m", advertise_timeout);
        c("advertise-url", advertise_url);
        c("idle-timeout", idle_timeout);
        c("incoming-filter", incoming_filter);
        c("incoming-packet-mark", incoming_packet_mark);
        c("keepalive-timeout", keepalive_timeout);
        c("on-login", on_login);
        c("on-logout", on_logout);
        c("open-status-page", "always", open_status_page);
        c("outgoing-filter", outgoing_filter);
        c("outgoing-packet-mark", outgoing_packet_mark);
        c("session-timeout", session_timeout);
        c("status-autorefresh", status_autorefresh);
        c("transparent-proxy", transparent_proxy);
    }
};

} // namespace tikpp::data::ip::hotspot

#endif
