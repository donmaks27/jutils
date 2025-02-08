// Copyright © 2025 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../core.h"

#ifndef JUTILS_MODULE
    #include JUTILS_FORMAT_HEADER
#endif

#define JUTILS_FORMATTER(type, funcName) JUTILS_MODULE_EXPORT namespace JUTILS_FORMAT_NAMESPACE             \
    {                                                                                                       \
        template<>                                                                                          \
        struct formatter<type> : formatter<decltype(funcName(std::declval<type>()))>                        \
        {                                                                                                   \
            template<typename FormatContext> auto format(const type& value, FormatContext& ctx) const       \
            {                                                                                               \
                return formatter<decltype(funcName(std::declval<type>()))>::format(funcName(value), ctx);   \
            }                                                                                               \
        };                                                                                                  \
    }

#define JUTILS_STRING_FORMATTER_CONSTEXPR(type, funcName) JUTILS_MODULE_EXPORT namespace jutils             \
    {                                                                                                       \
        template<> struct string_formatter<jutils::remove_cvref_t< type >> : std::true_type                 \
        {                                                                                                   \
            [[nodiscard]] static JUTILS_STD20_CONSTEXPR jutils::string format(const type& value) noexcept   \
                { return funcName(value); }                                                                 \
        };                                                                                                  \
    }                                                                                                       \
    JUTILS_FORMATTER(type, funcName)

#define JUTILS_STRING_FORMATTER(type, funcName) JUTILS_MODULE_EXPORT namespace jutils                           \
    {                                                                                                           \
        template<> struct string_formatter<jutils::remove_cvref_t< type >> : std::true_type                     \
        {                                                                                                       \
            [[nodiscard]] static jutils::string format(const type& value) noexcept { return funcName(value); }  \
        };                                                                                                      \
    }                                                                                                           \
    JUTILS_FORMATTER(type, funcName)
