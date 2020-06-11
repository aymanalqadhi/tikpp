#include "tikpp/tests/fakes/socket.hpp"
#include "tikpp/tests/fixtures/socket.hpp"

#include "tikpp/detail/operations/async_read_word_length.hpp"
#include "tikpp/request.hpp"

#include "gtest/gtest.h"
#include <boost/asio/buffer.hpp>
#include <boost/asio/write.hpp>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

namespace tikpp::tests {

struct AsyncReadWordLengthTest : tikpp::tests::fixtures::SocketTest {};

TEST_F(AsyncReadWordLengthTest, LengthTest) {
    constexpr auto test_iterations = 100;

    std::vector<std::uint8_t> buf {};
    std::time_t               start_seed {std::time(nullptr)};
    std::srand(start_seed);

    for (std::size_t i {0}; i < test_iterations; ++i) {
        tikpp::detail::encode_length(std::rand(), buf);
    }

    boost::asio::write(sock.input_pipe(), boost::asio::buffer(buf));
    std::srand(start_seed);

    for (std::size_t i {0}; i < test_iterations; ++i) {
        tikpp::detail::operations::async_read_word_length(
            sock, [](const auto &err, auto len) {
                EXPECT_FALSE(err);
                EXPECT_EQ(rand(), len);
            });
    }

    io.run();
}

} // namespace tikpp::tests
