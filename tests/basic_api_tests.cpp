#include "mocks/socket.hpp"
#include "tikpp/api.hpp"

#include "gtest/gtest.h"
#include <boost/asio/error.hpp>

#include <cstdint>
#include <future>
#include <iostream>
#include <random>

#define USE_MOCK_SOCKET

namespace {

constexpr auto invalid_ip_address = "Invalid address#@!#";
constexpr auto valid_ip_address   = "127.0.0.1";
constexpr auto api_port           = 8728;

} // namespace

struct ApiTest : ::testing::Test {
    void SetUp() override {
#if defined(USE_MOCK_SOCKET)
        api = tikpp::basic_api<tikpp::tests::mocks::socket>::create(
            io, tikpp::api_version::v1);
#else
        api = tikpp::api::create(io, tikpp::api_version::v1);
#endif
    }

    void TearDown() override {
    }

    boost::asio::io_context io;

#if defined(USE_MOCK_SOCKET)
    std::shared_ptr<tikpp::basic_api<tikpp::tests::mocks::socket>> api;
#else
    std::shared_ptr<tikpp::api> api;
#endif
};

TEST_F(ApiTest, AsyncConnectTest) {
    std::promise<void>                           prom {};
    std::future<void>                            fut {prom.get_future()};

    api->async_open(::invalid_ip_address, ::api_port,
                    [&prom](const auto &err) {
                        EXPECT_TRUE(err);
                        EXPECT_EQ(err, boost::asio::error::invalid_argument);
                        prom.set_value();
                    });
    fut.wait();
    EXPECT_FALSE(api->socket().is_open());
    EXPECT_FALSE(api->is_open());
    EXPECT_EQ(api->state(), tikpp::api_state::closed);

#if defined(USE_MOCK_SOCKET)
    prom = std::promise<void> {};
    fut  = prom.get_future();
    api->socket().always_fail(true);

    api->async_open(::valid_ip_address, ::api_port, [&prom](const auto &err) {
        EXPECT_TRUE(err);
        EXPECT_EQ(err, boost::asio::error::fault);
        prom.set_value();
    });
    fut.wait();
    EXPECT_FALSE(api->socket().is_open());
    EXPECT_FALSE(api->is_open());
    EXPECT_EQ(api->state(), tikpp::api_state::closed);

    api->socket().always_fail(false);
#endif

    prom = std::promise<void> {};
    fut  = prom.get_future();

    api->async_open(::valid_ip_address, ::api_port, [&prom](const auto &err) {
        EXPECT_FALSE(err);
        EXPECT_EQ(err.value(), 0);
        prom.set_value();
    });
    fut.wait();
    EXPECT_TRUE(api->socket().is_open());
    EXPECT_TRUE(api->is_open());
    EXPECT_NE(api->state(), tikpp::api_state::closed);
    EXPECT_EQ(::valid_ip_address,
              api->socket().remote_endpoint().address().to_string());

    api->close();
    EXPECT_FALSE(api->socket().is_open());
    EXPECT_FALSE(api->is_open());
    EXPECT_EQ(api->state(), tikpp::api_state::closed);
}
