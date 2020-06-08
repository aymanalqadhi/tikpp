#include "tikpp/tests/fakes/socket.hpp"
#include "tikpp/tests/fixtures/socket.hpp"

#include "tikpp/detail/operations/async_read_response.hpp"
#include "tikpp/request.hpp"

#include "fmt/format.h"
#include "gtest/gtest.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace tikpp::tests {

struct AsyncReadWordLengthTest : tikpp::tests::fixtures::SocketTest {};

TEST_F(AsyncReadWordLengthTest, SentenceTest) {
    static constexpr auto test_iterations    = 100;
    static constexpr auto words_per_sentence = 10;

    std::time_t start_seed {std::time(nullptr)};
    std::srand(start_seed);

    for (std::size_t i {0}; i < test_iterations; ++i) {
        std::vector<std::uint8_t> buf {};

        auto req = std::make_shared<tikpp::request>("!re", rand());

        for (std::size_t j {0}; j < words_per_sentence; ++j) {
            req->add_param(fmt::format("param_{}", rand()),
                           fmt::format("value_{}", rand()));
        }

        for (std::size_t j {0}; j < words_per_sentence; ++j) {
            req->add_attribute(fmt::format("attr_{}", rand()),
                               fmt::format("value_{}", rand()));
        }

        req->encode(buf);
        sock.input_buffer().write(reinterpret_cast<char *>(buf.data()),
                                  buf.size());
    }

    std::srand(start_seed);

    for (std::size_t i {0}; i < test_iterations; ++i) {
        tikpp::detail::operations::async_read_response(
            sock,
            [](const boost::system::error_code &err, auto &&resp) mutable {
                EXPECT_EQ(resp.type(), tikpp::response_type::data);
                EXPECT_EQ(resp.size(), words_per_sentence * 2);
                EXPECT_TRUE(resp.tag().has_value());
                EXPECT_EQ(resp.tag().value(), rand());

                for (std::size_t i {0}; i < words_per_sentence; ++i) {
                    auto key = fmt::format("param_{}", rand());
                    EXPECT_TRUE(resp.contains(key));
                    EXPECT_EQ(resp[key], fmt::format("value_{}", rand()));
                }

                for (std::size_t i {0}; i < words_per_sentence; ++i) {
                    auto key = fmt::format(".attr_{}", rand());
                    EXPECT_TRUE(resp.contains(key));
                    EXPECT_EQ(resp[key], fmt::format("value_{}", rand()));
                }

                EXPECT_FALSE(err);
            });

        io.restart();
        io.run();
    }
}

} // namespace tikpp::tests
