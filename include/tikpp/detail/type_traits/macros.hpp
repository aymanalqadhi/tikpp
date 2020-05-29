#ifndef TIKPP_DETAIL_TYPE_TRAIT_MACROS_HPP
#define TIKPP_DETAIL_TYPE_TRAIT_MACROS_HPP

#include <type_traits>

#define HAS_MEMBER_FUNCTION_FULL(name, member, ret, args)                      \
    template <typename T, typename = void>                                     \
    struct name : std::false_type {};                                          \
    template <typename T>                                                      \
    struct name<T, std::enable_if_t<std::is_assignable_v<                      \
                       ret, decltype(std::declval<T>().member args)>>>         \
        : std::true_type {};                                                   \
    template <typename T>                                                      \
    constexpr auto name##_v = name<T>::value;

#define HAS_MEMBER_FUNCTION(member, args)                                      \
    template <typename T, typename = void>                                     \
    struct has_##member : std::false_type {};                                  \
    template <typename T>                                                      \
    struct has_##member<T,                                                     \
                        std::void_t<decltype(std::declval<T>().member args)>>  \
        : std::true_type {};                                                   \
    template <typename T>                                                      \
    constexpr auto has_##member##_v = has_##member<T>::value;

#define HAS_STATIC_FUNCTION_FULL(name, func, ret, args)                        \
    template <typename T, typename = void>                                     \
    struct name : std::false_type {};                                          \
    template <typename T>                                                      \
    struct name<T,                                                             \
                std::enable_if_t<std::is_assignable_v<ret, T::member args>>>   \
        : std::true_type {};                                                   \
    template <typename T>                                                      \
    constexpr auto name##_v = name<T>::value;

#endif
