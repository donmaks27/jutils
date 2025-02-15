// Copyright © 2025 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../core.h"

#include JUTILS_FORMAT_HEADER

#define JUTILS_FORMATTER(type, funcName) namespace JUTILS_FORMAT_NAMESPACE                                  \
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

#define JUTILS_STRING_FORMATTER_CONSTEXPR(type, funcName) namespace jutils                              \
    {                                                                                                   \
        template<> struct string_formatter<jutils::remove_cvref_t< type >> : std::true_type             \
        {                                                                                               \
            [[nodiscard]] static JUTILS_STD20_CONSTEXPR std::string format(const type& value) noexcept  \
                { return funcName(value); }                                                             \
        };                                                                                              \
    }                                                                                                   \
    JUTILS_FORMATTER(type, funcName)

#define JUTILS_STRING_FORMATTER(type, funcName) namespace jutils                                            \
    {                                                                                                       \
        template<> struct string_formatter<jutils::remove_cvref_t< type >> : std::true_type                 \
        {                                                                                                   \
            [[nodiscard]] static std::string format(const type& value) noexcept { return funcName(value); } \
        };                                                                                                  \
    }                                                                                                       \
    JUTILS_FORMATTER(type, funcName)
