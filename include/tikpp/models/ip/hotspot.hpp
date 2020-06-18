#ifndef TIKPP_MODELS_IP_HOTSPOT_HPP
#define TIKPP_MODELS_IP_HOTSPOT_HPP

#include "tikpp/data/model.hpp"
#include "tikpp/data/types/duration.hpp"

#include <chrono>
#include <cstdint>
#include <string>

namespace tikpp::models::ip {

/*!
 * \brief A data model to represent `/ip/hotspot' list
 */
struct hotspot_model : tikpp::data::model {
    static constexpr auto api_path = "/ip/hotspot";

    using duration = tikpp::data::types::duration<std::chrono::seconds>;

    /*!
     * \brief HotSpot server's name or identifier.
     */
    read_write<std::string> name;

    /*!
     * \brief Address space used to change HotSpot client any IP address to a
     *        valid address.
     *
     * address space used to change HotSpot client any IP address to a valid
     * address. Useful for providing public network access to mobile clients
     * that are not willing to change their networking settings
     */
    read_write<std::string> address_pool;

    /*!
     * \brief Period of inactivity for unauthorized clients.
     *
     * period of inactivity for unauthorized clients. When there is no traffic
     * from this client (literally client computer should be switched off), once
     * the timeout is reached, user is dropped from the HotSpot host list, its
     * used address becomes available
     */
    read_write<duration> idle_timeout;

    /*!
     * \brief Value of how long host can stay out of reach to be removed from
     *        the HotSpot.
     */
    read_write<duration> keepalive_timeout;

    /*!
     * \brief Period of time after which if host hasn't been authorized it self
     *        with system the host entry gets deleted from host table.
     *
     * period of time after which if host hasn't been authorized it self with
     * system the host entry gets deleted from host table. Loop repeats until
     * host logs in the system. Enable if there are situations where host cannot
     * login after being to long in host table unauthorized.
     */
    read_write<duration> login_timeout;

    /*!
     * \brief Interface to run HotSpot on.
     */
    read_write<std::string> interface;

    /*!
     * \brief  number of IP addresses allowed to be bind with the MAC address.
     *
     * number of IP addresses allowed to be bind with the MAC address. When
     * multiple HotSpot clients connected with one MAC-address
     */
    read_write<std::uint32_t> addresses_per_mac;

    /*!
     * \brief HotSpot server default HotSpot profile, which is located in /ip
     *        hotspot profile
     */
    read_write<std::string> profile;

    template <typename Converter>
    void convert(Converter &c) {
        tikpp::data::model::convert(c);

        using namespace tikpp::data::types::literals;

        c("name", name);
        c("address-pool", address_pool);
        c("idle-timeout", idle_timeout, 5_m);
        c("keepalive-timeout", keepalive_timeout);
        c("login-timeout", login_timeout);
        c("interface", interface);
        c("addresses-per-mac", addresses_per_mac, 2);
        c("profile", profile, "default");
    }
};

} // namespace tikpp::models::ip

#endif
