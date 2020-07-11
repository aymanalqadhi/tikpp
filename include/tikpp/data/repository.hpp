#ifndef TIKPP_DATA_REPOSITORY_HPP
#define TIKPP_DATA_REPOSITORY_HPP

#include "tikpp/basic_api.hpp"
#include "tikpp/detail/async_result.hpp"

#include "tikpp/data/converters/creator.hpp"
#include "tikpp/data/query.hpp"
#include "tikpp/data/types/identity.hpp"

#include "tikpp/commands/add.hpp"
#include "tikpp/commands/getall.hpp"
#include "tikpp/commands/remove.hpp"
#include "tikpp/commands/set.hpp"

#include <boost/system/error_code.hpp>

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace tikpp::data {

/*!
 * \brief A struct to manage the router data sets
 */
template <typename Model, typename ApiPtr>
struct repository {
    /*!
     * \brief The query type of the repository
     */
    using query_type = decltype(std::declval<tikpp::data::query>().words);

    /*!
     * \brief Default constructor
     *
     * \param [in,out] api  A pointer to the API connection
     */
    explicit repository(ApiPtr api) : api_ {std::move(api)} {
    }

    /*!
     * \brief Asynchronously loads all items from the router
     *
     * \param [in,out] token  The asynchronous operation completion token
     *
     * \return The passed completion token result
     */
    template <typename CompletionToken>
    inline decltype(auto) async_load(CompletionToken &&token) {
        auto req =
            api_->template make_request<tikpp::commands::getall<Model>>();
        return do_async_load(std::move(req),
                             std::forward<CompletionToken>(token));
    }

    /*!
     * \brief Asynchronously loads filtered items from the router
     *
     * \param [in]     query  The query to be used to filter the result
     * \param [in,out] token  The asynchronous operation completion token
     *
     * \return The passed completion token result
     */
    template <typename CompletionToken>
    inline decltype(auto) async_load(query_type        query,
                                     CompletionToken &&token) {
        auto req = api_->template make_request<tikpp::commands::getall<Model>>(
            std::move(query));
        return do_async_load(std::move(req),
                             std::forward<CompletionToken>(token));
    }

    /*!
     * \brief Asynchronously loads all items from the router as a stream
     *
     * \param [in,out] token  The asynchronous operation completion token
     *
     * \return The passed completion token result
     */
    template <typename CompletionToken>
    inline decltype(auto) async_stream(CompletionToken &&token) {
        auto req =
            api_->template make_request<tikpp::commands::getall<Model>>();
        return do_async_stream(std::move(req),
                               std::forward<CompletionToken>(token));
    }

    /*!
     * \brief Asynchronously loads filtered items from the router as a stream
     *
     * \param [in]     query  The query to be used to filter the result
     * \param [in,out] token  The asynchronous operation completion token
     *
     * \return The passed completion token result
     */
    template <typename CompletionToken>
    inline decltype(auto) async_stream(query_type        query,
                                       CompletionToken &&token) {
        auto req = api_->template make_request<tikpp::commands::getall<Model>>(
            std::move(query));
        return do_async_stream(std::move(req),
                               std::forward<CompletionToken>(token));
    }

    /*!
     * \brief Asynchronously adds an item to the router
     *
     * \param [in]     model  The item to be added
     * \param [in,out] token  The asynchronous operation completion token
     *
     * \return The passed completion token result
     */
    template <typename CompletionToken>
    decltype(auto) async_add(Model model, CompletionToken &&token) {
        GENERATE_COMPLETION_HANDLER(void(const boost::system::error_code &,
                                         tikpp::data::types::identity &&),
                                    token, handler, result)

        auto req = api_->template make_request<tikpp::commands::add<Model>>(
            std::move(model));

        api_->async_send(
            std::move(req), [handler {std::move(handler)}](
                                const auto &err, auto &&resp) mutable {
                if (err) {
                    handler(err, 0);
                } else if (resp.error()) {
                    handler(resp.error(), 0);
                } else {
                    handler(boost::system::error_code {},
                            tikpp::data::types::identity {resp["ret"]});
                }

                return false;
            });

        return result.get();
    }

    /*!
     * \brief Asynchronously removes an item from the router
     *
     * \param [in]     id     The id of the item to be removed
     * \param [in,out] token  The asynchronous operation completion token
     *
     * \return The passed completion token result
     */
    template <typename CompletionToken>
    decltype(auto) async_remove(std::string id, CompletionToken &&token) {
        GENERATE_COMPLETION_HANDLER(void(const boost::system::error_code &),
                                    token, handler, result)

        auto req =
            api_->template make_request<tikpp::commands::remove<Model>>(id);

        api_->async_send(std::move(req),
                         [handler {std::move(handler)}](const auto &err,
                                                        auto &&resp) mutable {
                             if (err) {
                                 handler(err);
                             } else if (resp.error()) {
                                 handler(resp.error());
                             } else {
                                 handler(boost::system::error_code {});
                             }

                             return false;
                         });

        return result.get();
    }

    /*!
     * \brief Asynchronously removes an item from the router
     *
     * \param [in]     id     The id of the item to be removed
     * \param [in,out] token  The asynchronous operation completion token
     *
     * \return The passed completion token result
     */
    template <typename CompletionToken>
    inline decltype(auto) async_remove(tikpp::data::types::identity id,
                                       CompletionToken &&           token) {
        return async_remove(id.to_string(),
                            std::forward<CompletionToken>(token));
    }

    /*!
     * \brief Asynchronously updates an item in the router
     *
     * \param [in]     id     The id of the item to be removed
     * \param [in,out] token  The asynchronous operation completion token
     *
     * \return The passed completion token result
     */
    template <typename CompletionToken>
    decltype(auto) async_update(Model model, CompletionToken &&token) {
        GENERATE_COMPLETION_HANDLER(void(const boost::system::error_code &),
                                    token, handler, result)

        auto req = api_->template make_request<tikpp::commands::set<Model>>(
            std::move(model));

        api_->async_send(std::move(req),
                         [handler {std::move(handler)}](const auto &err,
                                                        auto &&resp) mutable {
                             if (err) {
                                 handler(err);
                             } else if (resp.error()) {
                                 handler(resp.error());
                             } else {
                                 handler(boost::system::error_code {});
                             }

                             return false;
                         });

        return result.get();
    }

  private:
    template <typename CompletionToken>
    decltype(auto) do_async_load(std::shared_ptr<tikpp::request> req,
                                 CompletionToken &&              token) {
        GENERATE_COMPLETION_HANDLER(
            void(const boost::system::error_code &, std::vector<Model> &&),
            token, handler, result)

        api_->async_send(
            std::move(req), [handler {std::move(handler)},
                             ret = std::make_shared<std::vector<Model>>()](
                                const auto &err, auto &&resp) mutable {
                if (err) {
                    handler(err, std::vector<Model> {});
                } else if (resp.error()) {
                    handler(resp.error(), std::vector<Model> {});
                } else if (resp.type() == tikpp::response_type::normal &&
                           resp.empty()) {
                    handler(boost::system::error_code {}, std::move(*ret));
                } else if (resp.type() != tikpp::response_type::data) {
                    handler(tikpp::make_error_code(
                                tikpp::error_code::invalid_response),
                            std::vector<Model> {});
                } else {
                    tikpp::data::converters::creator<tikpp::response> c {resp};
                    ret->emplace_back(c.create<Model>());
                    return true;
                }

                return false;
            });

        return result.get();
    }

    template <typename CompletionToken>
    decltype(auto) do_async_stream(std::shared_ptr<tikpp::request> req,
                                   CompletionToken &&              token) {
        GENERATE_COMPLETION_HANDLER(
            void(const boost::system::error_code &, Model &&), token, handler,
            result)

        api_->async_send(std::move(req), [handler {std::move(handler)}](
                                             const auto &err,
                                             auto &&     resp) mutable {
            if (err) {
                handler(err, Model {});
            } else if (resp.error()) {
                handler(resp.error(), Model {});
            } else if (resp.type() == tikpp::response_type::normal &&
                       resp.empty()) {
                handler(tikpp::make_error_code(tikpp::error_code::list_end),
                        Model {});
            } else if (resp.type() != tikpp::response_type::data) {
                handler(
                    tikpp::make_error_code(tikpp::error_code::invalid_response),
                    Model {});
            } else if (!resp.empty()) {
                tikpp::data::converters::creator<tikpp::response> creator {
                    resp};

                Model item {};
                item.convert(creator);

                handler(boost::system::error_code {}, std::move(item));
                return true;
            }

            return false;
        });

        return result.get();
    }

    ApiPtr api_;
};

/*!
 * \brief Creates a new instance of \see repository
 *
 * \param [in] api  A pointer to the api connection
 *
 * \return The create \see repository instance
 */
template <typename Model, typename ApiPtr>
[[nodiscard]] inline auto make_repository(ApiPtr api)
    -> repository<Model, ApiPtr> {
    return repository<Model, ApiPtr> {api};
}

} // namespace tikpp::data

#endif
