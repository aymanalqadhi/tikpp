#include "tikpp/data/types/wrapper.hpp"
#include "tikpp/tests/util/random.hpp"

#include "gtest/gtest.h"

using test_type = unsigned long;

namespace tikpp::tests {

using test_wrapper = tikpp::data::types::read_only<test_type>;

TEST(OneWayBinderTypeTests, ArithmeticTest) {
    auto rnd1 = tikpp::tests::util::random<test_type>();
    auto rnd2 = tikpp::tests::util::random<test_type>();

    test_wrapper w1 {rnd1}, w2 {rnd2};

    EXPECT_EQ(static_cast<test_type>(w1 + w2), rnd1 + rnd2);
    EXPECT_EQ(static_cast<test_type>(w1 - w2), rnd1 - rnd2);
    EXPECT_EQ(static_cast<test_type>(w1 * w2), rnd1 * rnd2);
    EXPECT_EQ(static_cast<test_type>(w1 / w2), rnd1 / rnd2);
    EXPECT_EQ(static_cast<test_type>(w1 % w2), rnd1 % rnd2);
}

TEST(OneWayBinderTypeTests, LogicTest) {
    auto rnd1 = tikpp::tests::util::random<test_type>();
    auto rnd2 = tikpp::tests::util::random<test_type>();

    test_wrapper w1 {rnd1}, w2 {rnd2};

    EXPECT_EQ(w1 == w2, rnd1 == rnd2);
    EXPECT_EQ(w1 != w2, rnd1 != rnd2);
    EXPECT_EQ(w1 < w2, rnd1 < rnd2);
    EXPECT_EQ(w1 <= w2, rnd1 <= rnd2);
    EXPECT_EQ(w1 > w2, rnd1 > rnd2);
    EXPECT_EQ(w1 >= w2, rnd1 >= rnd2);
}

} // namespace tikpp::tests
