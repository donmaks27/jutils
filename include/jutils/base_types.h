// Copyright © 2021-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include <cstdint>

#define JUTILS_STD20 202002L
#define JUTILS_STD_VERSION __cplusplus

#if JUTILS_STD_VERSION >= JUTILS_STD20
    #define JUTILS_STD20_CONSTEXPR constexpr
#else
    #define JUTILS_STD20_CONSTEXPR
    // Force using fmt lib
    #define JUTILS_USE_FMT
#endif

namespace jutils
{
    using int8 = std::int8_t;
    using uint8 = std::uint8_t;
    using int16 = std::int16_t;
    using uint16 = std::uint16_t;
    using int32 = std::int32_t;
    using uint32 = std::uint32_t;
    using int64 = std::int64_t;
    using uint64 = std::uint64_t;

    using index_type = std::size_t;
    constexpr index_type index_invalid = -1;
}
