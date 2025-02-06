// Copyright © 2025 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#ifndef JUTILS_LOG_DISABLED
    #if JUTILS_VA_OPT_SUPPORTED // __VA_OPT__
        #define JUTILS_LOG(logType, formatStr, ...) jutils::log::print(jutils::format(                      \
                "{} {}\n", jutils::log::type::logType, jutils::format(formatStr __VA_OPT__(,) __VA_ARGS__)  \
            ))
    #else
        #define JUTILS_LOG(logType, ...) jutils::log::print(jutils::format(         \
                "{} {}\n", jutils::log::type::logType, jutils::format(__VA_ARGS__)  \
            ))
    #endif
#else
    #define JUTILS_LOG(logType, formatStr, ...)
#endif