// Copyright © 2021 Leonov Maksim. All Rights Reserved.

#pragma once

/**
 * #define JUTILS_LOG_DISABLED - disable printing logs
 */

#include "format.h"

#ifndef JUTILS_LOG_DISABLED
    #if !defined(_MSC_VER) && defined(__FILE_NAME__)
        #define JUTILS_FILENAME __FILE_NAME__
    #else
        #define JUTILS_FILENAME __FILE__
    #endif
#endif

namespace jutils::log
{
    enum class verbosityLevel : uint8 { fatal, error, warning, info };

    constexpr const char* verbosityLevelToString(const verbosityLevel verbosity) noexcept
    {
        switch (verbosity)
        {
        case verbosityLevel::fatal:   return "[FATAL]";
        case verbosityLevel::error:   return "[ERR]  ";
        case verbosityLevel::warning: return "[WARN] ";
        case verbosityLevel::info:    return "[INFO] ";
        default: ;
        }
        return "       ";
    }

    inline void print(const char* str) { std::printf("%s", str); }
    inline void print(const std::string& str) { jutils::log::print(str.c_str()); }
    inline void print(const string& str) { jutils::log::print(*str); }

    inline void println(const char* str) { std::printf("%s\n", str); }
    inline void println(const std::string& str) { jutils::log::println(str.c_str()); }
    inline void println(const string& str) { jutils::log::println(*str); }
}

JUTILS_STRING_FORMATTER_CONSTEXPR(jutils::log::verbosityLevel, jutils::log::verbosityLevelToString);

#ifndef JUTILS_LOG_DISABLED
    #if JUTILS_VA_OPT_SUPPORTED // __VA_OPT__
        #define JUTILS_LOG(type, formatStr, ...) jutils::log::print(jutils::format(                                 \
                "{} {}\n", jutils::log::verbosityLevel::type, jutils::format(formatStr __VA_OPT__(,) __VA_ARGS__)   \
            ))
    #else
        #define JUTILS_LOG(type, ...) jutils::log::print(jutils::format(                    \
                "{} {}\n", jutils::log::verbosityLevel::type, jutils::format(__VA_ARGS__)   \
            ))
    #endif
#else
    #define JUTILS_LOG(type, formatStr, ...)
#endif
