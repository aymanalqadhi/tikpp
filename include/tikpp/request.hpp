#ifndef TIKPP_REQUEST_HPP
#define TIKPP_REQUEST_HPP

#include "tikpp/sentence.hpp"

#include "fmt/format.h"

#include <cstdint>
#include <string>
#include <vector>

namespace tikpp {

namespace detail {

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

} // namespace detail

struct request : sentence {
    request(std::string command, std::uint32_t tag)
        : command_ {std::move(command)}, tag_ {tag} {
    }

    inline void add_word(std::string key, std::string value) {
        words_.emplace(std::make_pair(std::move(key), std::move(value)));
    }

    template <typename... TArgs>
    inline void
    add_word(std::string key, const char *fmt, const TArgs &... args) {
        add_word(std::move(key), fmt::format(fmt, args...));
    }

    inline void add_param(std::string key, std::string value) {
        words_.emplace(std::make_pair(fmt::format("={}", std::move(key)),
                                      std::move(value)));
    }

    inline void add_attribute(std::string key, std::string value) {
        words_.emplace(std::make_pair(fmt::format(".{}", std::move(key)),
                                      std::move(value)));
    }

    template <typename T>
    inline void add_param(std::string key, T value) {
        add_param(std::move(key), fmt::to_string(value));
    }

    template <typename T>
    inline void add_attribute(std::string key, T value) {
        add_attribute(std::move(key), fmt::to_string(value));
    }

    [[nodiscard]] inline auto command() const noexcept -> const std::string & {
        return command_;
    }

    [[nodiscard]] inline auto tag() const noexcept -> std::uint32_t {
        return tag_;
    }

    void encode(std::vector<std::uint8_t> &buf) const;

  protected:
    std::string   command_;
    std::uint32_t tag_;
};

} // namespace tikpp

#endif
