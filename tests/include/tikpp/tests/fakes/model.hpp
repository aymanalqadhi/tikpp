#ifndef TIKPP_TESTS_FAKES_MODEL_HPP
#define TIKPP_TESTS_FAKES_MODEL_HPP

#include "tikpp/data/model.hpp"
#include "tikpp/data/types/identity.hpp"
#include "tikpp/data/types/wrapper.hpp"

#include <cstdint>
#include <string>

namespace tikpp::tests::fakes {

struct model1 {
    static constexpr auto api_path = "/tikpp/tests/fakes/model1";

    std::string   prop1;
    bool          prop2;
    std::uint8_t  prop3;
    std::uint16_t prop4;
    std::uint32_t prop5;
    std::uint64_t prop6;

    template <typename Convertor>
    void convert(Convertor &c) {
        c["prop1"] % prop1;
        c["prop2"] % prop2;
        c["prop3"] % prop3;
        c["prop4"] % prop4;
        c["prop5"] % prop5;
        c["prop6"] % prop6;
    }
};

struct model2 : tikpp::data::model {
    static constexpr auto api_path = "/tikpp/tests/fakes/model2";

    sticky<std::string>     sticky_data;
    read_write<std::string> read_write_data;
    read_only<std::string>  read_only_data;

    template <typename Convertor>
    void convert(Convertor &c) {
        c["id"] % id;
        c["sticky-data"] % sticky_data;
        c["read-only-data"] % read_only_data;
        c["read-write-data"] % read_write_data;
    }
};

} // namespace tikpp::tests::fakes

#endif
