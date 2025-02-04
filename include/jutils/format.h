// Copyright © 2024 Leonov Maksim. All rights reserved.

#pragma once

#include "base_types.h"
#include "type_traits.h"
#include "string.h"

#if defined(JUTILS_USE_FMT)
    #include <fmt/format.h>
#else
    #include <format>
#endif

namespace jutils
{
    template<typename T>
    struct formatter : std::false_type {};

    template<typename T>
    using has_string_formatter = std::bool_constant<formatter<jutils::remove_cvref_t<T>>::value>;
    template<typename T>
    constexpr bool has_string_formatter_v = has_string_formatter<T>::value;

    JUTILS_TEMPLATE_CONDITION(has_string_formatter_v<T>, typename T)
    [[nodiscard]] JUTILS_STD20_CONSTEXPR string toString(T value) noexcept { return formatter<jutils::remove_cvref_t<T>>::format(value); }

#if defined(JUTILS_USE_FMT)
    template<typename T>
    using has_formatter = fmt::has_formatter<T, fmt::format_context>;
#else
    template<typename T>
    using has_formatter = std::is_constructible<std::format_context::formatter_type<T>>;
#endif
    template<typename T>
    constexpr bool has_formatter_v = has_formatter<jutils::remove_cvref_t<T>>::value;

    [[nodiscard]] inline string format(const char* str) { return str; }
#if defined(JUTILS_USE_FMT)
    JUTILS_TEMPLATE_CONDITION((has_formatter_v<Args> && ...), typename... Args)
    [[nodiscard]] string format(const fmt::format_string<Args...> formatStr, Args&&... args)
    {
        return fmt::format(formatStr, std::forward<Args>(args)...);
    }
#else
    JUTILS_TEMPLATE_CONDITION((has_formatter_v<Args> && ...), typename... Args)
    [[nodiscard]] string format(const std::format_string<Args...> formatStr, Args&&... args)
    {
        return std::format(formatStr, std::forward<Args>(args)...);
    }
#endif

    JUTILS_TEMPLATE_CONDITION(has_string_formatter_v<T>, typename T)
    [[nodiscard]] JUTILS_STD20_CONSTEXPR string join(const vector<T>& values, const string& separator = "")
    {
        if (values.isEmpty())
        {
            return {};
        }
        string result = jutils::toString(values[0]);
        for (index_type index = 1; index < values.getSize(); index++)
        {
            result += separator + jutils::toString(values[index]);
        }
        return result;
    }

    template<>
    struct formatter<int8> : std::true_type { [[nodiscard]] static string format(int8 value) noexcept { return std::to_string(value); } };
    template<>
    struct formatter<uint8> : std::true_type { [[nodiscard]] static string format(uint8 value) noexcept { return std::to_string(value); } };
    template<>
    struct formatter<int16> : std::true_type { [[nodiscard]] static string format(int16 value) noexcept { return std::to_string(value); } };
    template<>
    struct formatter<uint16> : std::true_type { [[nodiscard]] static string format(uint16 value) noexcept { return std::to_string(value); } };
    template<>
    struct formatter<int32> : std::true_type { [[nodiscard]] static string format(int32 value) noexcept { return std::to_string(value); } };
    template<>
    struct formatter<uint32> : std::true_type { [[nodiscard]] static string format(uint32 value) noexcept { return std::to_string(value); } };
    template<>
    struct formatter<int64> : std::true_type { [[nodiscard]] static string format(int64 value) noexcept { return std::to_string(value); } };
    template<>
    struct formatter<uint64> : std::true_type { [[nodiscard]] static string format(uint64 value) noexcept { return std::to_string(value); } };
    template<>
    struct formatter<float> : std::true_type { [[nodiscard]] static string format(float value) noexcept { return std::to_string(value); } };
    template<>
    struct formatter<double> : std::true_type { [[nodiscard]] static string format(double value) noexcept { return std::to_string(value); } };

    template<>
    struct formatter<bool> : std::true_type
    {
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR string format(const bool value) noexcept { return value ? "true" : "false"; }
    };
    template<>
    struct formatter<const char*> : std::true_type
    {
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR string format(const char* value) noexcept { return value; }
    };
    template<>
    struct formatter<std::string> : std::true_type
    {
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR string format(std::string value) noexcept { return value; }
    };
    template<>
    struct formatter<string> : std::true_type
    {
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR string format(string value) noexcept { return value; }
    };
}

#if defined(JUTILS_USE_FMT)
    #define JUTILS_FORMAT_NAMESPACE fmt
#else
    #define JUTILS_FORMAT_NAMESPACE std
#endif

template<> struct JUTILS_FORMAT_NAMESPACE::formatter<jutils::string> : JUTILS_FORMAT_NAMESPACE::formatter<jutils::string::base_type>
{
    template<typename FormatContext> auto format(const jutils::string& str, FormatContext& ctx) const
    {
        return JUTILS_FORMAT_NAMESPACE::formatter<jutils::string::base_type>::format(*str, ctx);
    }
};
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
    template<> struct jutils::formatter<jutils::remove_cvref_t< type >> : std::true_type                                            \
    {                                                                                                                               \
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR jutils::string format(const type& value) noexcept { return funcName(value); }   \
    };                                                                                                                              \
    JUTILS_FORMATTER(type, funcName)

#define JUTILS_STRING_FORMATTER(type, funcName)                                                             \
    template<> struct jutils::formatter<jutils::remove_cvref_t< type >> : std::true_type                    \
    {                                                                                                       \
        [[nodiscard]] static jutils::string format(const type& value) noexcept { return funcName(value); }  \
    };                                                                                                      \
    JUTILS_FORMATTER(type, funcName)
