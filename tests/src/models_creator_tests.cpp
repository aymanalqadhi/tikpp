#include "tikpp/detail/type_traits/model.hpp"
#include "tikpp/models/converters.hpp"

#include "tikpp/tests/fakes/model.hpp"

#include "gtest/gtest.h"

#include <cstdint>
#include <string>
#include <unordered_map>

using model_type = tikpp::tests::fakes::model;
using map_type   = std::unordered_map<std::string, std::string>;

static_assert(tikpp::detail::type_traits::is_hash_map_v<map_type>);

namespace tikpp::tests {

TEST(ModelsCreatorTests, DefaultValuesTest) {
    model_type model {};
    map_type   map {};

    tikpp::models::creator<map_type> creator {map};
    model.convert(creator);

    EXPECT_EQ(model.prop1, "");
    EXPECT_EQ(model.prop2, false);
    EXPECT_EQ(model.prop3, 0);
    EXPECT_EQ(model.prop4, 0);
    EXPECT_EQ(model.prop5, 0);
    EXPECT_EQ(model.prop6, 0);
}

TEST(ModelsCreatorTests, CreationTest) {
    model_type model {};
    map_type   map {};

    map["prop1"] = "test_string";
    map["prop2"] = "yes";
    map["prop3"] = std::to_string(0xAB);
    map["prop4"] = std::to_string(0xABCDU);
    map["prop5"] = std::to_string(0xABCDEF01U);
    map["prop6"] = std::to_string(0xABCDEF0123456789UL);

    tikpp::models::creator<map_type> creator {map};
    model.convert(creator);

    EXPECT_EQ(model.prop1, "test_string");
    EXPECT_TRUE(model.prop2);
    EXPECT_EQ(model.prop3, 0xAB);
    EXPECT_EQ(model.prop4, 0xABCDU);
    EXPECT_EQ(model.prop5, 0xABCDEF01U);
    EXPECT_EQ(model.prop6, 0xABCDEF0123456789UL);
}

} // namespace tikpp::tests
