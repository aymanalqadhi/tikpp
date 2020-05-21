#include "tikpp/request.hpp"

#include "gtest/gtest.h"

#include <string>

using namespace std::string_literals;

namespace {

constexpr auto test_command = "/test/command/path";
constexpr auto test_tag     = 1234;

} // namespace

struct RequestTest : ::testing::Test {
    RequestTest() : request {::test_command, ::test_tag} {
    }

    tikpp::request request;
};

TEST_F(RequestTest, AddWordTest) {
    EXPECT_EQ(0, request.size());

    request.add_word("raw_word1", "string_{}_{}", 1, 1.1);
    request.add_word("raw_word2", "string_{}_{}", 12, 12.12);
    request.add_word("raw_word3", "string_{}_{}", 123, 123.123);
    request.add_word("raw_word4", "string_{}_{}", 1234, 1234.1234);
    request.add_word("raw_word5", "string_{}_{}", 12345, 12345.12345);

    EXPECT_EQ(5, request.size());

    EXPECT_EQ(request["raw_word1"], "string_1_1.1");
    EXPECT_EQ(request["raw_word2"], "string_12_12.12");
    EXPECT_EQ(request["raw_word3"], "string_123_123.123");
    EXPECT_EQ(request["raw_word4"], "string_1234_1234.1234");
    EXPECT_EQ(request["raw_word5"], "string_12345_12345.12345");
}

TEST_F(RequestTest, AddParamTest) {
    EXPECT_EQ(0, request.size());

    request.add_param("key1", 123);
    request.add_param("key2", "string"s);
    request.add_param("key3", 123.123);
    request.add_param("key4", true);

    EXPECT_EQ(4, request.size());

    EXPECT_EQ(request["=key1"], "123");
    EXPECT_EQ(request["=key2"], "string");
    EXPECT_EQ(request["=key3"], "123.123");
    EXPECT_EQ(request["=key4"], "true");
}

TEST_F(RequestTest, AddAttributeTest) {
    EXPECT_EQ(0, request.size());

    request.add_attribute("key1", 123);
    request.add_attribute("key2", "string"s);
    request.add_attribute("key3", 123.123);
    request.add_attribute("key4", true);

    EXPECT_EQ(4, request.size());

    EXPECT_EQ(request[".key1"], "123");
    EXPECT_EQ(request[".key2"], "string");
    EXPECT_EQ(request[".key3"], "123.123");
    EXPECT_EQ(request[".key4"], "true");
}
