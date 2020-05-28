#include "tikpp/request.hpp"

#include <cstdint>
#include <string>

namespace tikpp::commands {

struct getall : tikpp::request {
    getall(std::uint32_t tag, const std::string &path)
        : request {path + command_suffix, tag} {
    }

    static constexpr auto command_suffix = "/getall";
};

} // namespace tikpp::commands
