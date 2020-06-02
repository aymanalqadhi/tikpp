#include "tikpp/models/types/bytes.hpp"

#include "gtest/gtest.h"

#include <cmath>

using namespace tikpp::models::types::literals;

using tikpp::models::types::bytes;

namespace {

inline auto round_2(double x) {
    return std::round(x * 100) / 100.0;
}

} // namespace

namespace tikpp::tests {

TEST(BytesTypeTests, FromBytesCountTest) {
    bytes b1 {128937129841};
    bytes b2 {43895324981247};
    bytes b3 {21783123};
    bytes b4 {946084014};

    EXPECT_FLOAT_EQ(::round_2(b1.kb()), 125915165.86);
    EXPECT_FLOAT_EQ(::round_2(b1.mb()), 122964.02);
    EXPECT_FLOAT_EQ(::round_2(b1.gb()), 120.08);
    EXPECT_FLOAT_EQ(::round_2(b1.tb()), 0.12);

    EXPECT_FLOAT_EQ(::round_2(b2.kb()), 42866528301.99);
    EXPECT_FLOAT_EQ(::round_2(b2.mb()), 41861844.04);
    EXPECT_FLOAT_EQ(::round_2(b2.gb()), 40880.70);
    EXPECT_FLOAT_EQ(::round_2(b2.tb()), 39.92);

    EXPECT_FLOAT_EQ(::round_2(b3.kb()), 21272.58);
    EXPECT_FLOAT_EQ(::round_2(b3.mb()), 20.77);
    EXPECT_FLOAT_EQ(::round_2(b3.gb()), 0.02);
    EXPECT_FLOAT_EQ(::round_2(b3.tb()), 0.00);

    EXPECT_FLOAT_EQ(::round_2(b4.kb()), 923910.16);
    EXPECT_FLOAT_EQ(::round_2(b4.mb()), 902.26);
    EXPECT_FLOAT_EQ(::round_2(b4.gb()), 0.88);
    EXPECT_FLOAT_EQ(::round_2(b4.tb()), 0.00);
}

TEST(BytesTypeTests, ToStringTest) {
    bytes b1 {128937129841};
    bytes b2 {43895324981247};
    bytes b3 {21783123};
    bytes b4 {946084014};

    EXPECT_EQ(b1.to_string(), "120.08 GB");
    EXPECT_EQ(b2.to_string(), "39.92 TB");
    EXPECT_EQ(b3.to_string(), "20.77 MB");
    EXPECT_EQ(b4.to_string(), "902.26 MB");
}

TEST(BytesTypeTests, LiteralsTest) {
    auto b1 = 1200_b;
    auto b2 = 123456_gb;
    auto b3 = 20_mb;

    EXPECT_EQ(b1.value(), 1200);
    EXPECT_EQ(::round_2(b1.kb()), ::round_2(1200 / 1024.0));
    EXPECT_EQ(::round_2(b1.mb()), 0);

    EXPECT_EQ(b2.value(), 123456UL * 1024 * 1024 * 1024);
    EXPECT_EQ(b2.kb(), 123456UL * 1024 * 1024);
    EXPECT_EQ(b2.mb(), 123456UL * 1024);
    EXPECT_EQ(b2.gb(), 123456UL);

    EXPECT_EQ(b3.value(), 20 * 1024 * 1024);
    EXPECT_EQ(b3.kb(), 20 * 1024);
    EXPECT_EQ(b3.mb(), 20);
    EXPECT_EQ(b3.gb(), 20 / 1024.0);
}

TEST(BytesTypeTests, ArthimeticOperatorsTest) {
    auto b1 = 1_gb + 512_mb;

    EXPECT_EQ(b1.value(), (1024 + 512) * 1024 * 1024);
    EXPECT_EQ(b1.kb(), (1024 + 512) * 1024);
    EXPECT_EQ(b1.mb(), 1024 + 512);
    EXPECT_EQ(b1.gb(), 1.5);

    b1 += 1234_tb;

    EXPECT_EQ(b1, 1234_tb + 1_gb + 512_mb);
    EXPECT_NE(b1, 1234_tb + 1_gb + 512_mb + 1_b);

    b1 -= 1_gb;

    EXPECT_EQ(b1, 1234_tb + 512_mb);
    EXPECT_NE(b1, 1234_tb + 512_mb + 1_b);

    b1 -= 512_mb;

    EXPECT_EQ(b1, 1234_tb);
    EXPECT_NE(b1, 1234_tb + 1_b);

    b1 /= 1_kb;

    EXPECT_EQ(b1, 1234_gb);
    EXPECT_NE(b1, 1234_gb + 1_b);

    b1 *= 2_b;

    EXPECT_EQ(b1, 2468_gb);
    EXPECT_NE(b1, 2468_gb + 1_b);
}

TEST(BytesTypeTests, LogicalOperatorsTest) {
    EXPECT_LE(1023_b, 1_kb);
    EXPECT_EQ(1024_b, 1_kb);
    EXPECT_LE(1023_b, 1_kb);

    EXPECT_LE(1023_kb, 1_mb);
    EXPECT_EQ(1024_kb, 1_mb);
    EXPECT_GT(1025_kb, 1_mb);

    EXPECT_LE(1023_mb, 1_gb);
    EXPECT_EQ(1024_mb, 1_gb);
    EXPECT_GT(1025_mb, 1_gb);

    EXPECT_LE(1023_gb, 1_tb);
    EXPECT_EQ(1024_gb, 1_tb);
    EXPECT_GT(1025_gb, 1_tb);

    EXPECT_LE(1023_tb, 1_pb);
    EXPECT_EQ(1024_tb, 1_pb);
    EXPECT_GT(1025_tb, 1_pb);
}

} // namespace tikpp::tests
