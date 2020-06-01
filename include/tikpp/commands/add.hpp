#ifndef TIKPP_COMMANDS_ADD_HPP
#define TIKPP_COMMANDS_ADD_HPP

#include "tikpp/models/converters.hpp"
#include "tikpp/request.hpp"

namespace tikpp::commands {

template <typename Model>
struct add : tikpp::request {
    add(std::uint32_t tag, Model model)
        : request {std::string {Model::api_path} + command_suffix, tag} {
        tikpp::models::dissolver<add<Model>> dissolver {*this};
        model.convert(dissolver);

        if (auto itr = words_.find("=.id"); itr != words_.end()) {
            words_.erase(itr);
        }
    }

    static constexpr auto command_suffix = "/add";
};

} // namespace tikpp::commands

#endif
