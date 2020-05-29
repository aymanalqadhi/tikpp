#ifndef TIKPP_MODELS_IP_HOTSPOT_USER_HPP
#define TIKPP_MODELS_IP_HOTSPOT_USER_HPP

#include "tikpp/detail/type_traits/model.hpp"

#include <string>

namespace tikpp::models::ip::hotspot {

struct user final {
    static constexpr auto api_path = "/ip/hotspot/user";

    std::string name;
    std::string password;
    std::string profile;

    bool disabled;

    template <typename Converter>
    inline void convert(Converter &c) {
        c["name"] % name;
        c["password"] % password;
        c["profile"] % profile;
        c["disabled"] % disabled;
    }
};

} // namespace tikpp::models::ip::hotspot

#endif
