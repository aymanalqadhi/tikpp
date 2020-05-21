#ifndef TIKPP_REQUEST_HPP
#define TIKPP_REQUEST_HPP

#include "tikpp/sentence.hpp"

#include "fmt/format.h"

#include <cstdint>
#include <string>
#include <vector>

namespace tikpp {

struct request : sentence {
    request(std::string command, std::uint32_t tag)
        : command_ {std::move(command)}, tag_ {tag} {
    }

    template <typename... TArgs>
    inline void
    add_word(std::string key, const char *fmt, const TArgs &... args) {
        words_.emplace(
            std::make_pair(std::move(key), fmt::format(fmt, args...)));
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
