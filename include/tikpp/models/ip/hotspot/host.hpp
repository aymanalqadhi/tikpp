#ifndef TIKPP_MODELS_IP_HOTSPOT_HOST_HPP
#define TIKPP_MODELS_IP_HOTSPOT_HOST_HPP

#include "tikpp/data/model.hpp"
#include "tikpp/data/types/bytes.hpp"
#include "tikpp/data/types/duration.hpp"

#include <cstdint>
#include <string>

namespace tikpp::models::ip::hotspot {

/*!
 * \brief A data model to represent `/ip/hotspot/host' list
 */
struct host : tikpp::data::model {
    static constexpr auto api_path = "/ip/hotspot/host";

    using bytes    = tikpp::data::types::bytes;
    using duration = tikpp::data::types::duration<std::chrono::seconds>;

    /*!
     * \brief HotSpot user MAC-address.
     */
    read_only<std::string> mac_address;

    /*!
     * \brief HotSpot client original IP address.
     */
    read_only<std::string> address;

    /*!
     * \brief New client address assigned by HotSpot.
     *
     * New client address assigned by HotSpot, it might be the same as original
     * address
     */
    read_only<std::string> to_address;

    /*!
     * \brief HotSpot server name client is connected to.
     */
    read_only<std::string> server;

    /*!
     * \brief Interface bridge port client connected to.
     *
     * /interface bridge port client connected to, value is unknown when
     * HotSpot is not configured on the bridge
     */
    read_only<std::string> bridge_port;

    /*!
     * \brief Value shows how long user is online (connected to the HotSpot).
     */
    read_only<duration> uptime;

    /*!
     * \brief Time user has been idle.
     */
    read_only<duration> idle_time;

    /*!
     * \brief Value of the client idle-timeout (unauthorized client).
     */
    read_only<duration> idle_timeout;

    /*!
     * \brief keepalive-timeout value of the unauthorized client.
     */
    read_only<duration> keepalive_timeout;

    /*!
     * \brief Amount of bytes received from unauthorized client.
     */
    read_only<bytes> bytes_in;

    /*!
     * \brief Amount of packets received from unauthorized client.
     */
    read_only<std::uint32_t> packets_in;

    /*!
     * \brief Amount of bytes sent to unauthorized client.
     */
    read_only<bytes> bytes_out;

    /*!
     * \brief Amount of packets sent to unauthorized client.
     */
    read_only<std::uint32_t> packets_out;

    template <typename Converter>
    void convert(Converter &c) {
        model::convert(c);

        c("mac-address", mac_address);
        c("address", address);
        c("to-address", to_address);
        c("server", server);
        c("bridge-port", bridge_port);
        c("uptime", uptime);
        c("idle-time", idle_time);
        c("idle-timeout", idle_timeout);
        c("keepalive-timeout", keepalive_timeout);
        c("bytes-in", bytes_in);
        c("packets-in", packets_in);
        c("bytes-out", bytes_out);
        c("packets-out", packets_out);
    }
};

} // namespace tikpp::models::ip::hotspot

#endif
