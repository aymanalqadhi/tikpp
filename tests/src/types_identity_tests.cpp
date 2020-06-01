#include "tikpp/models/types/identity.hpp"

#include "gtest/gtest.h"

using namespace tikpp::models::types::literals;

using tikpp::models::types::identity;

namespace tikpp::tests {

TEST(IdentityTypeTests, FromStringTest) {
    identity id1 {"*12345678"};
    identity id2 {"*11223344"};
    identity id3 {"*DEADBEEF"};
    identity id4 {"*ABCDEF01"};

    EXPECT_EQ(id1, 0X12345678);
    EXPECT_EQ(id2, 0X11223344);
    EXPECT_EQ(id3, 0XDEADBEEF);
    EXPECT_EQ(id4, 0XABCDEF01);
}

TEST(IdentityTypeTests, ToStringTest) {
    auto id1 = 0x12345678_i;
    auto id2 = 0x11223344_i;
    auto id3 = 0xDEADBEEF_i;
    auto id4 = 0xABCDEF01_i;

    EXPECT_EQ(id1.to_string(), "*12345678");
    EXPECT_EQ(id2.to_string(), "*11223344");
    EXPECT_EQ(id3.to_string(), "*DEADBEEF");
    EXPECT_EQ(id4.to_string(), "*ABCDEF01");
}

TEST(IdentityTypeTests, LiteralsTest) {
    identity id1 {"*12345678"};
    identity id2 {"*11223344"};
    identity id3 {"*DEADBEEF"};
    identity id4 {"*ABCDEF01"};

    EXPECT_EQ(id1, 0x12345678_i);
    EXPECT_EQ(id2, 0x11223344_i);
    EXPECT_EQ(id3, 0xDEADBEEF_i);
    EXPECT_EQ(id4, 0xABCDEF01_i);
}

} // namespace tikpp::tests
