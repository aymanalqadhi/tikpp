#include "tikpp/commands/add.hpp"
#include "tikpp/data/converters/creator.hpp"
#include "tikpp/data/converters/dissolver.hpp"
#include "tikpp/data/repository.hpp"
#include "tikpp/request.hpp"
#include "tikpp/tests/fakes/model.hpp"
#include "tikpp/tests/fixtures/basic_api.hpp"

#include "fmt/format.h"
#include "gtest/gtest.h"
#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <cstdint>
#include <type_traits>
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

struct RepositoryTests : tikpp::tests::fixtures::ConnectedBasicApiTest {
    RepositoryTests() : ConnectedBasicApiTest {}, repo {api} {
    }

    tikpp::data::repository<tikpp::tests::fakes::model2,
                            std::decay_t<decltype(api)>>
        repo;
};

TEST_F(RepositoryTests, LoadTest) {
    constexpr auto test_iterations = 10;

    for (std::size_t i {0}; i < test_iterations; ++i) {
        auto buf = ::make_sentence(
            "!re", fmt::format("=id=*{:X}", i),
            fmt::format("=sticky-data=sticky_data_#{}", i),
            fmt::format("=read-only-data=read_only_data_#{}", i),
            fmt::format("=read-write-data=read_write_data_#{}", i), "=.tag=0");
        boost::asio::write(api->socket().input_pipe(),
                           boost::asio::buffer(buf));
    }

    boost::asio::write(
        api->socket().input_pipe(),
        boost::asio::buffer(::make_sentence("!done", "=.tag=0")));

    repo.async_load([&](const auto &err, auto &&items) {
        EXPECT_FALSE(err);
        EXPECT_EQ(items.size(), test_iterations);

        for (std::size_t i {0}; i < items.size(); ++i) {
            EXPECT_EQ(i, items[i].id.value());
            EXPECT_EQ(fmt::format("sticky_data_#{}", i),
                      items[i].sticky_data.value());
            EXPECT_EQ(fmt::format("read_only_data_#{}", i),
                      items[i].read_only_data.value());
            EXPECT_EQ(fmt::format("read_write_data_#{}", i),
                      items[i].read_write_data.value());
        }

        EXPECT_TRUE(api->is_open());
        api->close();
    });

    io.run();
}

TEST_F(RepositoryTests, StreamTest) {
    constexpr auto test_iterations = 10;

    for (std::size_t i {0}; i < test_iterations; ++i) {
        auto buf = ::make_sentence(
            "!re", fmt::format("=id=*{:X}", i),
            fmt::format("=sticky-data=sticky_data_#{}", i),
            fmt::format("=read-only-data=read_only_data_#{}", i),
            fmt::format("=read-write-data=read_write_data_#{}", i), "=.tag=0");
        boost::asio::write(api->socket().input_pipe(),
                           boost::asio::buffer(buf));
    }

    boost::asio::write(
        api->socket().input_pipe(),
        boost::asio::buffer(::make_sentence("!done", "=.tag=0")));

    repo.async_stream([&](const auto &err, auto &&item) {
        static std::size_t current {0};

        EXPECT_TRUE(api->is_open());

        if (err == tikpp::error_code::list_end) {
            api->close();
            return;
        }

        EXPECT_FALSE(err);
        EXPECT_EQ(current, item.id.value());
        EXPECT_EQ(fmt::format("sticky_data_#{}", current),
                  item.sticky_data.value());
        EXPECT_EQ(fmt::format("read_only_data_#{}", current),
                  item.read_only_data.value());
        EXPECT_EQ(fmt::format("read_write_data_#{}", current),
                  item.read_write_data.value());

        ++current;
    });

    io.run();
}

