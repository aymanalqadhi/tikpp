#include "tikpp/request.hpp"
#include "tikpp/tests/fakes/socket.hpp"
#include "tikpp/tests/fixtures/basic_api.hpp"
#include "tikpp/tests/util/random.hpp"

#include "fmt/format.h"
#include "gtest/gtest.h"
#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <cstdint>
#include <memory>
#include <vector>

namespace {

template <typename... Arg>
std::vector<std::uint8_t> make_sentence(Arg &&... args) {
    std::vector<std::uint8_t> buf {};

    (tikpp::detail::encode_word(
         tikpp::detail::convert_back(std::forward<Arg>(args)), buf),
     ...);

    tikpp::detail::encode_length(0, buf);
    return buf;
}

} // namespace

namespace tikpp::tests {

using tikpp::tests::fixtures::BasicApiTest;
using tikpp::tests::fixtures::ConnectedBasicApiTest;

TEST_F(BasicApiTest, ConnectTest) {
    EXPECT_FALSE(api->is_open());

    api->socket().always_fails(true);
    api->async_open(ConnectedBasicApiTest::test_ip_address,
                    ConnectedBasicApiTest::test_api_port,
                    [this](const auto &err) {
                        EXPECT_TRUE(err);
                        EXPECT_FALSE(api->is_open());

                        api->socket().always_fails(false);
                        api->async_open(ConnectedBasicApiTest::test_ip_address,
                                        ConnectedBasicApiTest::test_api_port,
                                        [this](const auto &err) {
                                            EXPECT_FALSE(err);
                                            EXPECT_TRUE(api->is_open());
                                            api->close();
                                            EXPECT_FALSE(api->is_open());
                                        });
                    });

    io.run();
}

TEST_F(BasicApiTest, ConnectLoginTest) {
    auto name = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);
    auto password = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);

    auto req = ::make_sentence("/login", ".tag=0",
                               fmt::format("=password={}", password),
                               fmt::format("=name={}", name));

    auto resp = ::make_sentence("!done", ".tag=0");
    boost::asio::write(api->socket().input_pipe(), boost::asio::buffer(resp));

    EXPECT_FALSE(api->is_open());

    api->socket().always_fails(true);
    api->async_open(ConnectedBasicApiTest::test_ip_address,
                    ConnectedBasicApiTest::test_api_port, name, password,
                    [this, name, password, &req](const auto &err) {
                        EXPECT_TRUE(err);
                        EXPECT_FALSE(api->is_open());

                        api->socket().always_fails(false);
                        api->async_open(
                            ConnectedBasicApiTest::test_ip_address,
                            ConnectedBasicApiTest::test_api_port, name,
                            password, [this, &req](const auto &err) {
                                EXPECT_FALSE(err);
                                EXPECT_TRUE(api->is_open());

                                std::vector<std::uint8_t> buf {};
                                buf.resize(req.size());

                                boost::asio::read(api->socket().output_pipe(),
                                                  boost::asio::buffer(buf));

                                for (std::size_t i {0}; i < req.size(); ++i) {
                                    EXPECT_EQ(req[i], buf[i]);
                                }

                                api->close();
                                EXPECT_FALSE(api->is_open());
                            });
                    });
    io.run();
}

TEST_F(ConnectedBasicApiTest, MakeRequestTest) {
    EXPECT_TRUE(api->is_open());

    auto command = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);
    auto arg1 = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);
    auto arg2 = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);
    auto arg3 = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);
    auto arg4 = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);

    auto req = api->make_request(command);
    req->add_param("arg1", arg1);
    req->add_param("arg2", arg2);
    req->add_param("arg3", arg3);
    req->add_param("arg4", arg4);

    EXPECT_EQ(req->size(), 4);
    EXPECT_EQ(req->tag(), api->aquire_unique_tag() - 1);
    EXPECT_TRUE(req->contains("=arg1"));
    EXPECT_TRUE(req->contains("=arg2"));
    EXPECT_TRUE(req->contains("=arg3"));
    EXPECT_TRUE(req->contains("=arg4"));
}

