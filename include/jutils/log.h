// Copyright © 2021 Leonov Maksim. All Rights Reserved.

#pragma once

/**
 * #define JUTILS_LOG_DISABLED - disable printing logs
 */

#include "core.h"
#include "format.h"
#include "macro_formatter.h"

namespace jutils::log
{
    enum class type : uint8 { fatal, error, warning, info };

    inline void print(const char* str) { std::printf("%s", str); }
    inline void print(const std::string& str) { print(str.c_str()); }
    inline void print(const string& str) { print(*str); }

    inline void println(const char* str) { std::printf("%s\n", str); }
    inline void println(const std::string& str) { println(str.c_str()); }
    inline void println(const string& str) { println(*str); }
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
JUTILS_STRING_FORMATTER_CONSTEXPR(jutils::log::type, jutils_private::log::typeToString);
