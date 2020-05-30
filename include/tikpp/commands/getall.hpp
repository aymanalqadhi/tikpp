#include "tikpp/models/convert.hpp"
#include "tikpp/request.hpp"

#include "fmt/format.h"
#include <boost/algorithm/string.hpp>

#include <cstdint>
#include <string>

namespace tikpp::commands {

template <typename Model>
struct getall : tikpp::request {
    getall(std::uint32_t tag)
        : request {std::string {Model::api_path} + command_suffix, tag} {
        tikpp::models::proplist_collector collector {};

        Model m {};
        m.convert(collector);

        auto proplist = boost::join(collector.proplist, ",");
        add_param(".proplist", std::move(proplist));
    }

    getall(std::uint32_t tag, std::vector<std::string> q) : getall(tag) {
        query(std::move(q));
    }

    static constexpr auto command_suffix = "/getall";
};

} // namespace tikpp::commands
