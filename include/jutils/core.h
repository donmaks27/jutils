// Copyright © 2025 Leonov Maksim. All Rights Reserved.

#pragma once

#define JUTILS_STD11 201103L
#define JUTILS_STD14 201402L
#define JUTILS_STD17 201703L
#define JUTILS_STD20 202002L
#define JUTILS_STD_VERSION __cplusplus

#if JUTILS_STD_VERSION >= JUTILS_STD20
    #define JUTILS_STD20_CONSTEXPR constexpr

    #define JUTILS_HELPER_VA_OPT_SUPPORTED_GET_THIRD(a1, a2, a3, ...) a3
    #define JUTILS_HELPER_VA_OPT_SUPPORTED(...) JUTILS_HELPER_VA_OPT_SUPPORTED_GET_THIRD(__VA_OPT__(,),true,false,)
    #define JUTILS_VA_OPT_SUPPORTED JUTILS_HELPER_VA_OPT_SUPPORTED(0)

    #if !__has_include(<format>)
        // Force using fmt lib
        #define JUTILS_USE_FMT
    #endif
#else
    #define JUTILS_STD20_CONSTEXPR

    #define JUTILS_VA_OPT_SUPPORTED false

    // Force using fmt lib
    #define JUTILS_USE_FMT
#endif

#ifdef JUTILS_USE_FMT
    #define JUTILS_FORMAT_HEADER <fmt/format.h>
    #define JUTILS_FORMAT_NAMESPACE fmt
#else
    #define JUTILS_FORMAT_HEADER <format>
    #define JUTILS_FORMAT_NAMESPACE std
#endif

#ifndef JUTILS_LOG_DISABLED
    #if !defined(_MSC_VER) && defined(__FILE_NAME__)
        #define JUTILS_FILENAME __FILE_NAME__
    #else
        #define JUTILS_FILENAME __FILE__
    #endif
#else
    #define JUTILS_FILENAME
#endif

#define JUTILS_HELPER_CONCAT(...) __VA_ARGS__
