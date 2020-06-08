#include "tikpp/detail/operations/async_connect.hpp"
#include "tikpp/tests/fakes/socket.hpp"
#include "tikpp/io_context.hpp"

#include "gtest/gtest.h"
#include <boost/asio/async_result.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace {

constexpr auto invalid_ip_address = "Invalid address";
constexpr auto valid_ip_address   = "127.0.0.1";
constexpr auto api_port           = 8728;

} // namespace

namespace tikpp::tests {

TEST(AsyncConnectTest, ValidityTest) {
    tikpp::io_context io {};

#if defined(NO_MOCK_OBJECTS)
    boost::asio::ip::tcp::socket sock {io};
#else
    tikpp::tests::fakes::socket sock {io};
#endif

    ASSERT_FALSE(sock.is_open());

    tikpp::detail::operations::async_connect(
        sock, ::invalid_ip_address, ::api_port, [&sock](const auto &err) {
            EXPECT_TRUE(err);
            EXPECT_FALSE(sock.is_open());
        });
    io.run();
    EXPECT_FALSE(sock.is_open());

    tikpp::detail::operations::async_connect(
        sock, ::valid_ip_address, ::api_port, [&sock](const auto &err) {
            EXPECT_FALSE(err);
            EXPECT_TRUE(sock.is_open());
        });
    io.run();
    EXPECT_TRUE(sock.is_open());
}

} // namespace tikpp::tests
