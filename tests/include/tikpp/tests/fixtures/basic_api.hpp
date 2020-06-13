#ifndef TIKPP_TESTS_FIXTURES_BASIC_API_HPP
#define TIKPP_TESTS_FIXTURES_BASIC_API_HPP

#include "tikpp/api.hpp"
#include "tikpp/io_context.hpp"
#include "tikpp/tests/fakes/socket.hpp"

#include "gtest/gtest.h"
#include <boost/system/error_code.hpp>

namespace tikpp::tests::fixtures {

struct BasicApiTest : ::testing::Test {
    using socket_type = tikpp::tests::fakes::socket;

    BasicApiTest()
        : api {tikpp::make_api_te<socket_type>(io, [this](const auto &err) {
              fmt::print("[!] An error occured: {}\n", err.message());
              last_error_ = err;
          })} {
    }

    inline auto last_error() const noexcept
        -> const boost::system::error_code & {
        return last_error_;
    }

    tikpp::io_context                                 io;
    std::shared_ptr<tikpp::basic_api_te<socket_type>> api;

  private:
    boost::system::error_code last_error_ {};
};

struct ConnectedBasicApiTest : BasicApiTest {
    static constexpr auto test_ip_address = "1.2.3.4";
    static constexpr auto test_api_port   = 8728;

    void SetUp() override {
        api->async_open(test_ip_address, test_api_port,
                        [](const auto &err) { EXPECT_FALSE(err); });
        io.poll();
        EXPECT_TRUE(api->is_open());
    }
};

} // namespace tikpp::tests::fixtures

#endif
