// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

/**
 * #define JUTILS_LOG_DISABLED - disable all logs
 */

#include "jstring.h"

#ifndef JUTILS_LOG_DISABLED

#include <fmt/color.h>

namespace jutils
{
    namespace jlog
    {
        template<typename... Args>
        inline void writeLog(const std::string& prefix, const char* functionName, const uint32 lineIndex, const char* message, Args&&... args)
        {
            fmt::print("{} {}({}): {}\n", prefix, functionName, lineIndex, fmt::format(message, std::forward<Args>(args)...));
        }
        template<>
        inline void writeLog<>(const std::string& prefix, const char* functionName, const uint32 lineIndex, const char* message)
        {
            fmt::print("{} {}({}): {}\n", prefix, functionName, lineIndex, message);
        }

        template<typename... Args>
        inline void writeLog_error(const char* functionName, const uint32 lineIndex, Args&&... args)
        {
            jutils::jlog::writeLog(fmt::format(fmt::fg(fmt::color::red), "[ERR] "), 
                functionName, lineIndex, std::forward<Args>(args)...
            );
        }
        template<typename... Args>
        inline void writeLog_warning(const char* functionName, const uint32 lineIndex, const char* message, Args&&... args)
        {
            jutils::jlog::writeLog(fmt::format(fmt::fg(fmt::color::yellow), "[WARN]"), 
                functionName, lineIndex, message, std::forward<Args>(args)...
            );
        }
        template<typename... Args>
        inline void writeLog_info(const char* functionName, const uint32 lineIndex, const char* message, Args&&... args)
        {
            jutils::jlog::writeLog(fmt::format(fmt::fg(fmt::color::white_smoke), "[INFO]"), 
                functionName, lineIndex, message, std::forward<Args>(args)...
            );
        }
        template<typename... Args>
        inline void writeLog_correct(const char* functionName, const uint32 lineIndex, const char* message, Args&&... args)
        {
            jutils::jlog::writeLog(fmt::format(fmt::fg(fmt::color::green), "[OK]  "), 
                functionName, lineIndex, message, std::forward<Args>(args)...
            );
        }

        template<typename... Args>
        inline void writeLog_errorCode(const char* functionName, const uint32 lineIndex, const int32 errorCode, const char* message, Args&&... args)
        {
            jutils::jlog::writeLog_error(functionName, lineIndex, "Code {:#x}. {}", errorCode, fmt::format(message, std::forward<Args>(args)...));
        }
        template<>
        inline void writeLog_errorCode<>(const char* functionName, const uint32 lineIndex, const int32 errorCode, const char* message)
        {
            jutils::jlog::writeLog_error(functionName, lineIndex, "Code {:#x}. {}", errorCode, message);
        }
    }
}

#define JUTILS_LOG(type, ...) jutils::jlog::writeLog_##type(__FUNCTION__, __LINE__, __VA_ARGS__)
#define JUTILS_ERROR_LOG(errorCode, ...) jutils::jlog::writeLog_errorCode(__FUNCTION__, __LINE__, errorCode, __VA_ARGS__)

#else

#define JUTILS_LOG(type, ...)
#define JUTILS_ERROR_LOG(errorCode, ...)

#endif
