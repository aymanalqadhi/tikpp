#ifndef TIKPP_MODELS_IP_HOTSPOT_USER_PROFILE_HPP
#define TIKPP_MODELS_IP_HOTSPOT_USER_PROFILE_HPP

#include "tikpp/data/model.hpp"
#include "tikpp/data/types/bytes.hpp"
#include "tikpp/data/types/duration.hpp"

#include <chrono>
#include <string>

/*!
 * \brief A data model to represent `/ip/hotspot/user/profile' list
 */
namespace tikpp::models::ip::hotspot {

struct user_profile_detail : tikpp::data::model {
    static constexpr auto api_path = "/ip/hotspot/user/profile";

    using duration = tikpp::data::types::duration<std::chrono::seconds>;

    /*!
     * \brief Name of the address list in which users IP address will be added.
     *
     * Name of the address list in which users IP address will be added.  Useful
     * to mark traffic per user groups for queue tree configurations.
     */
    read_write<std::string> address_list;

    /*!
     * \brief IP pool name from which the user will get IP.
     *
     * IP pool name from which the user will get IP.  When user has improper
     * network settings configuration on the computer, HotSpot server makes
     * translation and assigns correct IP address from the pool instead of
     * incorrect one
     */
    read_write<std::string> address_pool;

    /*!
     * \brief Enable forced advertisement popups.
     *
     * Enable forced advertisement popups.  After certain interval specific
     * web-page is being displayed for HotSpot users. Advertisement page might
     * be blocked by browsers popup blockers
     */
    read_write<bool> advertise;

    /*!
     * \brief Set of intervals between advertisement popups.
     *
     * Set of intervals between advertisement popups. After the list is done,
     * the last value is used for all further advertisements, 10 minutes
     */
    read_write<std::string> advertise_interval;

    /*!
     * \brief How long advertisement is shown, before blocking network access
     *        for HotSpot client.
     *
     * How long advertisement is shown, before blocking network access for
     * HotSpot client. Connection to Internet is not allowed, when advertisement
     * is not shown.
     */
    read_write<std::string> advertise_timeout;

    /*!
     * \brief List of URLs that is show for advertisement popups.
     *
     * List of URLs that is show for advertisement popups. After the last URL is
     * used, list starts from the begining.
     */
    read_write<std::string> advertise_url;

    /*!
     * \brief Maximal period of inactivity for authorized HotSpot clients.
     *
     * Maximal period of inactivity for authorized HotSpot clients. Timer is
     * counting, when there is no traffic coming from that client and going
     * through the router, for example computer is switched off. User is logged
     * out, dropped of the host list, the address used by the user is freed,
     * when timeout is reached.
     */
    read_write<duration> idle_timeout;

    /*!
     * \brief Name of the firewall chain applied to incoming packets from the
     *        users of this profile.
     *
     * Name of the firewall chain applied to incoming packets from the users of
     * this profile, jump rule is required from built-in chain (input, forward,
     * output) to chain=hotspot
     */
    read_write<std::string> incoming_filter;

    /*!
     * \brief Packet mark put on incoming packets from every user of this
     *        profile.
     */
    read_write<std::string> incoming_packet_mark;

    /*!
     * \brief Keepalive timeout for authorized HotSpot clients.
     *
     * Keepalive timeout for authorized HotSpot clients. Used to detect, that
     * the computer of the client is alive and reachable. User is logged out,
     * when timeout value is reached
     */
    read_write<std::string> keepalive_timeout;

    /*!
     * \brief Descriptive name of the profile.
     */
    read_write<std::string> name;

    /*!
     * \brief Script name to be executed, when user logs in to the HotSpot
     *        from the particular profile.
     *
     * Script name to be executed, when user logs in to the HotSpot from the
     * particular profile. It is possible to get username from internal user
     * and
     * interface variable. For example, :log info "User $user logged in!" . If
     * hotspot
     * is set on bridge interface, then interface variable will show bridge as
     * actual
     * interface unless use-ip-firewall' is set in bridge settings.
     *
     * List of available variables:
     * - $user
     * - $username (alternative var name for $user)
     * - $address
     * - $"mac-address"
     * - $interface
     */
    read_write<std::string> on_login;

