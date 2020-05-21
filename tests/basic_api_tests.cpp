#include "mocks/socket.hpp"
#include "tikpp/api.hpp"

#include "gtest/gtest.h"
#include <boost/asio/error.hpp>

#include <cstdint>
#include <future>
#include <iostream>
#include <random>

namespace {

constexpr auto invalid_ip_address = "Invalid address";
constexpr auto valid_ip_address   = "1.2.3.4";

} // namespace

struct ApiTest : ::testing::Test {
    void SetUp() override {
        api = tikpp::basic_api<tikpp::tests::mocks::socket>::create(
            io, tikpp::api_version::v1);
    }

    void TearDown() override {
    }

    boost::asio::io_context                                        io;
    std::shared_ptr<tikpp::basic_api<tikpp::tests::mocks::socket>> api;
    std::random_device                                             random {};
};

TEST_F(ApiTest, AsyncConnectTest) {
    std::uniform_int_distribution<std::uint16_t> dist {};
    std::promise<void>                           prom {};
    std::future<void>                            fut {prom.get_future()};

    api->async_open(::invalid_ip_address, dist(random),
                    [&prom](const auto &err) {
                        EXPECT_TRUE(err);
                        EXPECT_EQ(err, boost::asio::error::invalid_argument);
                        prom.set_value();
                    });
    fut.wait();
    EXPECT_FALSE(api->socket().is_open());
    EXPECT_FALSE(api->is_open());

    prom = std::promise<void> {};
    fut  = prom.get_future();
    api->socket().always_fail(true);

    api->async_open(::valid_ip_address, dist(random), [&prom](const auto &err) {
        EXPECT_TRUE(err);
        EXPECT_EQ(err, boost::asio::error::fault);
        prom.set_value();
    });
    fut.wait();
    EXPECT_FALSE(api->socket().is_open());
    EXPECT_FALSE(api->is_open());

    prom = std::promise<void> {};
    fut  = prom.get_future();
    api->socket().always_fail(false);

    api->async_open(::valid_ip_address, dist(random), [&prom](const auto &err) {
        EXPECT_FALSE(err);
        EXPECT_EQ(err.value(), 0);
        prom.set_value();
    });
    fut.wait();
    EXPECT_TRUE(api->socket().is_open());
    EXPECT_TRUE(api->is_open());
    EXPECT_EQ(::valid_ip_address,
              api->socket().remote_endpoint().address().to_string());
}
