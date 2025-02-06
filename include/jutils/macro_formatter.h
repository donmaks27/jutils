// Copyright © 2025 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#define JUTILS_FORMATTER(type, funcName)                                                                                            \
    template<>                                                                                                                      \
    struct JUTILS_FORMAT_NAMESPACE::formatter<type> : JUTILS_FORMAT_NAMESPACE::formatter<decltype(funcName(std::declval<type>()))>  \
    {                                                                                                                               \
        template<typename FormatContext> auto format(const type& value, FormatContext& ctx) const                                   \
        {                                                                                                                           \
            return JUTILS_FORMAT_NAMESPACE::formatter<decltype(funcName(std::declval<type>()))>::format(funcName(value), ctx);      \
        }                                                                                                                           \
    }

#define JUTILS_STRING_FORMATTER_CONSTEXPR(type, funcName)                                                                           \
    template<> struct jutils::string_formatter<jutils::remove_cvref_t< type >> : std::true_type                                     \
    {                                                                                                                               \
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR jutils::string format(const type& value) noexcept { return funcName(value); }   \
    };                                                                                                                              \
    JUTILS_FORMATTER(type, funcName)

#define JUTILS_STRING_FORMATTER(type, funcName)                                                             \
    template<> struct jutils::string_formatter<jutils::remove_cvref_t< type >> : std::true_type             \
    {                                                                                                       \
        [[nodiscard]] static jutils::string format(const type& value) noexcept { return funcName(value); }  \
    };                                                                                                      \
    JUTILS_FORMATTER(type, funcName)
