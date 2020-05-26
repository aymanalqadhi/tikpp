#ifndef TIKPP_API_HPP
#define TIKPP_API_HPP

#include "tikpp/basic_api.hpp"

#include <boost/asio/ip/tcp.hpp>

namespace tikpp {

template <typename ErrorHandler, api_version version = api_version::v1>
using api =
    tikpp::basic_api<boost::asio::ip::tcp::socket, ErrorHandler, version>;

}

#endif
