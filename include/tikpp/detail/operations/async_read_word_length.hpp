#ifndef TIKPP_DETAIL_OPERATIONS_ASYNC_READ_WORD_LENGTH_HPP
#define TIKPP_DETAIL_OPERATIONS_ASYNC_READ_WORD_LENGTH_HPP

#include "tikpp/detail/async_result.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/read.hpp>
#include <boost/system/error_code.hpp>

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <vector>

namespace tikpp::detail::operations {

template <typename AsyncReadStream, typename Handler>
struct async_read_word_length_op final {
    enum class read_step { reading_first_byte, reading_missing_bytes };

    inline async_read_word_length_op(AsyncReadStream &sock, Handler &&handler)
        : sock_ {sock},
          handler_ {std::forward<Handler>(handler)},
          length_ {0},
          missing_bytes_ {0},
          state_ {read_step::reading_first_byte} {
        buf_.resize(4);
    }

    void operator()(const boost::system::error_code &err, std::size_t rx) {
        if (err) {
            return handler_(err, 0);
        }

        if (state_ == read_step::reading_first_byte) {
            assert(rx == 1);
            on_reading_first_byte();
        } else {
            assert(rx == missing_bytes_);
            assert(state_ == read_step::reading_missing_bytes);
            on_reading_missing_bytes();
        }
    }

    inline void on_reading_first_byte() {
        const auto &first_byte {buf_[0]};

        if (first_byte == 0) {
            return handler_(boost::system::error_code {}, 0);
        }

        if ((first_byte & 0x80) != 0x00) {
            if ((first_byte & 0xC0) == 0x80) {
                length_        = first_byte & ~0xC0;
                missing_bytes_ = 1;
            } else if ((first_byte & 0xE0) == 0xC0) {
                length_        = first_byte & ~0xE0;
                missing_bytes_ = 2;
            } else if ((first_byte & 0xF0) == 0xE0) {
                length_        = first_byte & ~0xF0;
                missing_bytes_ = 3;
            } else if (first_byte == 0xF0) {
                missing_bytes_ = 4;
            } else {
                return handler_(boost::asio::error::message_size, 0);
            }

            state_ = read_step::reading_missing_bytes;
            boost::asio::async_read(sock_,
                                    boost::asio::buffer(buf_, missing_bytes_),
                                    std::move(*this));
        } else {
            handler_(boost::system::error_code {}, buf_[0]);
        }
    }

    inline void on_reading_missing_bytes() {
        for (std::size_t i {0}; i < missing_bytes_; ++i) {
            length_ = (length_ << 8) | (buf_[i] & 0xFF);
        }
        handler_(boost::system::error_code {}, length_);
    }

    inline void initiate() {
        boost::asio::async_read(sock_, boost::asio::buffer(buf_, 1),
                                std::move(*this));
    }

  private:
    AsyncReadStream &sock_;
    Handler          handler_;

    std::uint32_t length_;
    std::uint8_t  missing_bytes_;
    read_step     state_;

    std::vector<std::uint8_t> buf_;
};

template <typename AsyncReadStream, typename CompletionToken>
decltype(auto) async_read_word_length(AsyncReadStream & sock,
                                      CompletionToken &&token) {
    GENERATE_COMPLETION_HANDLER(
        void(const boost::system::error_code &, std::uint32_t), token, handler,
        result);

    async_read_word_length_op<AsyncReadStream, handler_type> {
        sock, std::move(handler)}
        .initiate();

    return result.get();
}

} // namespace tikpp::detail::operations

#endif
