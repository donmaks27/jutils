// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

/**
 * #define JUTILS_LOG_DISABLED - disable printing logs
 */

#include "jstring.h"

namespace jutils
{
    namespace log
    {
        enum class verbosityLevel : uint8 { error, warning, info };

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

        inline void print(const char* str) { std::printf(str); }
        inline void print(const std::string& str) { jutils::log::print(str.c_str()); }
        inline void print(const jstring& str) { jutils::log::print(*str); }
    }
}

JUTILS_STRING_FORMATTER(jutils::log::verbosityLevel, jutils::log::verbosityLevelToString);

#ifndef JUTILS_LOG_DISABLED

#define JUTILS_LOG(type, formatStr, ...) jutils::log::print(JSTR_FORMAT(                    \
        JSTR("{} {}({}): {}\n"), jutils::log::verbosityLevel::type, __FUNCTION__, __LINE__,   \
            JSTR_FORMAT(formatStr, __VA_ARGS__)                                             \
    ))
#define JUTILS_ERROR_LOG(errorCode, formatStr, ...) jutils::log::print(JSTR_FORMAT(                                                  \
        JSTR("{} {}({}): Code {:#x}. {}\n"), jutils::log::verbosityLevel::error, __FUNCTION__, __LINE__, static_cast<int64>(errorCode),\
            JSTR_FORMAT(formatStr, __VA_ARGS__)                                                                                      \
    ))

#else

#define JUTILS_LOG(type, formatStr, ...)
#define JUTILS_ERROR_LOG(errorCode, formatStr, ...)

#endif
