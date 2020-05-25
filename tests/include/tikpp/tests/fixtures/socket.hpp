#ifndef TIKPP_TESTS_FIXTURES_SOCKET_HPP
#define TIKPP_TESTS_FIXTURES_SOCKET_HPP

#include "tikpp/tests/fakes/socket.hpp"

#include "gtest/gtest.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#if !defined(TEST_IP_ADDRESS)
#    if defined(NO_MOCK_OBJECTS)
#        error "Test IP address was not defined"
#    else
#        define TEST_IP_ADDRESS "1.2.3.4"
#    endif
#endif

#if !defined(TEST_API_PORT)
#    if defined(NO_MOCK_OBJECTS)
#        error "Test API port was not defined"
#    else
#        define TEST_API_PORT 1234
#    endif
#endif

namespace tikpp::tests::fixtures {

struct socket : ::testing::Test {
    socket() : sock {io} {
    }

    void SetUp() override {
        sock.async_connect(
            {boost::asio::ip::address::from_string(TEST_IP_ADDRESS),
             TEST_API_PORT},
            [](const auto &err) {
                EXPECT_FALSE(err);
                EXPECT_EQ(err.value(), 0);
            });
        io.run();
        EXPECT_TRUE(sock.is_open());
    }

    boost::asio::io_context io;

#if defined(NO_MOCK_OBJECTS)
    boost::asio::ip::tcp::socket sock;
#else
    tikpp::tests::fakes::socket sock;
#endif
};

} // namespace tikpp::tests::fixtures

#endif
