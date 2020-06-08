#include "tikpp/tests/util/random.hpp"

#include "gtest/gtest.h"

#include <algorithm>
#include <cctype>
#include <string>

using tikpp::tests::util::random_string;
using tikpp::tests::util::random_string_options;

namespace {

constexpr auto test_string_size = 0xFF;

}

namespace tikpp::tests {

TEST(UtilRandomTests, NumOnlyTest) {
    auto str = random_string(::test_string_size, random_string_options::num);

    EXPECT_EQ(str.size(), ::test_string_size);
    EXPECT_TRUE(std::all_of(str.begin(), str.end(),
                            [](auto c) { return std::isdigit(c); }));
}

TEST(UtilRandomTests, AlphaTest) {
    auto str = random_string(::test_string_size, random_string_options::alpha);

    EXPECT_EQ(str.size(), ::test_string_size);
    EXPECT_TRUE(std::all_of(str.begin(), str.end(),
                            [](auto c) { return std::isalpha(c); }));
}

TEST(UtilRandomTests, LowerAlphaTest) {
    auto str = random_string(::test_string_size,
                             random_string_options::alpha |
                                 random_string_options::lower_case);

    EXPECT_EQ(str.size(), ::test_string_size);
    EXPECT_TRUE(std::all_of(str.begin(), str.end(), [](auto c) {
        return std::isalpha(c) && std::islower(c);
    }));
}

TEST(UtilRandomTests, UpperAlphaTest) {
    auto str = random_string(::test_string_size,
                             random_string_options::alpha |
                                 random_string_options::upper_case);

    EXPECT_EQ(str.size(), ::test_string_size);
    EXPECT_TRUE(std::all_of(str.begin(), str.end(), [](auto c) {
        return std::isalpha(c) && std::isupper(c);
    }));
}

TEST(UtilRandomTests, MixedAlphaTest) {
    auto str = random_string(::test_string_size,
                             random_string_options::alpha |
                                 random_string_options::mixed_case);

    EXPECT_EQ(str.size(), ::test_string_size);
    EXPECT_TRUE(std::all_of(str.begin(), str.end(), [](auto c) {
        return std::isalpha(c) && (std::islower(c) || std::isupper(c));
    }));
}

TEST(UtilRandomTests, AlphaNumTest) {
    auto str =
        random_string(::test_string_size, random_string_options::alphanum);

    EXPECT_EQ(str.size(), ::test_string_size);
    EXPECT_TRUE(std::all_of(str.begin(), str.end(),
                            [](auto c) { return std::isalnum(c); }));
}

TEST(UtilRandomTests, SymboleTest) {
    auto str = random_string(::test_string_size, random_string_options::sym);

    EXPECT_EQ(str.size(), ::test_string_size);
    EXPECT_TRUE(std::all_of(str.begin(), str.end(),
                            [](auto c) { return !std::isalnum(c); }));
}

TEST(UtilRandomTests, MixedTest) {
    auto str = random_string(::test_string_size, random_string_options::mixed);
    EXPECT_EQ(str.size(), ::test_string_size);
}

TEST(UtilRandomTests, IntTest) {
    constexpr auto min = 1;
    constexpr auto max = 100;

    auto value = tikpp::tests::util::random<int>(min, max);

    EXPECT_GE(value, min);
    EXPECT_LE(value, max);
}

TEST(UtilRandomTests, DoubleTest) {
    constexpr auto min = 1.0;
    constexpr auto max = 100.0;

    auto value = tikpp::tests::util::random<double>(min, max);

    EXPECT_GE(value, min);
    EXPECT_LE(value, max);
}

TEST(UtilRandomTests, LongTest) {
    constexpr auto min = 1UL;
    constexpr auto max = 100UL;

    auto value = tikpp::tests::util::random<long>(min, max);

    EXPECT_GE(value, min);
    EXPECT_LE(value, max);
}

} // namespace tikpp::tests
