#include "tikpp/tests/fakes/socket.hpp"
#include "tikpp/tests/fixtures/socket.hpp"

#include "tikpp/detail/operations/async_read_word.hpp"
#include "tikpp/request.hpp"

#include "fmt/format.h"
#include "gtest/gtest.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

namespace tikpp::tests {

struct AsyncReadWordLengthTest : tikpp::tests::fixtures::socket {};

TEST_F(AsyncReadWordLengthTest, WordTest) {
    static constexpr auto test_iterations    = 100;
    static constexpr auto test_string_format = "test_string_{:X}";

    static auto make_test_str = [](auto format) {
        return fmt::format(format, rand());
    };

    std::vector<std::uint8_t> buf {};
    std::time_t               start_seed {std::time(nullptr)};
    std::srand(start_seed);

    for (std::size_t i {0}; i < test_iterations; ++i) {
        tikpp::detail::encode_word(make_test_str(test_string_format), buf);
    }

    sock.input_buffer().write(reinterpret_cast<char *>(buf.data()), buf.size());
    std::srand(start_seed);

    for (std::size_t i {0}; i < test_iterations; ++i) {
        tikpp::detail::operations::async_read_word(
            sock, [](const auto &err, auto &&word) {
                EXPECT_FALSE(err);
                ASSERT_EQ(word, make_test_str(test_string_format));
            });

        io.restart();
        io.run();
    }
}

} // namespace tikpp::tests
