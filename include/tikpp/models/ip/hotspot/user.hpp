#ifndef TIKPP_MODELS_IP_HOTSPOT_USER_HPP
#define TIKPP_MODELS_IP_HOTSPOT_USER_HPP

#include "tikpp/models/model.hpp"
#include "tikpp/models/types/bytes.hpp"
#include "tikpp/models/types/one_way.hpp"

#include <cstdint>
#include <string>

namespace tikpp::models::ip::hotspot {

struct user : tikpp::models::model {
    static constexpr auto api_path = "/ip/hotspot/user";

    std::string name;
    std::string password;
    std::string profile;

    bool disabled;

    template <typename Converter>
    inline void convert(Converter &c) {
        model::convert(c);

        c["name"] % name;
        c["password"] % password;
        c["profile"] % profile;
        c["disabled"] % disabled;
    }
};

struct user_detail : user {
    using bytes = tikpp::models::types::bytes;

    bytes       limit_bytes_in;
    bytes       limit_bytes_out;
    bytes       limit_bytes_total;
    std::string limit_uptime;

    tikpp::models::types::one_way<bytes>       bytes_in;
    tikpp::models::types::one_way<bytes>       bytes_out;
    tikpp::models::types::one_way<std::string> uptime;
    tikpp::models::types::one_way<bool>        is_dynamic;
    tikpp::models::types::one_way<bool>        is_default;

    template <template <typename> typename Converter, typename Map>
    inline void convert(Converter<Map> &c) {
        user::convert(c);

        c["limit-bytes-in"] % limit_bytes_in;
        c["limit-bytes-out"] % limit_bytes_out;
        c["limit-bytes-total"] % limit_bytes_total;
        c["limit-uptime"] % (limit_uptime = "2313283712391839s");

        c["bytes-in"] % bytes_in;
        c["bytes-out"] % bytes_out;
        c["uptime"] % uptime;
        c["default"] % is_default;
        c["dynamic"] % is_dynamic;
    }
};

} // namespace tikpp::models::ip::hotspot

#endif
