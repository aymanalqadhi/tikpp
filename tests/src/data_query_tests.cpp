#include "tikpp/data/query.hpp"

#include "fmt/format.h"
#include "gtest/gtest.h"

#include <string>
#include <vector>

using namespace tikpp::data::literals;

namespace {

constexpr auto test_rhs = 12345;

}

namespace tikpp::tests {

struct QueryTokenOperatorTest : testing::Test {
    tikpp::data::query_token token {"test_token"};
};

TEST(QueryTests, LiteralsTest) {
    auto [gtoken1, gtoken2, gtoken3] =
        tikpp::data::make_tokens("token1", "token2", "token3");
    auto ltoken1 = "token1"_t, ltoken2 = "token2"_t, ltoken3 = "token3"_t;

    EXPECT_EQ(gtoken1.name, ltoken1.name);
    EXPECT_EQ(gtoken2.name, ltoken2.name);
    EXPECT_EQ(gtoken3.name, ltoken3.name);
}

TEST_F(QueryTokenOperatorTest, TokenNotOperatorTest) {
    auto not_query = !token;

    ASSERT_EQ(not_query.words.size(), 1);
    EXPECT_EQ(not_query.words[0], "?-" + token.name);
}

TEST_F(QueryTokenOperatorTest, TokenEqualsOperatorTest) {
    auto equals_query = token == ::test_rhs;

    ASSERT_EQ(equals_query.words.size(), 1);
    EXPECT_EQ(equals_query.words[0],
              fmt::format("?={}={}", token.name, ::test_rhs));
}

TEST_F(QueryTokenOperatorTest, TokenNotEqualsOperatorTest) {
    auto equals_query = token != ::test_rhs;

    ASSERT_EQ(equals_query.words.size(), 2);
    EXPECT_EQ(equals_query.words[0],
              fmt::format("?={}={}", token.name, ::test_rhs));
    EXPECT_EQ(equals_query.words[1], "?#!");
}

TEST_F(QueryTokenOperatorTest, TokenLessThanOperatorTest) {
    auto equals_query = token < ::test_rhs;

    ASSERT_EQ(equals_query.words.size(), 1);
    EXPECT_EQ(equals_query.words[0],
              fmt::format("?<{}={}", token.name, ::test_rhs));
}

TEST_F(QueryTokenOperatorTest, TokenGreaterThanOperatorTest) {
    auto equals_query = token > ::test_rhs;

    ASSERT_EQ(equals_query.words.size(), 1);
    EXPECT_EQ(equals_query.words[0],
              fmt::format("?>{}={}", token.name, ::test_rhs));
}

TEST_F(QueryTokenOperatorTest, TokenLessThanOrEqualsOperatorTest) {
    auto equals_query = token <= ::test_rhs;

    ASSERT_EQ(equals_query.words.size(), 3);
    EXPECT_EQ(equals_query.words[0],
              fmt::format("?<{}={}", token.name, ::test_rhs));
    EXPECT_EQ(equals_query.words[1],
              fmt::format("?={}={}", token.name, ::test_rhs));
    EXPECT_EQ(equals_query.words[2], "?#|");
}

TEST_F(QueryTokenOperatorTest, TokenGreaterThanOrEqualsOperatorTest) {
    auto equals_query = token >= ::test_rhs;

    ASSERT_EQ(equals_query.words.size(), 3);
    EXPECT_EQ(equals_query.words[0],
              fmt::format("?>{}={}", token.name, ::test_rhs));
    EXPECT_EQ(equals_query.words[1],
              fmt::format("?={}={}", token.name, ::test_rhs));
    EXPECT_EQ(equals_query.words[2], "?#|");
}

TEST(QueryLogicalOperators, LogicalAndTest) {
    auto and_query = "token1"_t == ::test_rhs && "token2"_t != ::test_rhs;

    ASSERT_EQ(and_query.words.size(), 4);
    EXPECT_EQ(and_query.words[0], fmt::format("?=token1={}", ::test_rhs));
    EXPECT_EQ(and_query.words[1], fmt::format("?=token2={}", ::test_rhs));
    EXPECT_EQ(and_query.words[2], "?#!");
    EXPECT_EQ(and_query.words[3], "?#&");
}

TEST(QueryLogicalOperators, LogicalOrTest) {
    auto or_query = "token1"_t == ::test_rhs || "token2"_t != ::test_rhs;

    ASSERT_EQ(or_query.words.size(), 4);
    EXPECT_EQ(or_query.words[0], fmt::format("?=token1={}", ::test_rhs));
    EXPECT_EQ(or_query.words[1], fmt::format("?=token2={}", ::test_rhs));
    EXPECT_EQ(or_query.words[2], "?#!");
    EXPECT_EQ(or_query.words[3], "?#|");
}

TEST(QueryLogicalOperators, LogicalXorTest) {
    auto [t1, t2] = tikpp::data::make_tokens("token1", "token2");

    auto xor_query =
        (t1 == ::test_rhs) ^ (t2 != ::test_rhs && t1 <= ::test_rhs);

    ASSERT_EQ(xor_query.words.size(), 19);

    EXPECT_EQ(xor_query.words[0], fmt::format("?={}={}", t1.name, ::test_rhs));
    EXPECT_EQ(xor_query.words[1], fmt::format("?={}={}", t2.name, ::test_rhs));
    EXPECT_EQ(xor_query.words[2], "?#!");
    EXPECT_EQ(xor_query.words[3], fmt::format("?<{}={}", t1.name, ::test_rhs));
    EXPECT_EQ(xor_query.words[4], fmt::format("?={}={}", t1.name, ::test_rhs));
    EXPECT_EQ(xor_query.words[5], "?#|");
    EXPECT_EQ(xor_query.words[6], "?#&");
    EXPECT_EQ(xor_query.words[7], "?#!");
    EXPECT_EQ(xor_query.words[8], "?#&");
    EXPECT_EQ(xor_query.words[9], fmt::format("?={}={}", t1.name, ::test_rhs));
    EXPECT_EQ(xor_query.words[10], "?#!");
    EXPECT_EQ(xor_query.words[11], fmt::format("?={}={}", t2.name, ::test_rhs));
    EXPECT_EQ(xor_query.words[12], "?#!");
    EXPECT_EQ(xor_query.words[13], fmt::format("?<{}={}", t1.name, ::test_rhs));
    EXPECT_EQ(xor_query.words[14], fmt::format("?={}={}", t1.name, ::test_rhs));
    EXPECT_EQ(xor_query.words[15], "?#|");
    EXPECT_EQ(xor_query.words[16], "?#&");
    EXPECT_EQ(xor_query.words[17], "?#&");
    EXPECT_EQ(xor_query.words[18], "?#|");
}

} // namespace tikpp::tests
