#ifndef TIKPP_MODELS_IP_HOTSPOT_ACTIVE_HPP
#define TIKPP_MODELS_IP_HOTSPOT_ACTIVE_HPP

#include "tikpp/data/model.hpp"
#include "tikpp/data/types/bytes.hpp"
#include "tikpp/data/types/duration.hpp"

#include <cstdint>
#include <string>

namespace tikpp::models::ip::hotspot {

/*!
 * \brief A data model to represent `/ip/hotspot/user' list
 */
struct active : tikpp::data::model {
    static constexpr auto api_path = "/ip/hotspot/active";

    using bytes    = tikpp::data::types::bytes;
    using duration = tikpp::data::types::duration<std::chrono::seconds>;

    /*!
     * \brief HotSpot server name client is logged in.
     */
    read_only<std::string> server;

    /*!
     * \brief Name of the HotSpot user.
     */
    read_only<std::string> user;

    /*!
     * \brief Domain of the user (if split from username), parameter is used
     *        only with RADIUS authentication.
     */
    read_only<std::string> domain;

    /*!
     * \brief IP address of the HotSpot user.
     */
    read_only<std::string> address;

    /*!
     * \brief MAC-address of the HotSpot user.
     */
    read_only<std::string> mac_address;

    /*!
     * \brief Authentication method used by HotSpot client.
     */
    read_only<std::string> login_by;

    /*!
     * \brief Current session time of the user, it is showing how long user has
     *        been logged in.
     */
    read_only<duration> uptime;

    /*!
     * \brief The amount of time user has been idle.
     */
    read_only<duration> idle_time;

    /*!
     * \brief the exact value of session-time, that is applied for user.
     *
     * the exact value of session-time, that is applied for user. Value shows
     * how long user is allowed to be online to be logged of automatically by
     * \see uptime reached.
     */
    read_only<duration> session_time_left;

    /*!
     * \brief The exact value of the user's idle-timeout
     */
    read_only<duration> idle_timeout;

    /*!
     * \brief The exact value of the keepalive-timeout, that is applied for
     *        user.
     *
     *  the exact value of the keepalive-timeout, that is applied for user.
     * Value shows how long host can stay out of reach to be removed from the
     * HotSpot
     */
    read_only<duration> keepalive_timeout;

    /*!
     * \brief Value shows how many bytes received from the client.
     *
     * value shows how many bytes received from the client, option is active
     * when the appropriate parameter is configured for HotSpot user
     */
    read_only<bytes> limit_bytes_in;

    /*!
     * \brief Value shows how many bytes send to the client.
     *
     * value shows how many bytes send to the client, option is active when the
     * appropriate parameter is configured for HotSpot user
     */
    read_only<bytes> limit_bytes_out;

    /*!
     * \brief Value shows how many bytes total were send/received from client.
     *
     * alue shows how many bytes total were send/received from client, option is
     * active when the appropriate parameter is configured for HotSpot user
     */
    read_only<bytes> limit_bytes_total;

    template <typename Converter>
    void convert(Converter &c) {
        model::convert(c);

        c("server", server);
        c("user", user);
        c("domain", domain);
        c("address", address);
        c("mac-address", mac_address);
        c("login-by", login_by);
        c("uptime", uptime);
        c("idle-time", idle_time);
        c("session-time-left", session_time_left);
        c("idle-timeout", idle_timeout);
        c("keepalive-timeout", keepalive_timeout);
        c("limit-bytes-in", limit_bytes_in);
        c("limit-bytes-out", limit_bytes_in);
        c("limit-bytes-total", limit_bytes_in);
    }
};

} // namespace tikpp::models::ip::hotspot

#endif
