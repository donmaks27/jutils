// Copyright © 2025 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../core.h"

#include <type_traits>

#define JUTILS_TEMPLATE_ENABLE_IMPL(...) std::enable_if_t<(__VA_ARGS__)>*
#define JUTILS_TEMPLATE_ENABLE(...) JUTILS_TEMPLATE_ENABLE_IMPL(__VA_ARGS__) = nullptr

#if JUTILS_STD_VERSION >= JUTILS_STD20
    #include <concepts>
    #define JUTILS_TEMPLATE_CONDITION_IMPL(Condition, ...) template<__VA_ARGS__> requires Condition
    #define JUTILS_TEMPLATE_CONDITION(Condition, ...) JUTILS_TEMPLATE_CONDITION_IMPL(Condition, __VA_ARGS__)
#else
    #define JUTILS_TEMPLATE_CONDITION_IMPL(Condition, ...) template<__VA_ARGS__, JUTILS_TEMPLATE_ENABLE_IMPL(Condition)>
    #define JUTILS_TEMPLATE_CONDITION(Condition, ...) template<__VA_ARGS__, JUTILS_TEMPLATE_ENABLE(Condition)>
#endif