#ifndef TIKPP_COMMANDS_REMOVE_HPP
#define TIKPP_COMMANDS_REMOVE_HPP

#include "tikpp/models/convert.hpp"
#include "tikpp/models/types/identity.hpp"
#include "tikpp/request.hpp"

#include <cstdint>

namespace tikpp::commands {

template <typename Model>
struct remove : tikpp::request {
    remove(std::uint32_t tag, std::string id)
        : request {std::string {Model::api_path} + command_suffix, tag} {
        add_param(".id", id);
    }

    static constexpr auto command_suffix = "/remove";
};

} // namespace tikpp::commands

#endif
