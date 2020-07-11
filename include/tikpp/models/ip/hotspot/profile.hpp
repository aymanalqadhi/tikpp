#ifndef TIKPP_MODELS_IP_HOTSPOT_PROFILE_HPP
#define TIKPP_MODELS_IP_HOTSPOT_PROFILE_HPP

#include "tikpp/data/model.hpp"
#include "tikpp/data/types/duration.hpp"

#include <cstdint>
#include <string>

namespace tikpp::models::ip::hotspot {

/*!
 * \brief A data model to represent `/ip/hotspot' list
 */
struct profile : tikpp::data::model {
    static constexpr auto api_path = "/ip/hotspot/profile";

    using duration = tikpp::data::types::duration<std::chrono::seconds>;

    /*!
     * \brief DNS name of the HotSpot server.
     *
     * DNS name of the HotSpot server (it appears as the location of the login
     * page). This name will automatically be added as a static DNS entry in the
     * DNS cache. Name can affect if Hotspot is automatically detected by client
     * device. For example, iOS devices may not detect Hotspot that has a name
     * which includes ".local"
     */
    read_write<std::string> dns_name;

    /*!
     * \brief IP address of HotSpot service.
     */
    read_write<std::string> hotspot_address;

    /*!
     * \brief Directory name in which HotSpot HTML pages are stored.
     *
     * Directory name in which HotSpot HTML pages are stored (by default hotspot
     * directory). It is possible to specify different directory with modified
     * HTML pages. To change HotSpot login page, connect to the router with FTP
     * and download hotspot directory contents.
     *
     * - v6.31 and older software builds: For devices where "flash" directory is
     * present, hotspot html directory must be stored there and path must be
     * typed in as follows: "/(hotspot_dir)". This must be done in this order as
     * hotspot sees "flash" directory as root location.
     * - v6.32 and newer software builds: full path must be typed in
     * html-directory field, including
     * "/flash/(hotspot_dir)"
     */
    read_write<std::string> html_directory;

    /*!
     * \brief Alternative path for hotspot html files.
     *
     * Alternative path for hotspot html files. It should be used only if
     * customized hotspot html files are stored on external storage(attached
     * usb, hdd, etc). If configured then hotspot will switch to this html path
     * as soon at it becomes available and switch back to html-directory path if
     * override path becomes non-available for some reason.
     */
    read_write<std::string> html_directory_override;

    /*!
     * \brief HTTP cookie validity time, the option is related to cookie HotSpot
     *        login method.
     */
    read_write<duration> http_cookie_lifetime;

    /*!
     * \brief Address and port of the proxy server for HotSpot service.
     *
     * Address and port of the proxy server for HotSpot service, when
     * default value is used all request are resolved by the local /ip proxy
     */
    read_write<std::string> http_proxy;

    /*!
     * \brief Whether to redirect unauthenticated user to hotspot login page.
     *
     * Whether to redirect unauthenticated user to hotspot login page, if he is
     * visiting a https:// url. Since certificate domain name will mismatch,
     * often this leads to errors, so you can set this parameter to "no" and all
     * https requests will simply be rejected and user will have to visit a http
     * page
     */
    read_write<bool> https_redirect;

    /*!
     * \brief Used HotSpot authentication method.
     *
     * Used HotSpot authentication method
     * - mac-cookie: enables login by mac cookie method
     * - cookie: may only be used with other HTTP authentication method. HTTP
     *   cookie is generated, when user authenticates in HotSpot for the first
     *   time. User is not asked for the login/password and authenticated
     *   automatically, until cookie-lifetime is active.
     * - http-chap: login/password is required for the user to authenticate in
     *   HotSpot. CHAP challenge-response method with MD5 hashing algorithm is
     *   used for protecting passwords.
     * - http-pap: login/password is required for user to authenticate
     *   in HotSpot. Username and password are sent over network in plain text.
     * - https: login/password is required for user to authenticate in HotSpot.
     *   Client login/password exchange between client and server is encrypted
     *   with SSL tunnel. HTTPs example.
     * - mac: client is authenticated without asking login form. Client
     *   MAC-address is added to /ip hotspot user database, client is
     *   authenticated as soon as connected to the HotSpot.
     * - trial: client is allowed to use internet without HotSpot login for the
     *   specified amount of time
     */
    read_write<std::string> login_by;

    /*!
     * \brief Used together with MAC authentication.
     *
     * Used together with MAC authentication, field used to specify password for
     * the users to be authenticated by their MAC addresses. The following
     * option is required, when specific RADIUS server rejects authentication
     * for the clients with blank passwor
     */
    read_write<std::string> mac_auth_password;

    /*!
     * \brief Descriptive name of the profile.
     */
    read_write<std::string> name;

    /*!
     * \brief NAS-Port-Type value to be sent to RADIUS server.
     *
     * NAS-Port-Type value to be sent to RADIUS server, NAS-Port-Type values are
     * described in the RADIUS RFC 2865. This optional value attribute indicates
     * the type of the physical port of the HotSpot server.
     */
    read_write<std::string> nas_port_type;

