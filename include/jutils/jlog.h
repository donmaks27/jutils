// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

/**
 * #define JUTILS_LOG_DISABLED - disable printing logs
 */

#include "jstring.h"

#include <fmt/color.h>

namespace jutils
{
    namespace jlog
    {
        using color = fmt::color;

        template<typename... Args>
        inline jstring formatColor(const jutils::jlog::color strColor, const char* formatStr, Args&&... args)
        {
            return jstring( fmt::format(fmt::fg(strColor), formatStr, std::forward<Args>(args)...) );
        }
        template<>
        inline jstring formatColor<>(const jutils::jlog::color strColor, const char* formatStr)
        {
            return jstring( fmt::format(fmt::fg(strColor), formatStr) );
        }

        template<typename... Args>
        inline void print(const char* formatStr, Args&&... args)
        {
#ifndef JUTILS_LOG_DISABLED
            fmt::print(formatStr, std::forward<Args>(args)...);
#endif
        }
        template<typename... Args>
        inline void print(const jutils::jlog::color strColor, const char* formatStr, Args&&... args)
        {
#ifndef JUTILS_LOG_DISABLED
            fmt::print(fmt::fg(strColor), formatStr, std::forward<Args>(args)...);
#endif
        }

        template<typename... Args>
        inline void printTrace(const jstring& prefix, const char* functionName, const uint32 lineIndex, const char* message, Args&&... args)
        {
#ifndef JUTILS_LOG_DISABLED
            jutils::jlog::print("{} {}({}): {}\n", prefix, functionName, lineIndex, jutils::jstring::format(message, std::forward<Args>(args)...));
#endif
        }
#ifndef JUTILS_LOG_DISABLED
        template<>
        inline void printTrace<>(const jstring& prefix, const char* functionName, const uint32 lineIndex, const char* message)
        {
            jutils::jlog::print("{} {}({}): {}\n", prefix, functionName, lineIndex, message);
        }
#endif

        template<typename... Args>
        inline void printTrace_error(const char* functionName, const uint32 lineIndex, Args&&... args)
        {
#ifndef JUTILS_LOG_DISABLED
            jutils::jlog::printTrace(
                jutils::jlog::formatColor(jutils::jlog::color::red, "[ERR] "), functionName, lineIndex, std::forward<Args>(args)...
            );
#endif
        }
        template<typename... Args>
        inline void printTrace_warning(const char* functionName, const uint32 lineIndex, Args&&... args)
        {
#ifndef JUTILS_LOG_DISABLED
            jutils::jlog::printTrace(
                jutils::jlog::formatColor(jutils::jlog::color::yellow, "[WARN]"), functionName, lineIndex, std::forward<Args>(args)...
            );
#endif
        }
        template<typename... Args>
        inline void printTrace_info(const char* functionName, const uint32 lineIndex, Args&&... args)
        {
#ifndef JUTILS_LOG_DISABLED
            jutils::jlog::printTrace(
                jutils::jlog::formatColor(jutils::jlog::color::white_smoke, "[INFO]"), functionName, lineIndex, std::forward<Args>(args)...
            );
#endif
        }
        template<typename... Args>
        inline void printTrace_correct(const char* functionName, const uint32 lineIndex, Args&&... args)
        {
#ifndef JUTILS_LOG_DISABLED
            jutils::jlog::printTrace(
                jutils::jlog::formatColor(jutils::jlog::color::green, "[OK]  "), functionName, lineIndex, std::forward<Args>(args)...
            );
#endif
        }

        template<typename... Args>
        inline void printTrace_errorCode(const char* functionName, const uint32 lineIndex, const int32 errorCode, const char* message, Args&&... args)
        {
#ifndef JUTILS_LOG_DISABLED
            jutils::jlog::printTrace_error(functionName, lineIndex, "Code {:#x}. {}", errorCode, jutils::jstring::format(message, std::forward<Args>(args)...));
#endif
        }
#ifndef JUTILS_LOG_DISABLED
        template<>
        inline void printTrace_errorCode<>(const char* functionName, const uint32 lineIndex, const int32 errorCode, const char* message)
        {
            jutils::jlog::printTrace_error(functionName, lineIndex, "Code {:#x}. {}", errorCode, message);
        }
#endif
    }
}

#ifndef JUTILS_LOG_DISABLED

#define JUTILS_LOG(type, ...) jutils::jlog::printTrace_##type(__FUNCTION__, __LINE__, __VA_ARGS__)
#define JUTILS_ERROR_LOG(errorCode, ...) jutils::jlog::printTrace_errorCode(__FUNCTION__, __LINE__, errorCode, __VA_ARGS__)

#else

#define JUTILS_LOG(type, ...)
#define JUTILS_ERROR_LOG(errorCode, ...)

#endif
