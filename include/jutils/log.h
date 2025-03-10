// Copyright © 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#include "format.h"

namespace jutils::log
{
    enum class type : uint8 { fatal, error, warning, info };

    inline void print(const char* str) { std::printf("%s", str); }
    inline void print(const std::string& str) { print(str.c_str()); }

    inline void println(const char* str) { std::printf("%s\n", str); }
    inline void println(const std::string& str) { println(str.c_str()); }
}

namespace jutils_private::log
{
    constexpr const char* typeToString(const jutils::log::type type) noexcept
    {
        switch (type)
        {
        case jutils::log::type::fatal:   return "[FATAL]";
        case jutils::log::type::error:   return "[ERR]  ";
        case jutils::log::type::warning: return "[WARN] ";
        case jutils::log::type::info:    return "[INFO] ";
        default: ;
        }
        return "       ";
    }
}
JUTILS_STRING_FORMATTER_CONSTEXPR(jutils::log::type, jutils_private::log::typeToString)

#if JUTILS_VA_OPT_SUPPORTED // __VA_OPT__
    #define JUTILS_LOG(logType, formatStr, ...) jutils::log::print(jutils::format(                 \
        "{} {}\n", jutils::log::type::logType, jutils::format(formatStr __VA_OPT__(,) __VA_ARGS__) \
    ))
#else
    #define JUTILS_LOG(logType, ...) jutils::log::print(jutils::format(    \
        "{} {}\n", jutils::log::type::logType, jutils::format(__VA_ARGS__) \
    ))
#endif
