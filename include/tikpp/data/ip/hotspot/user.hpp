#ifndef TIKPP_DATA_IP_HOTSPOT_USER_HPP
#define TIKPP_DATA_IP_HOTSPOT_USER_HPP

#include "tikpp/data/model.hpp"
#include "tikpp/data/types/bytes.hpp"
#include "tikpp/data/types/duration.hpp"

#include <chrono>
#include <cstdint>
#include <string>

namespace tikpp::data::ip::hotspot {

struct user : tikpp::data::model {
    static constexpr auto api_path = "/ip/hotspot/user";

    using bytes    = tikpp::data::types::bytes;
    using duration = tikpp::data::types::duration<std::chrono::seconds>;

    /*!
     * \brief HotSpot login page username
     *
     * HotSpot login page username, when MAC-address authentication is used
     * name is configured as client's MAC-address
     */
    read_write<std::string> name;

    /*!
     * \brief Whether the user is currently disabled or not
     */
    read_write<bool> is_disabled;

    /*!
     * \brief The user password
     */
    read_write<std::string> password;

    /*!
     * \brief User profile configured in /ip hotspot user profile
     */
    read_write<std::string> profile;

    /*!
     * \brief descriptive information for HotSpot user
     *
     * descriptive information for HotSpot user, it might be used for scripts
     * to change parameters for specific clients
     */
    read_write<std::string> comment;

    /*!
     * \brief The number of bytes that the user has recieved
     */
    read_only<bytes> bytes_in;

    /*!
     * \brief The number of bytes that the user has sent
     */
    read_only<bytes> bytes_out;

    /*!
     * \brief The number of packets that the user has recieved
     */
    read_only<std::uint32_t> packets_in;

    /*!
     * \brief The number of packets that the user has sent
     */
    read_only<std::uint32_t> packets_out;

    /*!
     * \brief The time that the user has spent online
     */
    read_only<duration> uptime;

    /*!
     * \brief The IP address that the user will get
     *
     * IP address, when specified client will get the address from the HotSpot
     * one-to-one NAT translations
     *
     * Note: Address does not restrict HotSpot login only
     * from this address
     */
    read_write<std::string> address;

    /*!
     * \brief HotSpot client's e-mail, informational value for the HotSpot
     *        user
     */
    read_write<std::string> email;

    /*!
     * \brief  Maximal amount of bytes that can be received from the user
     *
     * Maximal amount of bytes that can be received from the user
     * User is disconnected from HotSpot after the limit is reached.
     */
    read_write<bytes> limit_bytes_in;

    /*!
     * \brief  Maximal amount of bytes that can be transmitted from the user
     *
     * Maximal amount of bytes that can be transmitted from the user
     * User is disconnected from HotSpot after the limit is reached.
     */
    read_write<bytes> limit_bytes_out;

    /*!
     * \brief \see limit_bytes_in + limit_bytes_out
     *
     * (limit-bytes-in+limit-bytes-out)
     * User is disconnected from HotSpot after the limit is reached.
     */
    read_write<bytes> limit_bytes_total;

    /*!
     * \brief Uptime limit for the HotSpot client
     *
     * Uptime limit for the HotSpot client, user is disconnected from HotSpot as
     * soon as uptime is reached.
     */
    read_write<duration> limit_uptime;

    /*!
     * \brief Client is allowed to login only from the specified MAC-address
     *
     * Client is allowed to login only from the specified MAC-address
     * If value is 00:00:00:00:00:00, any mac address is allowed.
     */
    read_write<std::string> mac_address;

    /*!
     * \brief Routes added to HotSpot gateway when client is connected
     *
     * Routes added to HotSpot gateway when client is connected
     * The route format dst-address gateway metric (for example, 192.168.1.0/24
     * 192.168.0.1 1)
     */
    read_write<std::string> routes;

    /*!
     * \brief HotSpot server's name to which user is allowed login
     */
    read_write<std::string> server;

    template <typename Converter>
    inline void convert(Converter &c) {
        tikpp::data::model::convert(c);

        c("name") % name;
        c("disabled") % is_disabled;
        c("password") % password;
        c("profile") % profile;
        c("comment") % comment;

        c("bytes-in") % bytes_in;
        c("bytes-out") % bytes_out;
        c("packets-in") % packets_in;
        c("packets-out") % packets_out;
        c("uptime") % uptime;

        c("address") % address;
        c("email") % email;
        c("limit-bytes-in") % limit_bytes_in;
        c("limit-bytes-out") % limit_bytes_out;
        c("limit-bytes-total") % limit_bytes_total;
        c("limit-uptime") % limit_uptime;
        c("mac-address") % mac_address;
        c("routes") % routes;
        c("server") % server;
    }
};

} // namespace tikpp::data::ip::hotspot

#endif
