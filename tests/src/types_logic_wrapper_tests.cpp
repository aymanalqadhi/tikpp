#include "tikpp/models/types/logic_wrapper.hpp"

#include "gtest/gtest.h"

namespace tikpp::tests {

struct test_wrapper : tikpp::models::types::logic_wrapper<int> {
    test_wrapper(int val) : value {val}, logic_wrapper<int> {value} {
    }

    int value;
};

TEST(LogicWrapperTypeTests, EqualityTest) {
    test_wrapper rhs {10}, lhs {20};

    EXPECT_FALSE(lhs == rhs);
    EXPECT_TRUE(lhs != rhs);
}

TEST(LogicWrapperTypeTests, LessThanTest) {
    test_wrapper lhs1 {10}, rhs1 {20};
    test_wrapper lhs2 {10}, rhs2 {10};

    EXPECT_TRUE(lhs1 < rhs1);
    EXPECT_TRUE(lhs1 <= rhs1);
    EXPECT_FALSE(rhs1 < lhs1);
    EXPECT_FALSE(rhs1 <= lhs1);
    EXPECT_FALSE(lhs2 < rhs2);
    EXPECT_TRUE(lhs2 <= rhs2);
}

TEST(LogicWrapperTypeTests, GreaterThanTest) {
    test_wrapper lhs1 {20}, rhs1 {10};
    test_wrapper lhs2 {10}, rhs2 {10};

    EXPECT_TRUE(lhs1 > rhs1);
    EXPECT_TRUE(lhs1 >= rhs1);
    EXPECT_FALSE(rhs1 > lhs1);
    EXPECT_FALSE(rhs1 >= lhs1);
    EXPECT_FALSE(lhs2 > rhs2);
    EXPECT_TRUE(lhs2 >= rhs2);
}

} // namespace tikpp::tests
