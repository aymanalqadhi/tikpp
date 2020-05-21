#ifndef TIKPP_SENTENCE_HPP
#define TIKPP_SENTENCE_HPP

#include <boost/lexical_cast.hpp>

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

  protected:
    words_map words_;
};

} // namespace tikpp

#endif