    /*!
     * \brief Script name to be executed, when user logs out from the HotSpot.
     *
     * Script name to be executed, when user logs out from the HotSpot.It is
     * possible to get username from internal user and interface variable. For
     * example, :log info "User $user logged in!" . If hotspot is set on bridge
     * interface, then interface variable will show bridge as actual interface
     * unless use-ip-firewall is set in bridge settings.
     *
     * List of available variables:
     * - $user
     * - $username (alternative var name for $user)
     * - $address
     * - $"mac-address"
     * - $interface
     * - $cause
     *
     * Starting with v6.34rc11 some additional variables are available:
     * - $uptime-secs - final session time in seconds
     * - $bytes-in - bytes uploaded
     * - $bytes-out - bytes downloaded
     * - $bytes-total - bytes up + bytes down
     * - $packets-in - packets uploaded
     * - $packets-out - packets downloaded
     * - $packets-total - packets up + packets down
     */
    read_write<std::string> on_logout;

    /*!
     * \brief Option to show status page for user authenticated with mac login
     *        method.
     *
     * Option to show status page for user authenticated with mac login method.
     * For example to show advertisement on status page (alogin.html)
     * - http-login: open status page only for HTTP login (includes cookie and
     *               HTTPS)
     * - always: open HTTP status page in case of mac login as well
     */
    read_write<std::string> open_status_page;

    /*!
     * \brief Name of the firewall chain applied to outgoing packets from the
     *        users of this profile.
     *
     * Name of the firewall chain applied to outgoing packets from the users of
     * this profile, jump rule is required from built-in chain (input, forward,
     * output) to chain=hotspot
     */
    read_write<std::string> outgoing_filter;

    /*!
     * \brief Packet mark put on outgoing packets from every user of this
     *        profile
     */
    read_write<std::string> outgoing_packet_mark;

    /*!
     * \brief Simple dynamic queue is created for user, once it logs in to the
     *        HotSpot.
     *
     * Simple dynamic queue is created for user, once it logs in to the
     * HotSpot. Rate-limitation is configured in the following form
     * [rx-rate[/tx-rate] [rx-burst-rate[/tx-burst-rate]
     * [rx-burst-threshold[/tx-burst-threshold] [rx-burst-time[/tx-burst-time]
     * [priority] [rx-rate-min[/tx-rate-min]]]].
     * For example, to set 1M
     * download, 512k upload for the client, rate-limit=512k/1M
     */
    read_write<std::string> rate_limit;

    /*!
     * \brief Allowed session time for client. After this time, the user is
     *        logged out unconditionally.
     */
    read_write<duration> session_timeout;

    /*!
     * \brief Allowed number of simultaneously logged in users with the same
     *        HotSpot username.
     */
    read_write<std::uint32_t> shared_users;

    /*!
     * \brief HotSpot status page autorefresh interval.
     */
    read_write<duration> status_autorefresh;

    /*!
     * \brief Use transparent HTTP proxy for the authorized users of this
     *        profile
     */
    read_write<bool> transparent_proxy;

    template <typename Converter>
    inline void convert(Converter &c) {
        using namespace tikpp::data::types::literals;

        tikpp::data::model::convert(c);

        c("address-list", address_list, empty_string);
        c("address-pool", address_pool);
        c("advertise", advertise, false);
        c("advertise-interval", advertise_interval, "30m,10m");
        c("advertise-timeout", advertise_timeout, "1m");
        c("advertise-url", advertise_url, empty_string);
        c("idle-timeout", idle_timeout);
        c("incoming-filter", incoming_filter, empty_string);
        c("incoming-packet-mark", incoming_packet_mark, empty_string);
        c("keepalive-timeout", keepalive_timeout, empty_string);
        c("name", name, empty_string);
        c("on-login", on_login, empty_string);
        c("on-logout", on_logout, empty_string);
        c("open-status-page", open_status_page, "always");
        c("outgoing-filter", outgoing_filter, empty_string);
        c("outgoing-packet-mark", outgoing_packet_mark, empty_string);
        c("rate-limit", rate_limit, empty_string);
        c("shared-users", shared_users, 1);
        c("session-timeout", session_timeout, 0_s);
        c("status-autorefresh", status_autorefresh);
        c("transparent-proxy", transparent_proxy, true);
    }
};

} // namespace tikpp::models::ip::hotspot

#endif
