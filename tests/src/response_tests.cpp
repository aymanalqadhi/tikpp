#include "tikpp/response.hpp"

#include "gtest/gtest.h"

#include <string>
#include <vector>

namespace tikpp::tests {

TEST(ResponseTest, NormalTypeTest) {
    tikpp::response resp {{"!done"}};

    EXPECT_EQ(resp.type(), tikpp::response_type::normal);
    EXPECT_EQ(resp.size(), 0);
    EXPECT_FALSE(resp.tag().has_value());
}

TEST(ResponseTest, DataTypeTest) {
    tikpp::response resp {{"!re"}};

    EXPECT_EQ(resp.type(), tikpp::response_type::data);
    EXPECT_EQ(resp.size(), 0);
    EXPECT_FALSE(resp.tag().has_value());
}

TEST(ResponseTest, TrapTypeTest) {
    tikpp::response resp {{"!trap"}};

    EXPECT_EQ(resp.type(), tikpp::response_type::trap);
    EXPECT_EQ(resp.size(), 0);
    EXPECT_FALSE(resp.tag().has_value());
}

TEST(ResponseTest, FatalTypeTest) {
    tikpp::response resp {{"!fatal"}};

    EXPECT_EQ(resp.type(), tikpp::response_type::fatal);
    EXPECT_EQ(resp.size(), 0);
    EXPECT_FALSE(resp.tag().has_value());
}

TEST(ResponseTest, UnknownTypeTest) {
    tikpp::response resp {{"!something_else"}};

    EXPECT_EQ(resp.type(), tikpp::response_type::unknown);
    EXPECT_EQ(resp.size(), 0);
    EXPECT_FALSE(resp.tag().has_value());
}

TEST(ResponseTest, ParamsTest) {
    tikpp::response resp {
        {"!re", "=key1=value1", "=key2=1234", "=key3=1234.1234", "=key4=1"}};

    EXPECT_EQ(resp.type(), tikpp::response_type::data);
    EXPECT_EQ(resp.size(), 4);
    EXPECT_FALSE(resp.tag().has_value());

    EXPECT_EQ(resp.get<std::string>("key1"), "value1");
    EXPECT_EQ(resp.get<std::uint32_t>("key2"), 1234);
    EXPECT_EQ(resp.get<double>("key3"), 1234.1234);
    EXPECT_EQ(resp.get<bool>("key4"), true);
}

TEST(ResponseTest, AttributesTest) {
    tikpp::response resp {{"!done", ".attr1=value1", ".attr2=1234",
                           ".attr3=1234.1234", ".attr4=1"}};

    EXPECT_EQ(resp.type(), tikpp::response_type::normal);
    EXPECT_EQ(resp.size(), 4);
    EXPECT_FALSE(resp.tag().has_value());

    EXPECT_EQ(resp.get<std::string>(".attr1"), "value1");
    EXPECT_EQ(resp.get<std::uint32_t>(".attr2"), 1234);
    EXPECT_EQ(resp.get<double>(".attr3"), 1234.1234);
    EXPECT_EQ(resp.get<bool>(".attr4"), true);
}

TEST(ResponseTest, TagTest) {
    tikpp::response resp {
        {"!re", "=key1=value1", "=key2=value2", ".attr1=123", ".tag=321"}};

    EXPECT_EQ(resp.type(), tikpp::response_type::data);
    EXPECT_EQ(resp.size(), 3);
    ASSERT_TRUE(resp.tag().has_value());
    EXPECT_EQ(resp.tag().value(), 321);
}

TEST(ResponseTest, EmptyValues) {
    tikpp::response resp {
        {"!re", "=key1=", "=key2=", ".attr1="}};

    EXPECT_EQ(resp.type(), tikpp::response_type::data);
    EXPECT_EQ(resp.size(), 3);
    ASSERT_FALSE(resp.tag().has_value());

    EXPECT_TRUE(resp.contains("key1"));
    EXPECT_TRUE(resp.contains("key2"));
    EXPECT_TRUE(resp.contains(".attr1"));

    EXPECT_TRUE(resp["key1"].empty());
    EXPECT_TRUE(resp["key2"].empty());
    EXPECT_TRUE(resp[".attr1"].empty());
}

} // namespace tikpp::tests
