#ifndef TIKPP_UTIL_HPP
#define TIKPP_UTIL_HPP

#include <string>

namespace tikpp {

auto hash_password(const std::string &plain, const std::string &cha)
    -> std::string;

} // namespace tikpp

#endif
