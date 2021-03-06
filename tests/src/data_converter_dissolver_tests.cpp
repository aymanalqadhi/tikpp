#include "tikpp/data/converters/dissolver.hpp"
#include "tikpp/detail/type_traits/model.hpp"

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

    tikpp::data::converters::creation_dissolver<map_type> creator {map};
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

    tikpp::data::converters::creation_dissolver<map_type> creator {map};
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

    tikpp::data::converters::creation_dissolver<map_type> cd {map};
    model.convert(cd);

    EXPECT_EQ(map.find("=id"), map.end());
    EXPECT_EQ(map.find("=sticky-data"), map.end());
    EXPECT_NE(map.find("=read-only-data"), map.end());
    EXPECT_NE(map.find("=read-write-data"), map.end());

    EXPECT_TRUE(map["=read-only-data"].empty());
    EXPECT_TRUE(map["=read-write-data"].empty());

    map.clear();
    tikpp::data::converters::updating_dissolver<map_type> ud {map};
    model.convert(ud);

    EXPECT_FALSE(model.read_write_data.changed());
    EXPECT_NE(map.find("=id"), map.end());
    EXPECT_NE(map.find("=sticky-data"), map.end());
    EXPECT_EQ(map.find("=read-only-data"), map.end());
    EXPECT_EQ(map.find("=read-write-data"), map.end());

    EXPECT_EQ(map["=id"], "*0");
    EXPECT_TRUE(map["=sticky-data"].empty());
    EXPECT_TRUE(map["=read-write-data"].empty());
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

    model.id =
        tikpp::data::types::sticky<tikpp::data::types::identity> {value1};
    model.sticky_data     = tikpp::data::types::sticky {str1};
    model.read_only_data  = tikpp::data::types::read_only {str2};
    model.read_write_data = str3;

    tikpp::data::converters::creation_dissolver<map_type> cd {map};
    model.convert(cd);

    EXPECT_FALSE(model.read_write_data.changed());
    EXPECT_EQ(map.find("=id"), map.end());
    EXPECT_EQ(map.find("=sticky-data"), map.end());
    EXPECT_NE(map.find("=read-only-data"), map.end());
    EXPECT_NE(map.find("=read-write-data"), map.end());

    EXPECT_EQ(map["=read-only-data"], str2);
    EXPECT_EQ(map["=read-write-data"], str3);

    tikpp::data::converters::updating_dissolver<map_type> ud {map};

    map.clear();
    model.convert(ud);

    EXPECT_FALSE(model.read_write_data.changed());

    EXPECT_NE(map.find("=id"), map.end());
    EXPECT_NE(map.find("=sticky-data"), map.end());
    EXPECT_EQ(map.find("=read-only-data"), map.end());
    EXPECT_EQ(map.find("=read-write-data"), map.end());

    EXPECT_EQ(map["=id"], fmt::format("*{:X}", value1));
    EXPECT_EQ(map["=sticky-data"], str1);

    model.read_write_data = {str3};
    EXPECT_TRUE(model.read_write_data.changed());

    map.clear();
    model.convert(ud);

    EXPECT_NE(map.find("=read-write-data"), map.end());
    EXPECT_EQ(map["=read-write-data"], str3);
}

} // namespace tikpp::tests
