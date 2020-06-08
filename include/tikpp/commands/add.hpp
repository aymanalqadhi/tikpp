#ifndef TIKPP_COMMANDS_ADD_HPP
#define TIKPP_COMMANDS_ADD_HPP

#include "tikpp/data/converters/dissolver.hpp"
#include "tikpp/request.hpp"

#include <cstdint>

namespace tikpp::commands {

template <typename Model>
struct add : tikpp::request {
    add(std::uint32_t tag, Model model)
        : request {std::string {Model::api_path} + command_suffix, tag} {
        tikpp::data::converters::creation_dissolver<add<Model>> dissolver {
            *this};
        model.convert(dissolver);
    }

    static constexpr auto command_suffix = "/add";
};

} // namespace tikpp::commands

#endif
