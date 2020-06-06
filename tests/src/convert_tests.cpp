#include "tikpp/detail/convert.hpp"
#include "tikpp/tests/util/random.hpp"

#include "fmt/format.h"
#include "gtest/gtest.h"

#include <string>

namespace util = tikpp::tests::util;

namespace tikpp::tests {

TEST(ConvertTests, BooleanTest) {
    EXPECT_TRUE(tikpp::detail::convert<bool>("true"));
    EXPECT_TRUE(tikpp::detail::convert<bool>("yes"));

    EXPECT_FALSE(tikpp::detail::convert<bool>("false"));
    EXPECT_FALSE(tikpp::detail::convert<bool>("no"));

    EXPECT_FALSE(tikpp::detail::convert<bool>(
        util::random_string(10, util::random_string_options::mixed)));
}

TEST(ConvertTests, StringTest) {
    auto str = util::random_string(0xFF, util::random_string_options::mixed);
    EXPECT_EQ(str, tikpp::detail::convert<std::string>(str));
}

TEST(ConvertTests, DoubleTest) {
    auto value = util::random<double>();
    auto str   = fmt::to_string(value);

    EXPECT_EQ(value, tikpp::detail::convert<double>(str));
}

TEST(ConvertTests, IntTest) {
    auto value = util::random<int>();
    auto str   = fmt::to_string(value);

    EXPECT_EQ(value, tikpp::detail::convert<int>(str));
}

TEST(ConvertTests, Uint8Test) {
    auto value = util::random<std::uint8_t>();
    auto str   = fmt::to_string(value);

    EXPECT_EQ(value, tikpp::detail::convert<std::uint8_t>(str));
}

TEST(ConvertTests, Uint16Test) {
    auto value = util::random<std::uint16_t>();
    auto str   = fmt::to_string(value);

    EXPECT_EQ(value, tikpp::detail::convert<std::uint16_t>(str));
}

TEST(ConvertTests, Uint32Test) {
    auto value = util::random<std::uint32_t>();
    auto str   = fmt::to_string(value);

    EXPECT_EQ(value, tikpp::detail::convert<std::uint32_t>(str));
}

TEST(ConvertTests, Uint64Test) {
    auto value = util::random<std::uint64_t>();
    auto str   = fmt::to_string(value);

    EXPECT_EQ(value, tikpp::detail::convert<std::uint64_t>(str));
}

} // namespace tikpp::tests
