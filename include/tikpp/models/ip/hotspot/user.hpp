#ifndef TIKPP_MODELS_IP_HOTSPOT_USER_HPP
#define TIKPP_MODELS_IP_HOTSPOT_USER_HPP

#include <string>

namespace tikpp::models::ip::hotspot {

struct user final {
    std::string name;
    std::string password;

    static constexpr auto api_path = "/ip/hotspot/user";

    template <typename Map>
    static inline auto create(const Map &m) -> user {
        assert(m.contains("name"));
        assert(m.contains("password"));

        return {.name = m["name"], .password = m["password"]};
    }
};

} // namespace tikpp::models::ip::hotspot

#endif
