// Copyright © 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#include <type_traits>
#if JUTILS_STD_VERSION >= JUTILS_STD20
    #include <concepts>
#endif

namespace jutils
{
#if JUTILS_STD_VERSION >= JUTILS_STD20
    template<typename T>
    using remove_cvref = std::remove_cvref<T>;
#else
    template<class T>
    struct remove_cvref
    {
        using type = std::remove_cv_t<std::remove_reference_t<T>>;
    };
#endif
    template<class T>
    using remove_cvref_t = typename jutils::remove_cvref<T>::type;

#if JUTILS_STD_VERSION >= JUTILS_STD20
    template<typename Fn, typename... Args>
    struct is_predicate : std::false_type {};
    template<typename Fn, typename... Args> requires std::predicate<Fn, Args...>
    struct is_predicate<Fn, Args...> : std::true_type {};
#else
    template<typename Fn, typename... Args>
    struct is_predicate : std::is_invocable<Fn, Args...> {};
#endif
    template<typename Fn, typename... Args>
    constexpr bool is_predicate_v = jutils::is_predicate<Fn, Args...>::value;

    template<typename T>
    constexpr bool is_abstract_v = std::is_abstract_v<jutils::remove_cvref_t<T>>;
    template<typename T1, typename T2>
    constexpr bool is_same_v = std::is_same_v<jutils::remove_cvref_t<T1>, jutils::remove_cvref_t<T2>>;
    template<typename Base, typename Child>
    constexpr bool is_base_v = std::is_base_of_v<jutils::remove_cvref_t<Base>, jutils::remove_cvref_t<Child>>;
}
