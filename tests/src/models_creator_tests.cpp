#include "tikpp/detail/type_traits/model.hpp"
#include "tikpp/models/creator.hpp"

#include "tikpp/tests/fakes/model.hpp"
#include "tikpp/tests/util/random.hpp"

#include "gtest/gtest.h"

#include <cstdint>
#include <string>
#include <unordered_map>

using map_type = std::unordered_map<std::string, std::string>;

static_assert(tikpp::detail::type_traits::is_hash_map_v<map_type>);

namespace tikpp::tests {

TEST(ModelsCreatorTests, DefaultValuesTest1) {
    tikpp::tests::fakes::model1 model {};
    map_type                    map {};

    tikpp::models::creator<map_type> creator {map};
    model.convert(creator);

    EXPECT_EQ(model.prop1, "");
    EXPECT_EQ(model.prop2, false);
    EXPECT_EQ(model.prop3, 0);
    EXPECT_EQ(model.prop4, 0);
    EXPECT_EQ(model.prop5, 0);
    EXPECT_EQ(model.prop6, 0);
}

TEST(ModelsCreatorTests, CreationTest1) {
    tikpp::tests::fakes::model1 model {};
    map_type                    map {};

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

TEST(ModelsCreatorTests, DefaultValuesTest2) {
    tikpp::tests::fakes::model2 model {};
    map_type                    map {};

    tikpp::models::creator<map_type> creator {map};
    model.convert(creator);

    EXPECT_EQ(model.id.value().to_string(), "*0");
    EXPECT_EQ(model.readonly_data.value(), "");
    EXPECT_EQ(model.one_way_data.value(), "");
    EXPECT_EQ(model.two_way_data.value(), "");
}

TEST(ModelsCreatorTests, CreationTest2) {
    tikpp::tests::fakes::model2 model {};
    map_type                    map {};

    auto str1 = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);
    auto str2 = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);
    auto str3 = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);

    map["id"]            = "*ABCD";
    map["readonly-data"] = str1;
    map["one-way-data"]  = str2;
    map["two-way-data"]  = str3;

    tikpp::models::creator<map_type> creator {map};
    model.convert(creator);

    EXPECT_EQ(model.id.value().value(), 0xABCD);
    EXPECT_EQ(model.readonly_data.value(), str1);
    EXPECT_EQ(model.one_way_data.value(), str2);
    EXPECT_EQ(model.two_way_data.value(), str3);
}

} // namespace tikpp::tests
