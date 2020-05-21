#ifndef TIKPP_API_HPP
#define TIKPP_API_HPP

#include "tikpp/basic_api.hpp"

#include <boost/asio/ip/tcp.hpp>

namespace tikpp {

using api = tikpp::basic_api<boost::asio::ip::tcp::socket>;

}

#endif
