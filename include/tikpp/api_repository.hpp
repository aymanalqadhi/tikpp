#ifndef TIKPP_API_REPOSITORY_HPP
#define TIKPP_API_REPOSITORY_HPP

#include "tikpp/basic_api.hpp"
#include "tikpp/commands/getall.hpp"
#include "tikpp/detail/async_result.hpp"
#include "tikpp/models/convert.hpp"
#include "tikpp/models/query.hpp"

#include <boost/system/error_code.hpp>

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace tikpp {

template <typename Model, typename ApiPtr>
struct api_repository {
    using query_type = decltype(std::declval<tikpp::models::query>().words);

    explicit api_repository(ApiPtr api) : api_ {std::move(api)} {
    }

    template <typename CompletionToken>
    inline decltype(auto) load_all(CompletionToken &&token) {
        auto req =
            api_->template make_request<tikpp::commands::getall<Model>>();
        return do_load(std::move(req), std::forward<CompletionToken>(token));
    }

    template <typename CompletionToken>
    inline decltype(auto) load(query_type query, CompletionToken &&token) {
        auto req = api_->template make_request<tikpp::commands::getall<Model>>(
            std::move(query));
        return do_load(std::move(req), std::forward<CompletionToken>(token));
    }

  private:
    template <typename CompletionToken>
    decltype(auto) do_load(std::shared_ptr<tikpp::request> req,
                           CompletionToken &&              token) {
        GENERATE_COMPLETION_HANDLER(
            void(const boost::system::error_code &, std::vector<Model> &&),
            token, handler, result)

        api_->async_send(
            std::move(req), [handler {std::move(handler)},
                             ret = std::make_shared<std::vector<Model>>()](
                                const auto &err, auto &&resp) {
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

    ApiPtr api_;
};

template <typename Model, typename ApiPtr>
inline auto make_repository(ApiPtr api) -> api_repository<Model, ApiPtr> {
    return api_repository<Model, ApiPtr> {api};
}

} // namespace tikpp

#endif
