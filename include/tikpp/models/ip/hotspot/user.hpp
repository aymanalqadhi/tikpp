#ifndef TIKPP_MODELS_IP_HOTSPOT_USER_HPP
#define TIKPP_MODELS_IP_HOTSPOT_USER_HPP

#include "tikpp/models/model.hpp"
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
    tikpp::models::types::one_way<std::uint64_t> bytes_in;
    tikpp::models::types::one_way<std::uint64_t> bytes_out;

    std::uint64_t limit_bytes_in;
    std::uint64_t limit_bytes_out;

    template <template <typename> typename Converter, typename Map>
    inline void convert(Converter<Map> &c) {
        user::convert(c);

        c["bytes-in"] % bytes_in;
        c["bytes-out"] % bytes_out;
        c["limit-bytes-in"] % limit_bytes_in;
        c["limit-bytes-out"] % limit_bytes_out;
    }
};

} // namespace tikpp::models::ip::hotspot

#endif