TEST_F(RepositoryTests, AddTest) {
    constexpr auto test_iterations = 10;

    std::vector<std::uint8_t> buf {};

    for (std::size_t i {0}; i < test_iterations; ++i) {
        auto resp =
            ::make_sentence("!done", fmt::format("=ret=*{:X}", i),
                            fmt::format("=.tag={}", api->current_tag()));
        boost::asio::write(api->socket().input_pipe(),
                           boost::asio::buffer(resp));

        tikpp::tests::fakes::model2 model {};
        model.id = tikpp::data::types::sticky<tikpp::data::types::identity> {i};
        model.sticky_data = tikpp::data::types::sticky<std::string> {
            fmt::format("sticky_data_#{}", i)};
        model.read_only_data = tikpp::data::types::read_only<std::string> {
            fmt::format("read_only_data_#{}", i)};
        model.read_write_data = fmt::format("read_write_data_#{}", i);

        auto req = tikpp::commands::add<tikpp::tests::fakes::model2>(
            api->current_tag(), model);
        req.encode(buf);

        repo.async_add(std::move(model), [&](const auto &err, auto id) {
            static std::size_t current {0};

            EXPECT_FALSE(err);
            EXPECT_EQ(id, current);

            if (++current >= test_iterations) {
                std::vector<std::uint8_t> result {};
                result.resize(buf.size());
                boost::asio::read(api->socket().output_pipe(),
                                  boost::asio::buffer(result));

                for (std::size_t i {0}; i < buf.size(); ++i) {
                    EXPECT_EQ(buf[i], result[i]);
                }

                api->close();
            }
        });
    }

    io.run();
}

TEST_F(RepositoryTests, RemoveTest) {
    constexpr auto test_iterations = 10;

    std::vector<std::uint8_t> buf {};

    for (std::size_t i {0}; i < test_iterations; ++i) {
        auto resp = ::make_sentence(
            "!done", fmt::format("=.tag={}", api->current_tag()));
        boost::asio::write(api->socket().input_pipe(),
                           boost::asio::buffer(resp));

        auto req = tikpp::commands::remove<tikpp::tests::fakes::model2>(
            api->current_tag(), i);
        req.encode(buf);

        repo.async_remove(i, [&](const auto &err) {
            static std::size_t current {0};

            EXPECT_FALSE(err);

            if (++current >= test_iterations) {
                std::vector<std::uint8_t> result {};
                result.resize(buf.size());
                boost::asio::read(api->socket().output_pipe(),
                                  boost::asio::buffer(result));

                for (std::size_t i {0}; i < buf.size(); ++i) {
                    EXPECT_EQ(buf[i], result[i]);
                }

                api->close();
            }
        });
    }

    io.run();
}

TEST_F(RepositoryTests, UpdateTest) {
    constexpr auto test_iterations = 10;

    std::vector<std::uint8_t> buf {};

    for (std::size_t i {0}; i < test_iterations; ++i) {
        auto resp = ::make_sentence(
            "!done", fmt::format("=.tag={}", api->current_tag()));
        boost::asio::write(api->socket().input_pipe(),
                           boost::asio::buffer(resp));

        tikpp::tests::fakes::model2 model {};
        model.id = tikpp::data::types::sticky<tikpp::data::types::identity> {i};
        model.sticky_data = tikpp::data::types::sticky<std::string> {
            fmt::format("sticky_data_#{}", i)};
        model.read_only_data = tikpp::data::types::read_only<std::string> {
            fmt::format("read_only_data_#{}", i)};
        model.read_write_data = fmt::format("read_write_data_#{}", i);

        auto req = tikpp::commands::set<tikpp::tests::fakes::model2>(
            api->current_tag(), model);
        req.encode(buf);

        repo.async_update(std::move(model), [&](const auto &err) {
            static std::size_t current {0};

            EXPECT_FALSE(err);

            if (++current >= test_iterations) {
                std::vector<std::uint8_t> result {};
                result.resize(buf.size());
                boost::asio::read(api->socket().output_pipe(),
                                  boost::asio::buffer(result));

                for (std::size_t i {0}; i < buf.size(); ++i) {
                    EXPECT_EQ(buf[i], result[i]);
                }

                api->close();
            }
        });
    }

    io.run();
}

} // namespace tikpp::tests
