#include "tikpp/tests/fakes/socket.hpp"
#include "tikpp/tests/fixtures/socket.hpp"

#include "tikpp/detail/operations/async_read_word_length.hpp"
#include "tikpp/request.hpp"

#include "gtest/gtest.h"

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

    sock.input_buffer().write(reinterpret_cast<char *>(buf.data()), buf.size());
    std::srand(start_seed);

    for (std::size_t i {0}; i < test_iterations; ++i) {
        tikpp::detail::operations::async_read_word_length(
            sock, [](const auto &err, auto len) {
                EXPECT_FALSE(err);
                EXPECT_EQ(rand(), len);
            });

        io.restart();
        io.run();
    }
}

} // namespace tikpp::tests
