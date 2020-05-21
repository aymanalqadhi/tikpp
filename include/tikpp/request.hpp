#ifndef TIKPP_REQUEST_HPP
#define TIKPP_REQUEST_HPP

#include "fmt/format.h"

#include <boost/lexical_cast.hpp>

#include <cassert>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace tikpp {

struct request {
    using words_map = std::unordered_map<std::string, std::string>;

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

    [[nodiscard]] inline auto begin() const noexcept
        -> words_map::const_iterator {
        return words_.begin();
    }

    [[nodiscard]] inline auto end() const noexcept
        -> words_map::const_iterator {
        return words_.end();
    }

    [[nodiscard]] inline auto contains(const std::string &key) const noexcept
        -> bool {
        return words_.find(key) != words_.end();
    }

    [[nodiscard]] inline auto size() const noexcept -> std::size_t {
        return words_.size();
    }

    [[nodiscard]] inline auto operator[](const std::string &key) const noexcept
        -> const std::string & {
        assert(contains(key));

        return words_.at(key);
    }

    template <typename T>
    [[nodiscard]] inline auto get(const std::string &key) const noexcept -> T {
        assert(contains(key));
        return boost::lexical_cast<T>(words_.at(key));
    }

    void encode(std::vector<std::uint8_t> &buf) const;

  protected:
    std::string   command_;
    std::uint32_t tag_;

    words_map words_;
};

} // namespace tikpp

#endif
