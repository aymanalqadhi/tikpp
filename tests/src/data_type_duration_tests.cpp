#include "tikpp/data/types/duration.hpp"
#include "tikpp/tests/util/random.hpp"

#include "tikpp/detail/type_traits/operators.hpp"

#include "fmt/format.h"
#include "gtest/gtest.h"

#include <chrono>

using namespace std::chrono_literals;
using namespace tikpp::data::types::literals;

using tikpp::tests::util::random_string;
using tikpp::tests::util::random_string_options;

using test_rep_type      = std::uint32_t;
using test_duration_type = std::chrono::seconds;

namespace {

constexpr auto test_string_size = 0xFF;

}

namespace tikpp::tests {

TEST(DurationTypeTests, LiteralsTest) {
    EXPECT_EQ((1_s).value(), 1s);
    EXPECT_EQ((1_m).value(), 1min);
    EXPECT_EQ((1_h).value(), 1h);
    EXPECT_EQ((1_d).value(), 24h);
    EXPECT_EQ((1_w).value(), 24h * 7);
}

TEST(DurationTypeTests, OperatorsTest) {
    tikpp::data::types::duration<test_duration_type> dur {1};

    EXPECT_EQ(dur.value(), test_duration_type {1});

    dur = test_duration_type {2};
    EXPECT_EQ(dur.value(), test_duration_type {2});

    dur += 123s;
    EXPECT_EQ(dur.value(), test_duration_type {2} + 123s);

    dur -= test_duration_type {2};
    EXPECT_EQ(dur.value(), 123s);

    dur = dur.value() * 2;
    EXPECT_EQ(dur.value(), 246s);

    dur = dur.value() / 123;
    EXPECT_EQ(dur.value(), 2s);
}

TEST(DurationTypeTests, ToStringTest) {
    auto rnd = test_duration_type {tikpp::tests::util::random<test_rep_type>()};
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(rnd).count();

    tikpp::data::types::duration<test_duration_type> dur {rnd};
    EXPECT_EQ(dur.to_string(), fmt::format("{}s", secs));
}

TEST(DurationTypeTests, ToHumanReadableString) {
    constexpr auto weeks_mul   = 60 * 60 * 24 * 7;
    constexpr auto days_mul    = 60 * 60 * 24;
    constexpr auto hours_mul   = 60 * 60;
    constexpr auto minutes_mul = 60;
    constexpr auto seconds_mul = 1;

    auto rnd = test_duration_type {tikpp::tests::util::random<test_rep_type>()};
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(rnd).count();

    tikpp::data::types::duration<test_duration_type> dur {rnd};
    std::string                                      expected {};

    const auto append_unit = [&secs, &expected](const std::string &label,
                                                std::size_t mul) noexcept {
        auto amount = secs / mul;

        if (amount > 0) {
            if (amount > 1) {
                fmt::format_to(std::back_inserter(expected), "{} {}s", amount,
                               label);
            } else {
                fmt::format_to(std::back_inserter(expected), "{} {}", amount,
                               label);
            }

            secs -= amount * mul;

            if (secs > 0) {
                fmt::format_to(std::back_inserter(expected), ", ");
            }
        }
    };

    append_unit("Week", weeks_mul);
    append_unit("Day", days_mul);
    append_unit("Hour", hours_mul);
    append_unit("Minute", minutes_mul);
    append_unit("Second", seconds_mul);

    EXPECT_EQ(expected, dur.to_human_readable_string());
}

TEST(DurationTypeTests, InvalidStringTest) {
    auto str = random_string(::test_string_size,
                             random_string_options::alpha |
                                 random_string_options::mixed_case);

    auto dur = tikpp::data::types::duration<std::chrono::seconds>(str);
    EXPECT_EQ(0, dur.value().count());
}

TEST(DurationTypeTests, UniversialFormatTest) {
    auto secs  = tikpp::tests::util::random<test_rep_type>();
    auto mins  = tikpp::tests::util::random<test_rep_type>();
    auto hours = tikpp::tests::util::random<test_rep_type>();
    auto days  = tikpp::tests::util::random<test_rep_type>();
    auto weeks = tikpp::tests::util::random<test_rep_type>();

    auto str = fmt::format("{}w{}d{}h{}m{}s", weeks, days, hours, mins, secs);
    auto dur = tikpp::data::types::duration<test_duration_type> {str};

    EXPECT_EQ(std::chrono::duration_cast<test_duration_type>(
                  (std::chrono::hours {weeks} * 24 * 7) +
                  (std::chrono::hours {days} * 24) +
                  std::chrono::hours {hours} + std::chrono::minutes {mins} +
                  std::chrono::seconds {secs}),
              dur.value());
}

TEST(DurationTypeTests, HHMMSSFormatTest) {
    auto secs  = tikpp::tests::util::random<test_rep_type>();
    auto mins  = tikpp::tests::util::random<test_rep_type>();
    auto hours = tikpp::tests::util::random<test_rep_type>();

    auto str = fmt::format("{}:{}:{}", hours, mins, secs);
    auto dur = tikpp::data::types::duration<test_duration_type>(str);

    EXPECT_EQ(std::chrono::duration_cast<test_duration_type>(
                  std::chrono::hours {hours} + std::chrono::minutes {mins} +
                  std::chrono::seconds {secs}),
              dur.value());
}

TEST(DurationTypeTests, MixedFormatTest) {
    auto secs  = tikpp::tests::util::random<test_rep_type>();
    auto mins  = tikpp::tests::util::random<test_rep_type>();
    auto hours = tikpp::tests::util::random<test_rep_type>();
    auto days  = tikpp::tests::util::random<test_rep_type>();
    auto weeks = tikpp::tests::util::random<test_rep_type>();

    auto str = fmt::format("{}w{}d{}h{}m{}s{}:{}:{}", weeks, days, hours, mins,
                           secs, hours, mins, secs);
    auto dur = tikpp::data::types::duration<test_duration_type>(str);

    EXPECT_EQ(std::chrono::duration_cast<test_duration_type>(
                  (std::chrono::hours {weeks} * 24 * 7) +
                  (std::chrono::hours {days} * 24) +
                  std::chrono::hours {hours} * 2 +
                  (std::chrono::minutes {mins} * 2) +
                  (std::chrono::seconds {secs} * 2)),
              dur.value());
}

} // namespace tikpp::tests
