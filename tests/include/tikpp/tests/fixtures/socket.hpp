#ifndef TIKPP_TESTS_FIXTURES_SOCKET_HPP
#define TIKPP_TESTS_FIXTURES_SOCKET_HPP

#include "tikpp/io_context.hpp"
#include "tikpp/tests/fakes/socket.hpp"

#include "gtest/gtest.h"

namespace tikpp::tests::fixtures {

struct SocketTest : ::testing::Test {
    static constexpr auto test_ip_address = "1.2.3.4";
    static constexpr auto test_api_port   = 8728;

    SocketTest() : sock {io} {
    }

    void SetUp() override {
        sock.async_connect(
            {boost::asio::ip::address::from_string(test_ip_address),
             test_api_port},
            [](const auto &err) { EXPECT_FALSE(err); });
        io.run();
        EXPECT_TRUE(sock.is_open());
    }

    tikpp::io_context           io;
    tikpp::tests::fakes::socket sock;
};

} // namespace tikpp::tests::fixtures

#endif
