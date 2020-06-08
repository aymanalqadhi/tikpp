#ifndef TIKPP_COMMANDS_SET_HPP
#define TIKPP_COMMANDS_SET_HPP

#include "tikpp/data/converters/dissolver.hpp"
#include "tikpp/request.hpp"

#include <cstdint>

namespace tikpp::commands {

template <typename Model>
struct set : tikpp::request {
    set(std::uint32_t tag, Model model)
        : request {std::string {Model::api_path} + command_suffix, tag} {
        tikpp::data::converters::updating_dissolver<set<Model>> dissolver {
            *this};
        model.convert(dissolver);
    }

    static constexpr auto command_suffix = "/set";
};

} // namespace tikpp::commands

#endif
