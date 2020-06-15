#ifndef TIKPP_COMMANDS_REMOVE_HPP
#define TIKPP_COMMANDS_REMOVE_HPP

#include "tikpp/request.hpp"
#include "tikpp/data/types/identity.hpp"

#include <cstdint>

namespace tikpp::commands {

template <typename Model>
struct remove : tikpp::request {
    remove(std::uint32_t tag, std::string id)
        : request {std::string {Model::api_path} + command_suffix, tag} {
        add_param(".id", id);
    }

    remove(std::uint32_t tag, tikpp::data::types::identity id)
        : request {std::string {Model::api_path} + command_suffix, tag} {
        add_param(".id", id.to_string());
    }

    static constexpr auto command_suffix = "/remove";
};

} // namespace tikpp::commands

#endif
