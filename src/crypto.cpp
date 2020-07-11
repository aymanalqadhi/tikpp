#include "tikpp/detail/crypto.hpp"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include <openssl/md5.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace {

constexpr auto md5_size           = 16;
constexpr auto challenge_str_size = md5_size * 2;

constexpr auto digit_value(std::uint8_t digit) -> std::uint8_t {
    if (digit >= '0' && digit <= '9') {
        return digit - '0';
    }

    digit &= ~0x20;

    if (digit >= 'A' && digit <= 'F') {
        return digit - 'A' + 10;
    }

    throw std::runtime_error {"Invalid hex string"};
}

template <std::size_t size>
inline void decode_hex_string(const std::string &             hex_str,
                              std::array<std::uint8_t, size> &outbuf) {
    if (hex_str.size() < size * 2) {
        throw std::out_of_range {"Too short string"};
    }

    for (std::size_t i {0}; i < size * 2; i += 2) {
        outbuf[i / 2] =
            (digit_value(hex_str[i]) << 4) | digit_value(hex_str[i + 1]);
    }
}

template <std::size_t size>
inline void encode_hex_string(const std::array<std::uint8_t, size> &buf,
                              std::string &                         outstr) {
    constexpr std::array<char, 16> hex_chars {'0', '1', '2', '3', '4', '5',
                                              '6', '7', '8', '9', 'a', 'b',
                                              'c', 'd', 'e', 'f'};

    outstr.resize(size * 2);

    for (std::size_t i {0}; i < size; ++i) {
        outstr[i * 2]       = hex_chars[(buf[i] & 0xF0) >> 4];
        outstr[(i * 2) + 1] = hex_chars[buf[i] & 0x0F];
    }
}

} // namespace

namespace tikpp::detail {

auto hash_password(const std::string &plain, const std::string &cha)
    -> std::string {
    static std::uint8_t empty_byte {0};

    if (cha.size() != ::challenge_str_size) {
        throw std::out_of_range {"Invalid challenge size"};
    }

    std::string                        tmp {};
    std::array<std::uint8_t, md5_size> cha_bytes {};
    std::array<std::uint8_t, md5_size> digest_bytes {};

    decode_hex_string(cha, cha_bytes);

    MD5_CTX ctx;

    if (!MD5_Init(&ctx)) {
        throw std::runtime_error {"Could not create an OpenSSL MD5 context"};
    }

    MD5_Update(&ctx, &empty_byte, 1);
    MD5_Update(&ctx, plain.data(), plain.size());
    MD5_Update(&ctx, cha_bytes.data(), cha_bytes.size());
    MD5_Final(digest_bytes.data(), &ctx);

    encode_hex_string(digest_bytes, tmp);

    return tmp;
}

} // namespace tikpp::detail
