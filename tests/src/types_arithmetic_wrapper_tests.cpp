#include "tikpp/models/types/arithmetic_wrapper.hpp"
#include "tikpp/tests/util/random.hpp"

#include "gtest/gtest.h"

#include <limits>
#include <type_traits>

#include "fmt/format.h"

using test_type = unsigned long;

namespace tikpp::tests {

struct test_wrapper
    : tikpp::models::types::arithmetic_wrapper<test_wrapper, test_type> {
    test_wrapper(test_type value)
        : value_ {value}, arithmetic_wrapper<test_wrapper, test_type> {value_} {
    }

    using arithmetic_wrapper<test_wrapper, test_type>::operator=;

    test_type value_;
};

TEST(ArithmeticWrapperTypeTests, PlusTest) {
    auto rnd1 = tikpp::tests::util::random<test_type>();
    auto rnd2 = tikpp::tests::util::random<test_type>();

    test_wrapper w1 {rnd1}, w2 {rnd2};

    EXPECT_EQ(w1 + w2, rnd1 + rnd2);
}

TEST(ArithmeticWrapperTypeTests, MinusTest) {
    auto rnd1 = tikpp::tests::util::random<test_type>();
    auto rnd2 = tikpp::tests::util::random<test_type>();

    test_wrapper w1 {rnd1}, w2 {rnd2};
    EXPECT_EQ(w1 - w2, rnd1 - rnd2);
}

TEST(ArithmeticWrapperTypeTests, MultiplyTest) {
    auto rnd1 = tikpp::tests::util::random<test_type>();
    auto rnd2 = tikpp::tests::util::random<test_type>();

    test_wrapper w1 {rnd1}, w2 {rnd2};
    EXPECT_EQ(w1 * w2, rnd1 * rnd2);
}

TEST(ArithmeticWrapperTypeTests, DivideTest) {
    auto rnd1 = tikpp::tests::util::random<test_type>();
    auto rnd2 = tikpp::tests::util::random<test_type>();

    test_wrapper w1 {rnd1}, w2 {rnd2};
    EXPECT_EQ(w1 / w2, rnd1 / rnd2);
}

TEST(ArithmeticWrapperTypeTests, ModTest) {
    auto rnd1 = tikpp::tests::util::random<test_type>();
    auto rnd2 = tikpp::tests::util::random<test_type>();

    test_wrapper w1 {rnd1}, w2 {rnd2};
    EXPECT_EQ(w1 % w2, rnd1 % rnd2);
}

TEST(ArithmeticWrapperTypeTests, AssignmentTest) {
    constexpr test_type test_factor {2};

    auto rnd = tikpp::tests::util::random<test_type>();

    test_wrapper w {rnd};

    EXPECT_EQ(w.value(), w.value_);
    EXPECT_EQ(w.value(), w);
    EXPECT_EQ(w.value(), rnd);

    rnd = tikpp::tests::util::random<test_type>(
        0, std::numeric_limits<test_type>().max() / (test_factor * 2));

    w = rnd;
    EXPECT_EQ(w.value(), rnd);

    w += test_factor;
    EXPECT_EQ(w.value(), rnd + test_factor);

    w -= test_factor * 2;
    EXPECT_EQ(w.value(), rnd - test_factor);

    w += test_factor;
    EXPECT_EQ(w.value(), rnd);

    w *= test_factor;
    EXPECT_EQ(w.value(), rnd * test_factor);

    w /= test_factor;
    EXPECT_EQ(w.value(), rnd);

    if constexpr (std::is_integral_v<test_type>) {
        w %= test_factor;
        EXPECT_LT(w.value(), test_factor);
    }
}

} // namespace tikpp::tests
