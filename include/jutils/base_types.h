// Copyright © 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#ifndef JUTILS_USE_MODULES
    #include <cstdint>
    #include <cstddef>
#endif

JUTILS_MODULE_EXPORT namespace jutils
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
