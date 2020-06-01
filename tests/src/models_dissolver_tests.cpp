#include "tikpp/detail/type_traits/model.hpp"
#include "tikpp/models/converters.hpp"

#include "tikpp/tests/fakes/model.hpp"

#include "gtest/gtest.h"

#include <iostream>

#include <cstdint>
#include <string>
#include <unordered_map>

using model_type = tikpp::tests::fakes::model;
using map_type   = std::unordered_map<std::string, std::string>;

static_assert(tikpp::detail::type_traits::is_hash_map_v<map_type>);

namespace tikpp::tests {

TEST(ModelsDissolverTests, DefaultValuesTest) {
    model_type model {};
    map_type   map {};

    tikpp::models::dissolver<map_type> creator {map};
    model.convert(creator);

    EXPECT_TRUE(map.find("=prop1") != map.end());
    EXPECT_TRUE(map.find("=prop2") != map.end());
    EXPECT_TRUE(map.find("=prop3") != map.end());
    EXPECT_TRUE(map.find("=prop4") != map.end());
    EXPECT_TRUE(map.find("=prop5") != map.end());
    EXPECT_TRUE(map.find("=prop6") != map.end());

    EXPECT_TRUE(map["=prop1"].empty());
    EXPECT_EQ(map["=prop2"], "false");
    EXPECT_EQ(map["=prop3"], "0");
    EXPECT_EQ(map["=prop4"], "0");
    EXPECT_EQ(map["=prop5"], "0");
    EXPECT_EQ(map["=prop6"], "0");
}

TEST(ModelsDissolverTests, DissolvingTest) {
    model_type model {};
    map_type   map {};

    model.prop1 = "test_string";
    model.prop2 = true;
    model.prop3 = 0xAB;
    model.prop4 = 0xABCD;
    model.prop5 = 0xABCDEF01U;
    model.prop6 = 0xABCDEF0123456789UL;

    tikpp::models::dissolver<map_type> creator {map};
    model.convert(creator);

    EXPECT_TRUE(map.find("=prop1") != map.end());
    EXPECT_TRUE(map.find("=prop2") != map.end());
    EXPECT_TRUE(map.find("=prop3") != map.end());
    EXPECT_TRUE(map.find("=prop4") != map.end());
    EXPECT_TRUE(map.find("=prop5") != map.end());
    EXPECT_TRUE(map.find("=prop6") != map.end());

    EXPECT_EQ(map["=prop1"], "test_string");
    EXPECT_EQ(map["=prop2"], "true");
    EXPECT_EQ(map["=prop3"], std::to_string(0xAB));
    EXPECT_EQ(map["=prop4"], std::to_string(0xABCDU));
    EXPECT_EQ(map["=prop5"], std::to_string(0xABCDEF01U));
    EXPECT_EQ(map["=prop6"], std::to_string(0xABCDEF0123456789UL));
}

} // namespace tikpp::tests
