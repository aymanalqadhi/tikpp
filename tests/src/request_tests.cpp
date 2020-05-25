#include "tikpp/request.hpp"

#include "gtest/gtest.h"

#include <cstdint>
#include <string>
#include <vector>

using namespace std::string_literals;

namespace {

constexpr auto test_command = "/test/command/path";
constexpr auto test_tag     = 1234;

} // namespace

namespace tikpp::tests {

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

    EXPECT_EQ(3, request.size());

    EXPECT_EQ(request["=key1"], "123");
    EXPECT_EQ(request["=key2"], "string");
    EXPECT_EQ(request["=key3"], "123.123");

    EXPECT_EQ(request.get<int>("=key1"), 123);
    EXPECT_EQ(request.get<std::string>("=key2"), "string");
    EXPECT_EQ(request.get<double>("=key3"), 123.123);
}

TEST_F(RequestTest, AddAttributeTest) {
    EXPECT_EQ(0, request.size());

    request.add_attribute("key1", 123);
    request.add_attribute("key2", "string"s);
    request.add_attribute("key3", 123.123);

    EXPECT_EQ(3, request.size());

    EXPECT_EQ(request[".key1"], "123");
    EXPECT_EQ(request[".key2"], "string");
    EXPECT_EQ(request[".key3"], "123.123");

    EXPECT_EQ(request.get<int>(".key1"), 123);
    EXPECT_EQ(request.get<std::string>(".key2"), "string");
    EXPECT_EQ(request.get<double>(".key3"), 123.123);
}

TEST_F(RequestTest, EncodeTest) {
    EXPECT_EQ(0, request.size());

    request.add_param("param", "value1");
    request.add_attribute("attr", "value2");

    EXPECT_EQ(2, request.size());

    EXPECT_EQ(request["=param"], "value1");
    EXPECT_EQ(request[".attr"], "value2");

    std::vector<std::uint8_t> buf {};
    std::vector<std::uint8_t> expected_buf {
        0x12, 0x2f, 0x74, 0x65, 0x73, 0x74, 0x2f, 0x63, 0x6f, 0x6d, 0x6d, 0x61,
        0x6e, 0x64, 0x2f, 0x70, 0x61, 0x74, 0x68, 0x09, 0x2e, 0x74, 0x61, 0x67,
        0x3d, 0x31, 0x32, 0x33, 0x34, 0x0c, 0x2e, 0x61, 0x74, 0x74, 0x72, 0x3d,
        0x76, 0x61, 0x6c, 0x75, 0x65, 0x32, 0x0d, 0x3d, 0x70, 0x61, 0x72, 0x61,
        0x6d, 0x3d, 0x76, 0x61, 0x6c, 0x75, 0x65, 0x31, 0x00};

    request.encode(buf);

    ASSERT_EQ(buf.size(), expected_buf.size());

    for (std::size_t i {0}; i < buf.size(); ++i) {
        EXPECT_EQ(buf[i], expected_buf[i]);
    }
}

} // namespace tikpp::tests
