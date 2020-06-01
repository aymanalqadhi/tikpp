#ifndef TIKPP_TESTS_FAKES_MODEL_HPP
#define TIKPP_TESTS_FAKES_MODEL_HPP

#include <cstdint>
#include <string>

namespace tikpp::tests::fakes {

struct model {
    static constexpr auto api_path = "/tikpp/tests/fakes/model";

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

} // namespace tikpp::tests::fakes

#endif
