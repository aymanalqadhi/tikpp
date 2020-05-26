#include "tikpp/request.hpp"

#include "fmt/format.h"
#include <boost/endian.hpp>

#include <cstdint>
#include <vector>

namespace tikpp {

void request::encode(std::vector<std::uint8_t> &buf) const {
    tikpp::detail::encode_word(command_, buf);
    tikpp::detail::encode_word(fmt::format(".tag={}", tag_), buf);

    for (const auto &[key, value] : words_) {
        tikpp::detail::encode_word(fmt::format("{}={}", key, value), buf);
    }

    buf.push_back(0x00);
}

} // namespace tikpp
