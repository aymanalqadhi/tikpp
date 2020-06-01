#ifndef TIKPP_SENTENCE_HPP
#define TIKPP_SENTENCE_HPP

#include "tikpp/detail/convert.hpp"

#include <cassert>
#include <string>
#include <unordered_map>

namespace tikpp {

struct sentence {
    using words_map = std::unordered_map<std::string, std::string>;

    [[nodiscard]] inline auto begin() const noexcept
        -> words_map::const_iterator {
        return words_.begin();
    }

    [[nodiscard]] inline auto end() const noexcept
        -> words_map::const_iterator {
        return words_.end();
    }

    [[nodiscard]] inline auto find(const std::string &key) noexcept
        -> words_map::iterator {
        return words_.find(key);
    }

    [[nodiscard]] inline auto contains(const std::string &key) const noexcept
        -> bool {
        return words_.find(key) != words_.end();
    }

    [[nodiscard]] inline auto size() const noexcept -> std::size_t {
        return words_.size();
    }

    [[nodiscard]] inline auto empty() const noexcept -> bool {
        return words_.empty();
    }

    [[nodiscard]] inline auto operator[](const std::string &key) noexcept
        -> std::string & {
        return words_[key];
    }

    template <typename T>
    [[nodiscard]] inline auto get(const std::string &key) const noexcept -> T {
        assert(contains(key));
        return tikpp::detail::convert<T>(words_.at(key));
    }

  protected:
    words_map words_;
};

} // namespace tikpp

#endif
