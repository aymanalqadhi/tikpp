#include "tikpp/models/proplist_collector.hpp"
#include "tikpp/tests/fakes/model.hpp"

#include "gtest/gtest.h"

#include <algorithm>
#include <string>
#include <vector>

namespace tikpp::tests {

TEST(PropListTests, CollectingTest) {
    tikpp::tests::fakes::model1                                 model {};
    tikpp::models::proplist_collector<std::vector<std::string>> collector {};

    model.convert(collector);

    const auto &proplist = collector.proplist;

    auto contains = [&proplist](const std::string &key) -> bool {
        auto itr = std::find(proplist.begin(), proplist.end(), key);
        return itr != proplist.end();
    };

    EXPECT_TRUE(contains("prop1"));
    EXPECT_TRUE(contains("prop2"));
    EXPECT_TRUE(contains("prop3"));
    EXPECT_TRUE(contains("prop5"));
    EXPECT_TRUE(contains("prop6"));
    EXPECT_FALSE(contains("prop7"));
    EXPECT_FALSE(contains("prop8"));
}

} // namespace tikpp::tests
