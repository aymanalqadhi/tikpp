#ifndef TIKPP_API_REPOSITORY_HPP
#define TIKPP_API_REPOSITORY_HPP

#include "tikpp/basic_api.hpp"
#include "tikpp/commands/add.hpp"
#include "tikpp/commands/getall.hpp"
#include "tikpp/commands/remove.hpp"
#include "tikpp/commands/set.hpp"
#include "tikpp/detail/async_result.hpp"
#include "tikpp/models/converters.hpp"
#include "tikpp/models/query.hpp"
#include "tikpp/models/types/identity.hpp"

#include <boost/system/error_code.hpp>

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include <iostream>

namespace tikpp {

template <typename Model, typename ApiPtr>
struct api_repository {
    using query_type = decltype(std::declval<tikpp::models::query>().words);

    explicit api_repository(ApiPtr api) : api_ {std::move(api)} {
    }

    template <typename CompletionToken>
    inline decltype(auto) async_load(CompletionToken &&token) {
        auto req =
            api_->template make_request<tikpp::commands::getall<Model>>();
        return do_async_load(std::move(req),
                             std::forward<CompletionToken>(token));
    }

    template <typename CompletionToken>
    inline decltype(auto) async_load(query_type        query,
                                     CompletionToken &&token) {
        auto req = api_->template make_request<tikpp::commands::getall<Model>>(
            std::move(query));
        return do_async_load(std::move(req),
                             std::forward<CompletionToken>(token));
    }

    template <typename CompletionToken>
    decltype(auto) async_add(Model model, CompletionToken &&token) {
        GENERATE_COMPLETION_HANDLER(void(const boost::system::error_code &,
                                         tikpp::models::types::identity &&),
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
                            tikpp::models::types::identity {resp["ret"]});
                }

                return false;
            });

        return result.get();
    }

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
                                 handler(tikpp::make_error_code(resp.error()));
                             } else {
                                 handler(boost::system::error_code {});
                             }

                             return false;
                         });

        return result.get();
    }

    template <typename CompletionToken>
    inline decltype(auto) async_remove(tikpp::models::types::identity id,
                                       CompletionToken &&             token) {
        return async_remove(id.to_string(),
                            std::forward<CompletionToken>(token));
    }

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

    template <typename CompletionToken>
    inline decltype(auto) async_stream(CompletionToken &&token) {
        auto req =
            api_->template make_request<tikpp::commands::getall<Model>>();
        return do_async_stream(std::move(req),
                               std::forward<CompletionToken>(token));
    }

    template <typename CompletionToken>
    inline decltype(auto) async_stream(query_type        query,
                                       CompletionToken &&token) {
        auto req = api_->template make_request<tikpp::commands::getall<Model>>(
            std::move(query));
        return do_async_stream(std::move(req),
                               std::forward<CompletionToken>(token));
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
                    return false;
                }

                if (resp.empty()) {
                    handler(boost::system::error_code {}, std::move(*ret));
                    return false;
                }

                tikpp::models::creator<tikpp::response> creator {resp};

                Model item {};
                item.convert(creator);

                ret->emplace_back(std::move(item));
                return true;
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
                tikpp::models::creator<tikpp::response> creator {resp};

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
}; // namespace tikpp

template <typename Model, typename ApiPtr>
inline auto make_repository(ApiPtr api) -> api_repository<Model, ApiPtr> {
    return api_repository<Model, ApiPtr> {api};
}

} // namespace tikpp

#endif
