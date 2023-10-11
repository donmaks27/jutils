// Copyright © 2021-2023 Leonov Maksim. All Rights Reserved.

#pragma once

/**
 * #define JUTILS_LOG_DISABLED - disable printing logs
 */

#include "jstring.h"

#if defined(JUTILS_LOG_COLOR_ENABLED) && defined(JUTILS_USE_FMT)
    #include <fmt/color.h>
#endif

#ifndef JUTILS_LOG_DISABLED
    #if !defined(_MSC_VER) && defined(__FILE_NAME__)
        #define JUTILS_FILENAME __FILE_NAME__
    #else
        #define JUTILS_FILENAME __FILE__
    #endif
#endif

namespace jutils
{
    namespace log
    {
        enum class verbosityLevel : uint8 { error, warning, info };

#if defined(JUTILS_LOG_COLOR_ENABLED) && defined(JUTILS_USE_FMT)
        constexpr jstring verbosityLevelToString(const verbosityLevel verbosity) noexcept
        {
            switch (verbosity)
            {
            case verbosityLevel::error:   return fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::red),    JSTR("[ERR] "));
            case verbosityLevel::warning: return fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::yellow), JSTR("[WARN]"));
            case verbosityLevel::info:    return fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::green),  JSTR("[INFO]"));
            default: ;
            }
            return JSTR("      ");
        }
#else
        constexpr const char* verbosityLevelToString(const verbosityLevel verbosity) noexcept
        {
            switch (verbosity)
            {
            case verbosityLevel::error:   return JSTR("[ERR] ");
            case verbosityLevel::warning: return JSTR("[WARN]");
            case verbosityLevel::info:    return JSTR("[INFO]");
            default: ;
            }
            return JSTR("      ");
        }
#endif

        inline void print(const char* str) { std::printf("%s", str); }
        inline void print(const std::string& str) { jutils::log::print(str.c_str()); }
        inline void print(const jstring& str) { jutils::log::print(*str); }
    }
}

JUTILS_STRING_FORMATTER(jutils::log::verbosityLevel, jutils::log::verbosityLevelToString);

#ifndef JUTILS_LOG_DISABLED

#define JUTILS_LOG(type, formatStr, ...) jutils::log::print(JSTR_FORMAT(                                                    \
        JSTR("{} {}({}): {}\n"), jutils::log::verbosityLevel::type, static_cast<const char*>(JUTILS_FILENAME), __LINE__,   \
            JSTR_FORMAT(formatStr, __VA_ARGS__)                                                                             \
    ))
#define JUTILS_ERROR_LOG(errorCode, formatStr, ...) jutils::log::print(JSTR_FORMAT(                                         \
        JSTR("{} {}({}): Code {:#x}. {}\n"), jutils::log::verbosityLevel::error, static_cast<const char*>(JUTILS_FILENAME),\
            __LINE__, static_cast<jutils::int64>(errorCode), JSTR_FORMAT(formatStr, __VA_ARGS__)                            \
    ))

#else

#define JUTILS_LOG(type, formatStr, ...)
#define JUTILS_ERROR_LOG(errorCode, formatStr, ...)

#endif
