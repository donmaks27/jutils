// Copyright © 2024 Leonov Maksim. All rights reserved.

#pragma once

#include "base_types.h"
#include "type_traits.h"
#include "jstring.h"

#if defined(JUTILS_USE_FMT)
    #include <fmt/format.h>
#else
    #include <format>
#endif

namespace jutils
{
    template<typename T>
    struct formatter : std::bool_constant<std::is_arithmetic_v< jutils::remove_cvref_t<T> >>
    {
        [[nodiscard]] static jstring format(T value) noexcept { return std::to_string(value); }
    };

    template<typename T>
    using has_jstring_formatter = std::bool_constant<formatter<T>::value>;
    template<typename T>
    constexpr bool has_jstring_formatter_v = has_jstring_formatter<T>::value;

    JUTILS_TEMPLATE_CONDITION(has_jstring_formatter_v<T>, typename T)
    [[nodiscard]] JUTILS_STD20_CONSTEXPR jstring toString(T value) noexcept { return formatter<T>::format(value); }

#if defined(JUTILS_USE_FMT)
    template<typename T>
    using has_formatter = fmt::has_formatter<T, fmt::format_context>;
#else
    template<typename T>
    using has_formatter = std::is_constructible<std::format_context::formatter_type<T>>;
#endif
    template<typename T>
    constexpr bool has_formatter_v = has_formatter<T>::value;

    [[nodiscard]] inline jstring format(const char* str) { return str; }
#if defined(JUTILS_USE_FMT)
    JUTILS_TEMPLATE_CONDITION((has_formatter_v<Args> && ...), typename... Args)
    [[nodiscard]] jstring format(const fmt::format_string<Args...> formatStr, Args&&... args)
    {
        return fmt::format(formatStr, std::forward<Args>(args)...);
    }
#else
    template<typename... Args> requires (has_formatter_v<Args> && ...)
    [[nodiscard]] jstring format(const std::format_string<Args...> formatStr, Args&&... args)
    {
        return std::format(formatStr, std::forward<Args>(args)...);
    }
#endif

    JUTILS_TEMPLATE_CONDITION(has_jstring_formatter_v<T>, typename T)
    [[nodiscard]] JUTILS_STD20_CONSTEXPR jstring join(const jarray<T>& values, const jstring& separator = "")
    {
        if (values.isEmpty())
        {
            return "";
        }
        jstring result = toString(values[0]);
        for (typename jarray<T>::size_type index = 1; index < values.getSize(); index++)
        {
            result += separator + toString(values[index]);
        }
        return result;
    }

    template<>
    struct formatter<const char*> : std::true_type
    {
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR jstring format(const char* value) noexcept { return value; }
    };
    template<>
    struct formatter<std::string> : std::true_type
    {
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR jstring format(std::string value) noexcept { return value; }
    };
    template<>
    struct formatter<jstring> : std::true_type
    {
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR jstring format(jstring value) noexcept { return std::move(value); }
    };
}

#if defined(JUTILS_USE_FMT)
template<>
struct fmt::formatter<jutils::jstring> : fmt::formatter<const jutils::jstring::char_type*>
{
    template<typename FormatContext>
    auto format(const jutils::jstring& str, FormatContext& ctx) const
    {
        return fmt::formatter<const jutils::jstring::char_type*>::format(*str, ctx);
    }
};
#define JUTILS_STRING_FORMATTER_CONSTEXPR(type, funcName)                                                                           \
    template<> struct jutils::formatter<jutils::remove_cvref_t< type >> : std::true_type                                            \
    {                                                                                                                               \
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR jutils::jstring format(const type& value) noexcept { return funcName(value); }  \
    };                                                                                                                              \
    template<>                                                                                                                      \
    struct fmt::formatter<type> : fmt::formatter<decltype(funcName(type()))>                                                        \
    {                                                                                                                               \
        template<typename FormatContext>                                                                                            \
        auto format(const type& value, FormatContext& ctx) const                                                                    \
        {                                                                                                                           \
            return fmt::formatter<decltype(funcName(type()))>::format(funcName(value), ctx);                                        \
        }                                                                                                                           \
    };
#define JUTILS_STRING_FORMATTER(type, funcName)                                                             \
    template<> struct jutils::formatter<jutils::remove_cvref_t< type >> : std::true_type                    \
    {                                                                                                       \
        [[nodiscard]] static jutils::jstring format(const type& value) noexcept { return funcName(value); } \
    };                                                                                                      \
    template<>                                                                                              \
    struct fmt::formatter<type> : fmt::formatter<decltype(funcName(type()))>                                \
    {                                                                                                       \
        template<typename FormatContext>                                                                    \
        auto format(const type& value, FormatContext& ctx) const                                            \
        {                                                                                                   \
            return fmt::formatter<decltype(funcName(type()))>::format(funcName(value), ctx);                \
        }                                                                                                   \
    };
#else
template<>
struct std::formatter<jutils::jstring> : std::formatter<const jutils::jstring::char_type*>
{
    template<typename FormatContext>
    auto format(const jutils::jstring& str, FormatContext& ctx) const
    {
        return std::formatter<const jutils::jstring::char_type*>::format(*str, ctx);
    }
};
#define JUTILS_STRING_FORMATTER_CONSTEXPR(type, funcName)                                                                           \
    template<> struct jutils::formatter<jutils::remove_cvref_t< type >> : std::true_type                                            \
    {                                                                                                                               \
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR jutils::jstring format(const type& value) noexcept { return funcName(value); }  \
    };                                                                                                                              \
    template<>                                                                                                                      \
    struct std::formatter<type> : std::formatter<decltype(funcName(type()))>                                                        \
    {                                                                                                                               \
        template<typename FormatContext>                                                                                            \
        auto format(const type& value, FormatContext& ctx) const                                                                    \
        {                                                                                                                           \
            return std::formatter<decltype(funcName(type()))>::format(funcName(value), ctx);                                        \
        }                                                                                                                           \
    };
#define JUTILS_STRING_FORMATTER(type, funcName)                                                             \
    template<> struct jutils::formatter<jutils::remove_cvref_t< type >> : std::true_type                    \
    {                                                                                                       \
        [[nodiscard]] static jutils::jstring format(const type& value) noexcept { return funcName(value); } \
    };                                                                                                      \
    template<>                                                                                              \
    struct std::formatter<type> : std::formatter<decltype(funcName(type()))>                                \
    {                                                                                                       \
        template<typename FormatContext>                                                                    \
        auto format(const type& value, FormatContext& ctx) const                                            \
        {                                                                                                   \
            return std::formatter<decltype(funcName(type()))>::format(funcName(value), ctx);                \
        }                                                                                                   \
    };
#endif
