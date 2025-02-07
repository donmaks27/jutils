// Copyright © 2024 Leonov Maksim. All rights reserved.

#pragma once

#include "core.h"
#include "macro_template_condition.h"

#ifndef JUTILS_MODULE
    #include "type_traits.h"
    #include "string.h"
    #include JUTILS_FORMAT_HEADER
#endif

JUTILS_MODULE_EXPORT namespace jutils
{
    template<typename T>
    struct string_formatter : std::false_type {};

    template<typename T>
    using has_string_formatter = std::bool_constant<string_formatter<jutils::remove_cvref_t<T>>::value>;
    template<typename T>
    constexpr bool has_string_formatter_v = has_string_formatter<T>::value;

    JUTILS_TEMPLATE_CONDITION(has_string_formatter_v<T>, typename T)
    [[nodiscard]] JUTILS_STD20_CONSTEXPR string toString(T value) noexcept { return string_formatter<jutils::remove_cvref_t<T>>::format(value); }

#if defined(JUTILS_USE_FMT)
    template<typename T>
    using has_formatter = fmt::has_formatter<T, fmt::format_context>;
#else
    template<typename T>
    using has_formatter = std::is_constructible<std::formatter<T>>;
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
    struct string_formatter<int8> : std::true_type { [[nodiscard]] static string format(int8 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<uint8> : std::true_type { [[nodiscard]] static string format(uint8 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<int16> : std::true_type { [[nodiscard]] static string format(int16 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<uint16> : std::true_type { [[nodiscard]] static string format(uint16 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<int32> : std::true_type { [[nodiscard]] static string format(int32 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<uint32> : std::true_type { [[nodiscard]] static string format(uint32 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<int64> : std::true_type { [[nodiscard]] static string format(int64 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<uint64> : std::true_type { [[nodiscard]] static string format(uint64 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<float> : std::true_type { [[nodiscard]] static string format(float value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<double> : std::true_type { [[nodiscard]] static string format(double value) noexcept { return std::to_string(value); } };

    template<>
    struct string_formatter<bool> : std::true_type
    {
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR string format(const bool value) noexcept { return value ? "true" : "false"; }
    };
    template<>
    struct string_formatter<const char*> : std::true_type
    {
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR string format(const char* value) noexcept { return value; }
    };
    template<>
    struct string_formatter<std::string> : std::true_type
    {
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR string format(std::string value) noexcept { return value; }
    };
    template<>
    struct string_formatter<string> : std::true_type
    {
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR string format(string value) noexcept { return value; }
    };
}

template<>
struct JUTILS_FORMAT_NAMESPACE::formatter<jutils::string> : JUTILS_FORMAT_NAMESPACE::formatter<jutils::string::base_type>
{
    template<typename FormatContext> auto format(const jutils::string& str, FormatContext& ctx) const
    {
        return JUTILS_FORMAT_NAMESPACE::formatter<jutils::string::base_type>::format(*str, ctx);
    }
};
