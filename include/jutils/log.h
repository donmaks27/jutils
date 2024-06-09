// Copyright © 2021-2024 Leonov Maksim. All Rights Reserved.

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
    enum class verbosityLevel : uint8 { error, warning, info };

#if defined(JUTILS_LOG_COLOR_ENABLED)
    constexpr const char* verbosityLevelToString(const verbosityLevel verbosity) noexcept
    {
        switch (verbosity)
        {
            case verbosityLevel::error:   return "\033[1;38;5;9m[ERR] \033[0m";
            case verbosityLevel::warning: return "\033[1;38;5;11m[WARN]\033[0m";
            case verbosityLevel::info:    return "\033[1;38;5;10m[INFO]\033[0m";
            default: ;
        }
        return "      ";
    }
#else
    constexpr const char* verbosityLevelToString(const verbosityLevel verbosity) noexcept
    {
        switch (verbosity)
        {
        case verbosityLevel::error:   return "[ERR] ";
        case verbosityLevel::warning: return "[WARN]";
        case verbosityLevel::info:    return "[INFO]";
        default: ;
        }
        return "      ";
    }
#endif

    inline void print(const char* str) { std::printf("%s", str); }
    inline void print(const std::string& str) { jutils::log::print(str.c_str()); }
    inline void print(const jstring& str) { jutils::log::print(*str); }
}

JUTILS_STRING_FORMATTER_CONSTEXPR(jutils::log::verbosityLevel, jutils::log::verbosityLevelToString);

#ifndef JUTILS_LOG_DISABLED
    #if JUTILS_VA_OPT_SUPPORTED // __VA_OPT__
        #define JUTILS_LOG(type, formatStr, ...) jutils::log::print(jutils::format(                                         \
                "{} {}({}): {}\n", jutils::log::verbosityLevel::type, static_cast<const char*>(JUTILS_FILENAME), __LINE__,  \
                    jutils::format(formatStr __VA_OPT__(,) __VA_ARGS__)                                                     \
            ))
        #define JUTILS_ERROR_LOG(errorCode, formatStr, ...) jutils::log::print(jutils::format(                                  \
                "{} {}({}): Code {:#x}. {}\n", jutils::log::verbosityLevel::error, static_cast<const char*>(JUTILS_FILENAME),   \
                    __LINE__, static_cast<jutils::int64>(errorCode), jutils::format(formatStr __VA_OPT__(,) __VA_ARGS__)        \
            ))
    #else
        #define JUTILS_LOG(type, ...) jutils::log::print(jutils::format(                                                    \
                "{} {}({}): {}\n", jutils::log::verbosityLevel::type, static_cast<const char*>(JUTILS_FILENAME), __LINE__,  \
                    jutils::format(__VA_ARGS__)                                                                             \
            ))
        #define JUTILS_ERROR_LOG(errorCode, ...) jutils::log::print(jutils::format(                                             \
                "{} {}({}): Code {:#x}. {}\n", jutils::log::verbosityLevel::error, static_cast<const char*>(JUTILS_FILENAME),   \
                    __LINE__, static_cast<jutils::int64>(errorCode), jutils::format(__VA_ARGS__)                                \
            ))
    #endif
#else
    #define JUTILS_LOG(type, formatStr, ...)
    #define JUTILS_ERROR_LOG(errorCode, formatStr, ...)
#endif