    /*!
     * \brief Send RADIUS server accounting information for each user, when yes
     *        is used.
     */
    read_write<bool> radius_accounting;

    /*!
     * \brief 	Default domain to use for RADIUS requests.
     *
     * 	Default domain to use for RADIUS requests. Allows to use separate RADIUS
     * server per /ip hotspot profile. If used, same domain name should be
     * specified under /radius domain value
     */
    read_write<std::string> radius_default_domain;

    /*!
     * \brief How often to send accounting updates.
     *
     * How often to send accounting updates. When received is set, interim-time
     * is used from RADIUS server. 0s is the same as received
     */
    read_write<std::string> radius_interim_update;

    /*!
     * \brief RADIUS-Location-Id to be sent to RADIUS server.
     *
     * RADIUS-Location-Id to be sent to RADIUS server. Used to identify
     * location of the HotSpot server during the communication with RADIUS
     * server. Value is optional and used together with RADIUS server.
     */
    read_write<std::string> radius_location_name;

    /*!
     * \brief The MAC address format used by RADIUS
     */
    read_write<std::string> radius_mac_format;

    /*!
     * Rate limitation in form of rx-rate[/tx-rate]
     * [rx-burst-rate[/tx-burst-rate] [rx-burst-threshold[/tx-burst-threshold]
     * [rx-burst-time[/tx-burst-time]]]] [priority] [rx-rate-min[/tx-rate-min]]
     * from the point of view of the router (so "rx" is client upload, and "tx"
     * is client download). All rates should be numbers with optional 'k'
     * (1,000s) or 'M' (1,000,000s). If tx-rate is not specified, rx-rate is as
     * tx-rate too. Same goes for tx-burst-rate and tx-burst-threshold and
     * tx-burst-time. If both rx-burst-threshold and tx-burst-threshold are not
     * specified (but burst-rate is specified), rx-rate and tx-rate is used as
     * burst thresholds. If both rx-burst-time and tx-burst-time are not
     * specified, 1s is used as default. rx-rate-min and tx-rate min are the
     * values of limit-at properties
     *
     */
    read_write<std::string> rate_limit;

    /*!
     * \brief SMTP server address to be used to redirect HotSpot users SMTP
     *        requests.
     */
    read_write<std::string> smtp_server;

    /*!
     * \brief Split username from domain name when the username is given in
     *        "user@domain" or in "domain\user" format from RADIUS server.
     */
    read_write<bool> split_user_domain;

    /*!
     * \brief Name of the SSL certificate on the router to to use only for
     *        HTTPS authentication.
     */
    read_write<std::string> ssl_certificate;

    /*!
     * \brief Used only with trial authentication method.
     *
     * 	Used only with trial authentication method. First time value specifies,
     * how long trial user identified by MAC address can use access to public
     * networks without HotSpot authentication. Second time value specifies
     * amount of time, that has to pass until user is allowed to use trial
     * again.
     */
    read_write<duration> trial_uptime;

    /*!
     * \brief Specifies hotspot user profile for trial users.
     */
    read_write<std::string> trial_user_profile;

    /*!
     * \brief Use RADIUS to authenticate HotSpot users.
     */
    read_write<bool> use_radius;

    template <typename Converter>
    void convert(Converter &c) {
        tikpp::data::model::convert(c);

        using namespace tikpp::data::types::literals;

        c("dns-name", dns_name, empty_string);
        c("hotspot-address", hotspot_address, "0.0.0.0");
        c("html-directory", html_directory, "hotspot");
        c("html-directory-override", html_directory_override);
        c("http-cookie-lifetime", http_cookie_lifetime, 3_d);
        c("http-proxy", http_proxy, "0.0.0.0:0");
        c("https-redirect", https_redirect, true);
        c("login-by", login_by, "http-chap, cookie");
        c("mac-auth-password", mac_auth_password, empty_string);
        c("name", name, empty_string);
        c("name", name, empty_string);
        c("nas-port-type", nas_port_type, "wireless-802.11");
        c("radius-accounting", radius_accounting, true);
        c("radius-default-domain", radius_default_domain, empty_string);
        c("radius-interim-update", radius_interim_update, "received");
        c("radius-location-name", radius_location_name, empty_string);
        c("radius-mac-format", radius_mac_format, "XX:XX:XX:XX:XX:XX");
        c("rate-limit", rate_limit, empty_string);
        c("smtp-server", smtp_server, "0.0.0.0");
        c("split-user-domain", split_user_domain, false);
        c("ssl-certificate", ssl_certificate);
        c("trial-uptime", trial_uptime, "30m/1d");
        c("trial-user-profile", trial_user_profile, "default");
        c("use-radius", use_radius, false);
    }
};

} // namespace tikpp::models::ip::hotspot

#endif
