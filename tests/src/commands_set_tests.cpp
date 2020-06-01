#include "tikpp/commands/set.hpp"
#include "tikpp/tests/fakes/model.hpp"

#include "gtest/gtest.h"

#include <string>
#include <unordered_map>

using model_type = tikpp::tests::fakes::model;

using namespace std::literals;

namespace tikpp::tests {

TEST(AddCommandTests, CommandWordsTest) {
    constexpr auto test_tag = 0xABCDEF;

    model_type model {};
    model.prop1 = "test_string";
    model.prop2 = true;
    model.prop3 = 0xAB;
    model.prop4 = 0xABCD;
    model.prop5 = 0xABCDEF01U;
    model.prop6 = 0xABCDEF0123456790LU;

    tikpp::commands::set<model_type> set_cmd {test_tag, model};

    ASSERT_EQ(tikpp::commands::set<model_type>::command_suffix, "/set");
    EXPECT_EQ(set_cmd.command(), model_type::api_path + "/set"s);
    EXPECT_EQ(set_cmd.size(), 6);

    EXPECT_TRUE(set_cmd.contains("=prop1"));
    EXPECT_TRUE(set_cmd.contains("=prop2"));
    EXPECT_TRUE(set_cmd.contains("=prop3"));
    EXPECT_TRUE(set_cmd.contains("=prop4"));
    EXPECT_TRUE(set_cmd.contains("=prop5"));
    EXPECT_TRUE(set_cmd.contains("=prop6"));
    EXPECT_FALSE(set_cmd.contains("=prop7"));
    EXPECT_FALSE(set_cmd.contains("=prop8"));

    EXPECT_EQ(set_cmd.get<std::string>("=prop1"), model.prop1);
    EXPECT_EQ(set_cmd.get<bool>("=prop2"), model.prop2);
    EXPECT_EQ(set_cmd.get<std::uint8_t>("=prop3"), model.prop3);
    EXPECT_EQ(set_cmd.get<std::uint16_t>("=prop4"), model.prop4);
    EXPECT_EQ(set_cmd.get<std::uint32_t>("=prop5"), model.prop5);
    EXPECT_EQ(set_cmd.get<std::uint64_t>("=prop6"), model.prop6);
}

} // namespace tikpp::tests
