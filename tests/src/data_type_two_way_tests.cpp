#include "tikpp/data/types/wrapper.hpp"
#include "tikpp/tests/util/random.hpp"

#include "gtest/gtest.h"

using test_type    = unsigned long;
using test_wrapper = tikpp::data::types::two_way<test_type>;

namespace tikpp::tests {

TEST(TwoWayBinderTypeTests, ChangeTest) {
    auto rnd1 = tikpp::tests::util::random<test_type>();
    auto rnd2 = tikpp::tests::util::random<test_type>();

    test_wrapper w1 {rnd1}, w2 {rnd2};

    EXPECT_FALSE(w1.changed());
    EXPECT_FALSE(w2.changed());

    test_wrapper w3 {w1 + w2};

    EXPECT_FALSE(w1.changed());
    EXPECT_FALSE(w2.changed());
    EXPECT_FALSE(w3.changed());

    EXPECT_EQ(w1, rnd1);
    EXPECT_EQ(w2, rnd2);
    EXPECT_EQ(w3, w1 + w2);

    w3 = w1 * w2;
    EXPECT_TRUE(w3.changed());
    EXPECT_EQ(w3, w1 * w2);

    w3.changed(false);
    EXPECT_FALSE(w3.changed());

    w3 += w1;
    EXPECT_TRUE(w3.changed());
    EXPECT_EQ(w3, w1 * w2 + w1);

    w3 -= w1;
    EXPECT_TRUE(w3.changed());
    EXPECT_EQ(w3, w1 * w2);

    w3.changed(false);
    EXPECT_FALSE(w3.changed());

    w3 = w3 / w2;
    EXPECT_TRUE(w3.changed());
    EXPECT_EQ(w3, (w1 * w2) / w2);

    w3.changed(false);
    EXPECT_FALSE(w3.changed());
}

TEST(TwoWayBinderTypeTests, ArithmeticTest) {
    auto rnd1 = tikpp::tests::util::random<test_type>();
    auto rnd2 = tikpp::tests::util::random<test_type>();

    test_wrapper w1 {rnd1}, w2 {rnd2};

    EXPECT_EQ(static_cast<test_type>(w1 + w2), rnd1 + rnd2);
    EXPECT_EQ(static_cast<test_type>(w1 - w2), rnd1 - rnd2);
    EXPECT_EQ(static_cast<test_type>(w1 * w2), rnd1 * rnd2);
    EXPECT_EQ(static_cast<test_type>(w1 / w2), rnd1 / rnd2);
    EXPECT_EQ(static_cast<test_type>(w1 % w2), rnd1 % rnd2);
}

TEST(TwoWayBinderTypeTests, LogicTest) {
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
