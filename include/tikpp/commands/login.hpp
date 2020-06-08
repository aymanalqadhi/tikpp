#include "tikpp/detail/crypto.hpp"
#include "tikpp/request.hpp"

#include <cstdint>
#include <string>

namespace tikpp::commands {

namespace v1 {

struct login final : tikpp::request {
    login(std::uint32_t      tag,
          const std::string &name,
          const std::string &password,
          const std::string &cha)
        : request {command, tag} {
        add_param(name_param, name);
        add_param(
            password_param,
            fmt::format("00{}", tikpp::detail::hash_password(password, cha)));
    }

    static constexpr auto command         = "/login";
    static constexpr auto name_param      = "name";
    static constexpr auto password_param  = "response";
    static constexpr auto challenge_param = "ret";
    static constexpr auto challenge_size  = 32UL;
};

} // namespace v1

namespace v2 {

struct login : tikpp::request {
    login(std::uint32_t      tag,
          const std::string &name,
          const std::string &password)
        : request {command, tag} {
        add_param(name_param, name);
        add_param(password_param, password);
    }

    static constexpr auto command        = "/login";
    static constexpr auto name_param     = "name";
    static constexpr auto password_param = "password";
};

} // namespace v2

} // namespace tikpp::commands
