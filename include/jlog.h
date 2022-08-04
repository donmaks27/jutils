// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

/**
 * #define JUTILS_LOG_DISABLED - disable all logs
 */

#ifndef JUTILS_LOG_DISABLED

#include <fmt/color.h>

#include "jstring.h"

namespace jutils
{
    namespace jlog
    {
        template<typename... Args>
        inline void writeLog(const std::string& prefix, const char* functionName, const uint32 lineIndex, const char* message, Args&&... args)
        {
            fmt::print("{} {}({}): {}\n", prefix, functionName, lineIndex, fmt::format(message, std::forward<Args>(args)...));
        }
        template<typename... Args>
        inline void writeLog_error(const char* functionName, const uint32 lineIndex, const char* message, Args&&... args)
        {
            jutils::jlog::writeLog(fmt::format(fmt::fg(fmt::color::red), "[ERR] "), 
                functionName, lineIndex, message, std::forward<Args>(args)...
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
    }
}

#define JUTILS_LOG(type, ...) jutils::jlog::writeLog_##type(__FUNCTION__, __LINE__, __VA_ARGS__)

#else

#define JUTILS_LOG(type, message)

#endif
