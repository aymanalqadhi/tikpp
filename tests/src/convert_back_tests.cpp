#include "tikpp/detail/convert.hpp"
#include "tikpp/tests/util/random.hpp"

#include "fmt/format.h"
#include "gtest/gtest.h"

#include <string>

namespace util = tikpp::tests::util;

namespace tikpp::tests {

TEST(ConvertBackTests, BooleanTest) {
    EXPECT_EQ("true", tikpp::detail::convert_back<bool>(true));
    EXPECT_EQ("false", tikpp::detail::convert_back<bool>(false));
}

TEST(ConvertBackTests, StringTest) {
    auto str = util::random_string(0xFF, util::random_string_options::mixed);
    EXPECT_EQ(str, tikpp::detail::convert_back<std::string>(str));
}

TEST(ConvertBackTests, DoubleTest) {
    auto value = util::random<double>();
    auto str   = fmt::to_string(value);

    EXPECT_EQ(str, tikpp::detail::convert_back<double>(value));
}

TEST(ConvertBackTests, IntTest) {
    auto value = util::random<int>();
    auto str   = fmt::to_string(value);

    EXPECT_EQ(str, tikpp::detail::convert_back<int>(value));
}

TEST(ConvertBackTests, Uint8Test) {
    auto value = util::random<std::uint8_t>();
    auto str   = fmt::to_string(value);

    EXPECT_EQ(str, tikpp::detail::convert_back<std::uint8_t>(value));
}

TEST(ConvertBackTests, Uint16Test) {
    auto value = util::random<std::uint16_t>();
    auto str   = fmt::to_string(value);

    EXPECT_EQ(str, tikpp::detail::convert_back<std::uint16_t>(value));
}

TEST(ConvertBackTests, Uint32Test) {
    auto value = util::random<std::uint32_t>();
    auto str   = fmt::to_string(value);

    EXPECT_EQ(str, tikpp::detail::convert_back<std::uint32_t>(value));
}

TEST(ConvertBackTests, Uint64Test) {
    auto value = util::random<std::uint64_t>();
    auto str   = fmt::to_string(value);

    EXPECT_EQ(str, tikpp::detail::convert_back<std::uint64_t>(value));
}

} // namespace tikpp::tests
