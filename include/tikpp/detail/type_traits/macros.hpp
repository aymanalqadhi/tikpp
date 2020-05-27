#ifndef TIKPP_DETAIL_TYPE_TRAIT_MACROS_HPP
#define TIKPP_DETAIL_TYPE_TRAIT_MACROS_HPP

#include <type_traits>

#define HAS_MEMBER_FUNCTION(member, args)                                      \
    template <typename T, typename = void>                                     \
    struct has_##member : std::false_type {};                                  \
    template <typename T>                                                      \
    struct has_##member<T,                                                     \
                        std::void_t<decltype(std::declval<T>().member args)>>  \
        : std::true_type {};                                                   \
    template <typename T>                                                      \
    constexpr auto has_##member##_v = has_##member<T>::value;

#endif
