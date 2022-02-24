// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

/**
 * #define JUTILS_LOG_DISABLED - disable all logs
 */

#include "jstring.h"

namespace jutils
{
    namespace jlog
    {
#if _WIN32
        constexpr const char* logPrefix_error   = JSTR("\033[31m[ERR]  \033[0m");
        constexpr const char* logPrefix_warning = JSTR("\033[93m[WARN] \033[0m");
        constexpr const char* logPrefix_info    = JSTR("\033[97m[INFO] \033[0m");
        constexpr const char* logPrefix_correct = JSTR("\033[92m[OK]   \033[0m");
#else
        constexpr const char* logPrefix_error   = JSTR("[ERR]  ");
        constexpr const char* logPrefix_warning = JSTR("[WARN] ");
        constexpr const char* logPrefix_info    = JSTR("[INFO] ");
        constexpr const char* logPrefix_correct = JSTR("[OK]   ");
#endif

#ifndef JUTILS_LOG_DISABLED

        extern void writeLog(const char* prefix, const char* method = nullptr, const char* message = nullptr);
        inline void writeLog(const char* const prefix, const jstring& method, const char* message = nullptr) { writeLog(prefix, *method, message); }
        inline void writeLog(const char* const prefix, const char* const method, const jstring& message) { writeLog(prefix, method, *message); }
        inline void writeLog(const char* const prefix, const jstring& method, const jstring& message) { writeLog(prefix, *method, *message); }
        
#define JUTILS_LOG_WRITE_CUSTOM(prefix, message) jutils::jlog::writeLog(prefix, jutils::jstring(__FUNCTION__) + JSTR("(") + TO_JSTR(__LINE__) + JSTR(")"), message)
#define JUTILS_LOG_WRITE(type, message) JUTILS_LOG_WRITE_CUSTOM(jutils::jlog::logPrefix_##type, message)
#define JUTILS_LOG_WRITE_EMPTY(type) JUTILS_LOG_WRITE(type, nullptr)

#else

#define JUTILS_LOG_WRITE_CUSTOM(prefix, message)
#define JUTILS_LOG_WRITE(type, message)
#define JUTILS_LOG_WRITE_EMPTY(type)

#endif
    }
}