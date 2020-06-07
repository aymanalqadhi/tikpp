#include "tikpp/detail/type_traits/model.hpp"
#include "tikpp/models/dissolver.hpp"

#include "tikpp/tests/fakes/model.hpp"
#include "tikpp/tests/util/random.hpp"

#include "fmt/format.h"
#include "gtest/gtest.h"

#include <cstdint>
#include <string>
#include <unordered_map>

using map_type = std::unordered_map<std::string, std::string>;

static_assert(tikpp::detail::type_traits::is_hash_map_v<map_type>);

namespace tikpp::tests {

TEST(CreationModelsDissolverTests, DefaultValuesTest1) {
    tikpp::tests::fakes::model1 model {};
    map_type                    map {};

    tikpp::models::creation_dissolver<map_type> creator {map};
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

TEST(CreationModelsDissolverTests, DissolvingTest1) {
    tikpp::tests::fakes::model1 model {};
    map_type                    map {};

    model.prop1 = "test_string";
    model.prop2 = true;
    model.prop3 = 0xAB;
    model.prop4 = 0xABCD;
    model.prop5 = 0xABCDEF01U;
    model.prop6 = 0xABCDEF0123456789UL;

    tikpp::models::creation_dissolver<map_type> creator {map};
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

TEST(CreationModelsDissolverTests, DefaultValuesTest2) {
    tikpp::tests::fakes::model2 model {};
    map_type                    map {};

    tikpp::models::creation_dissolver<map_type> cd {map};
    model.convert(cd);

    EXPECT_EQ(map.find("=id"), map.end());
    EXPECT_EQ(map.find("=readonly-data"), map.end());
    EXPECT_NE(map.find("=one-way-data"), map.end());
    EXPECT_NE(map.find("=two-way-data"), map.end());

    EXPECT_TRUE(map["=one-way-data"].empty());
    EXPECT_TRUE(map["=two-way-data"].empty());

    map.clear();
    tikpp::models::updating_dissolver<map_type> ud {map};
    model.convert(ud);

    EXPECT_FALSE(model.two_way_data.changed());
    EXPECT_NE(map.find("=id"), map.end());
    EXPECT_NE(map.find("=readonly-data"), map.end());
    EXPECT_EQ(map.find("=one-way-data"), map.end());
    EXPECT_EQ(map.find("=two-way-data"), map.end());

    EXPECT_EQ(map["=id"], "*0");
    EXPECT_TRUE(map["=readonly-data"].empty());
    EXPECT_TRUE(map["=two-way-data"].empty());
}

TEST(CreationModelsDissolverTests, DissolvingTest2) {
    tikpp::tests::fakes::model2 model {};
    map_type                    map {};

    auto value1 = tikpp::tests::util::random<std::uint32_t>();
    auto str1   = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);
    auto str2 = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);
    auto str3 = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);

    model.id            = {value1};
    model.readonly_data = str1;
    model.one_way_data  = str2;
    model.two_way_data  = {str3};

    tikpp::models::creation_dissolver<map_type> cd {map};
    model.convert(cd);

    EXPECT_FALSE(model.two_way_data.changed());
    EXPECT_EQ(map.find("=id"), map.end());
    EXPECT_EQ(map.find("=readonly-data"), map.end());
    EXPECT_NE(map.find("=one-way-data"), map.end());
    EXPECT_NE(map.find("=two-way-data"), map.end());

    EXPECT_EQ(map["=one-way-data"], str2);
    EXPECT_EQ(map["=two-way-data"], str3);

    tikpp::models::updating_dissolver<map_type> ud {map};

    map.clear();
    model.convert(ud);

    EXPECT_FALSE(model.two_way_data.changed());

    EXPECT_NE(map.find("=id"), map.end());
    EXPECT_NE(map.find("=readonly-data"), map.end());
    EXPECT_EQ(map.find("=one-way-data"), map.end());
    EXPECT_EQ(map.find("=two-way-data"), map.end());

    EXPECT_EQ(map["=id"], fmt::format("*{:X}", value1));
    EXPECT_EQ(map["=readonly-data"], str1);

    model.two_way_data = {str3};
    EXPECT_TRUE(model.two_way_data.changed());

    map.clear();
    model.convert(ud);

    EXPECT_NE(map.find("=two-way-data"), map.end());
    EXPECT_EQ(map["=two-way-data"], str3);
}

} // namespace tikpp::tests
