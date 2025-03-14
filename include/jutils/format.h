// Copyright © 2024 Leonov Maksim. All rights reserved.

#pragma once

#include "core.h"

#include "base_types.h"
#include "type_traits.h"
#include <string>
#include <vector>
#include JUTILS_FORMAT_HEADER

namespace jutils
{
    template<typename T>
    struct string_formatter : std::false_type {};

    template<typename T>
    using has_string_formatter = std::bool_constant<string_formatter<jutils::remove_cvref_t<T>>::value>;
    template<typename T>
    constexpr bool has_string_formatter_v = has_string_formatter<T>::value;

    JUTILS_TEMPLATE_CONDITION(has_string_formatter_v<T>, typename T)
    [[nodiscard]] JUTILS_STD20_CONSTEXPR std::string toString(T value) noexcept { return string_formatter<jutils::remove_cvref_t<T>>::format(value); }

    template<typename T>
    using has_formatter = std::is_constructible<JUTILS_FORMAT_NAMESPACE::formatter<T>>;
    template<typename T>
    constexpr bool has_formatter_v = has_formatter<jutils::remove_cvref_t<T>>::value;

    [[nodiscard]] inline std::string format(const char* str) { return str; }
    JUTILS_TEMPLATE_CONDITION((has_formatter_v<Args>&&...), typename... Args)
    [[nodiscard]] std::string format(const JUTILS_FORMAT_NAMESPACE::format_string<Args...> formatStr, Args&&... args)
    {
        return JUTILS_FORMAT_NAMESPACE::format(formatStr, std::forward<Args>(args)...);
    }

    JUTILS_TEMPLATE_CONDITION(has_string_formatter_v<T>, typename T)
    [[nodiscard]] JUTILS_STD20_CONSTEXPR std::string join(const std::vector<T>& values, const std::string& separator = "")
    {
        if (values.isEmpty())
        {
            return {};
        }
        std::string result = jutils::toString(values[0]);
        for (std::size_t index = 1; index < values.getSize(); index++)
        {
            result += separator + jutils::toString(values[index]);
        }
        return result;
    }

    template<>
    struct string_formatter<int8> : std::true_type { [[nodiscard]] static std::string format(int8 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<uint8> : std::true_type { [[nodiscard]] static std::string format(uint8 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<int16> : std::true_type { [[nodiscard]] static std::string format(int16 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<uint16> : std::true_type { [[nodiscard]] static std::string format(uint16 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<int32> : std::true_type { [[nodiscard]] static std::string format(int32 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<uint32> : std::true_type { [[nodiscard]] static std::string format(uint32 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<int64> : std::true_type { [[nodiscard]] static std::string format(int64 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<uint64> : std::true_type { [[nodiscard]] static std::string format(uint64 value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<float> : std::true_type { [[nodiscard]] static std::string format(float value) noexcept { return std::to_string(value); } };
    template<>
    struct string_formatter<double> : std::true_type { [[nodiscard]] static std::string format(double value) noexcept { return std::to_string(value); } };

    template<>
    struct string_formatter<bool> : std::true_type
    {
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR std::string format(const bool value) noexcept { return value ? "true" : "false"; }
    };
    template<>
    struct string_formatter<const char*> : std::true_type
    {
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR std::string format(const char* value) noexcept { return value; }
    };
    template<>
    struct string_formatter<std::string> : std::true_type
    {
        [[nodiscard]] static JUTILS_STD20_CONSTEXPR std::string format(std::string value) noexcept { return value; }
    };
}

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
