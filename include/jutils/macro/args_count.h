// Copyright © 2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../core.h"

#define JUTILS_ARGS_COUNT_SUPPORTED JUTILS_VA_OPT_SUPPORTED

#if JUTILS_ARGS_COUNT_SUPPORTED
    #define JUTILS_ARGS_COUNT_HELPER(                                               \
         a1, a2, a3, a4, a5, a6, a7, a8, a9,a10,a11,a12,a13,a14,a15,a16,            \
        a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31,a32,            \
        a33,a34,a35,a36,a37,a38,a39,a40,a41,a42,a43,a44,a45,a46,a47,a48,            \
        a49,a50,a51,a52,a53,a54,a55,a56,a57,a58,a59,a60,a61,a62,a63,a64, N, ...) N
    #define JUTILS_ARGS_COUNT(...) JUTILS_ARGS_COUNT_HELPER(__VA_ARGS__ __VA_OPT__(,)   \
         64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49,                \
         48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33,                \
         32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17,                \
         16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1, 0)
#endif
