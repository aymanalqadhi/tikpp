#include "tikpp/data/types/wrapper.hpp"
#include "tikpp/tests/util/random.hpp"

#include "gtest/gtest.h"

#include <cstdint>

using test_type = std::uint64_t;

namespace tikpp::tests {

TEST(StatefullValueWrapper, OperatorsTest) {
    auto rnd1 = tikpp::tests::util::random<test_type>(
        1, std::numeric_limits<test_type>::max() / 2);
    auto rnd2 = tikpp::tests::util::random<test_type>(
        1, std::numeric_limits<test_type>::max() / 2);

    tikpp::data::types::stateful_wrapper<test_type> w1 {rnd1};
    tikpp::data::types::stateful_wrapper<test_type> w2 {rnd2};

    EXPECT_EQ(w1.value(), rnd1);
    EXPECT_FALSE(w1.changed());
    EXPECT_TRUE(w1.has_value());
    EXPECT_EQ(w2.value(), rnd2);
    EXPECT_FALSE(w2.changed());
    EXPECT_TRUE(w2.has_value());

    EXPECT_EQ(w1 + w2, rnd1 + rnd2);
    EXPECT_EQ(w1 - w2, rnd1 - rnd2);
    EXPECT_EQ(w1 / w2, rnd1 / rnd2);
    EXPECT_EQ(w1 * w2, rnd1 * rnd2);
    EXPECT_EQ(w1 % w2, rnd1 % rnd2);

    w1 += w2;

    EXPECT_TRUE(w1.changed());
    EXPECT_EQ(w1, rnd1 + rnd2);

    w1.changed(false);
    EXPECT_FALSE(w1.changed());

    w1 -= w2;

    EXPECT_EQ(w1, rnd1);
    EXPECT_TRUE(w1.changed());

    w1.changed(false);
    EXPECT_FALSE(w1.changed());

    w1 *= w2;

    EXPECT_TRUE(w1.changed());
    EXPECT_EQ(w1, rnd1 * rnd2);

    w1.changed(false);
    EXPECT_FALSE(w1.changed());

    w1 /= w2;

    EXPECT_TRUE(w1.changed());
    EXPECT_EQ(w1, (rnd1 * rnd2) / rnd2);
}

TEST(StatefulValueWrapperTypeTests, HasValueTest) {
    auto rnd1 = tikpp::tests::util::random<test_type>();
    auto rnd2 = tikpp::tests::util::random<test_type>();

    tikpp::data::types::stateful_value_wrapper<test_type> w1 {rnd1};

    EXPECT_TRUE(w1.has_value());
    EXPECT_EQ(w1.value(), rnd1);

    tikpp::data::types::stateful_value_wrapper<test_type> w2 {};
    EXPECT_FALSE(w2.has_value());

    w2.value(rnd2);
    EXPECT_TRUE(w2.has_value());
    EXPECT_EQ(w2.value(), rnd2);
}

} // namespace tikpp::tests