TEST_F(ConnectedBasicApiTest, SendTest) {
    auto command = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);
    auto value1 = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);
    auto value2 = tikpp::tests::util::random_string(
        0xFF, tikpp::tests::util::random_string_options::mixed);

    auto req = api->make_request(command);
    req->add_param("key1", value1);
    req->add_param("key2", value2);

    auto resp = ::make_sentence("!done", fmt::format("=key1={}", value1),
                                fmt::format("=key2={}", value2),
                                fmt::format(".tag={}", req->tag()));
    boost::asio::write(api->socket().input_pipe(), boost::asio::buffer(resp));

    api->async_send(std::move(req),
                    [this, &value1, &value2](const auto &err, auto &&resp) {
                        EXPECT_FALSE(err);
                        EXPECT_EQ(resp.type(), tikpp::response_type::normal);
                        EXPECT_TRUE(resp.contains("key1"));
                        EXPECT_TRUE(resp.contains("key2"));
                        EXPECT_EQ(resp["key1"], value1);
                        EXPECT_EQ(resp["key2"], value2);

                        api->close();
                        return false;
                    });

    io.poll();
}

TEST_F(ConnectedBasicApiTest, FatalResponseTest) {
    auto resp = ::make_sentence("!fatal", "Some message");
    boost::asio::write(api->socket().input_pipe(), boost::asio::buffer(resp));

    auto req = api->make_request("/test/command");
    api->async_send(std::move(req), [this](const auto &err, auto &&resp) {
        EXPECT_FALSE(true);
        return false;
    });

    io.run();

    EXPECT_FALSE(api->is_open());
    EXPECT_TRUE(last_error());
    EXPECT_EQ(last_error().value(),
              static_cast<int>(tikpp::error_code::fatal_response));
}

TEST_F(BasicApiTest, NotConnectedSendTest) {
    EXPECT_FALSE(api->is_open());

    auto req = api->make_request("/test/command");
    api->async_send(std::move(req), [](const auto &err, auto &&resp) {
        EXPECT_TRUE(err);
        EXPECT_EQ(err, boost::asio::error::not_connected);
        return false;
    });

    io.run();
}

TEST_F(ConnectedBasicApiTest, UntaggedSendTest) {
    auto req  = api->make_request("/test/command");
    auto resp = ::make_sentence("!done", "=param=value");
    boost::asio::write(api->socket().input_pipe(), boost::asio::buffer(resp));

    api->async_send(std::move(req), [](const auto &err, auto &&resp) {
        EXPECT_TRUE(false);
        return false;
    });

    io.run();

    EXPECT_FALSE(api->is_open());
    EXPECT_TRUE(last_error());
    EXPECT_EQ(last_error().value(),
              static_cast<int>(tikpp::error_code::untagged_response));
}

TEST_F(ConnectedBasicApiTest, InvalidResponseTest) {
    auto req  = api->make_request("/test/command");
    auto resp = ::make_sentence("invalid header", "invalid param");
    boost::asio::write(api->socket().input_pipe(), boost::asio::buffer(resp));

    api->async_send(std::move(req), [](const auto &err, auto &&resp) {
        EXPECT_TRUE(false);
        return false;
    });

    io.run();

    EXPECT_FALSE(api->is_open());
    EXPECT_TRUE(last_error());
    EXPECT_EQ(last_error().value(),
              static_cast<int>(tikpp::error_code::invalid_response));
}

TEST_F(ConnectedBasicApiTest, HandlerPersistenceTest) {
    constexpr auto test_iterations = 10;

    auto req  = api->make_request("/test/command");
    auto resp = ::make_sentence("!done", "=param=value",
                                fmt::format(".tag={}", req->tag()));
    boost::asio::write(api->socket().input_pipe(), boost::asio::buffer(resp));

    for (std::size_t i {0}; i < test_iterations; ++i) {
        boost::asio::write(api->socket().input_pipe(),
                           boost::asio::buffer(resp));
    }

    api->async_send(std::move(req), [=](const auto &err, auto &&resp) {
        static std::size_t hits {0};

        EXPECT_FALSE(err);
        EXPECT_EQ(resp.type(), tikpp::response_type::normal);
        EXPECT_TRUE(resp.contains("param"));
        EXPECT_EQ(resp["param"], "value");
        EXPECT_LT(hits, test_iterations);

        if (++hits >= test_iterations) {
            api->close();
            return false;
        }

        return true;
    });

    io.run();
}

} // namespace tikpp::tests
