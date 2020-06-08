#ifndef TIKPP_TESTS_FAKES_MODEL_HPP
#define TIKPP_TESTS_FAKES_MODEL_HPP

#include "tikpp/models/types/identity.hpp"
#include "tikpp/models/types/wrapper.hpp"

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

struct model2 {
    static constexpr auto api_path = "/tikpp/tests/fakes/model2";

    tikpp::models::types::readonly<tikpp::models::types::identity> id;

    tikpp::models::types::two_way<std::string>  two_way_data;
    tikpp::models::types::one_way<std::string>  one_way_data;
    tikpp::models::types::readonly<std::string> readonly_data;

    template <typename Convertor>
    void convert(Convertor &c) {
        c["id"] % id;
        c["readonly-data"] % readonly_data;
        c["one-way-data"] % one_way_data;
        c["two-way-data"] % two_way_data;
    }
};

} // namespace tikpp::tests::fakes

#endif
