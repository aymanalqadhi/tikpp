#ifndef TIKPP_DETAIL_CRYPTO_HPP
#define TIKPP_DETAIL_CRYPTO_HPP

#include <string>

namespace tikpp::detail {

auto hash_password(const std::string &plain, const std::string &cha)
    -> std::string;

} // namespace tikpp::detail

#endif
