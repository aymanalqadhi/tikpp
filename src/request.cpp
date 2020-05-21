#include "tikpp/request.hpp"

#include "fmt/format.h"
#include <boost/endian.hpp>

#include <algorithm>
#include <cstdint>
#include <vector>

namespace {

template <typename T, std::size_t size = sizeof(T)>
inline void append_uint(const T &val, std::vector<std::uint8_t> &buf) {
    static_assert(size >= 1 && size <= 4 && size <= sizeof(T));

    buf.push_back((val >> (8 * (size - 1))) & 0xFF);
    if constexpr (size - 1 > 0) {
        append_uint<T, size - 1>(val, buf);
    }
}

inline void encode_length(std::size_t len, std::vector<std::uint8_t> &buf) {
    if (len < 0x80) {
        append_uint<decltype(len), 1>(len, buf);
    } else if (len < 0x4000) {
        append_uint<decltype(len), 2>(len | 0x00008000, buf);
    } else if (len < 0x200000) {
        append_uint<decltype(len), 3>(len | 0x00C00000, buf);
    } else if (len < 0x10000000) {
        append_uint<decltype(len), 4>(len | 0xE0000000, buf);
    } else {
        append_uint<std::uint8_t>(0xF0, buf);
        append_uint<decltype(len), 4>(len, buf);
    }
}

inline void encode_word(std::string word, std::vector<std::uint8_t> &buf) {
    encode_length(word.size(), buf);
    std::copy(word.begin(), word.end(), std::back_inserter(buf));
}

} // namespace

namespace tikpp {

void request::encode(std::vector<std::uint8_t> &buf) const {
    ::encode_word(command_, buf);
    ::encode_word(fmt::format(".tag={}", tag_), buf);

    for (const auto &[key, value] : words_) {
        ::encode_word(fmt::format("{}={}", key, value), buf);
    }

    buf.push_back(0x00);
}

} // namespace tikpp
