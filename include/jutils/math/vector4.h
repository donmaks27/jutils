// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types/vector4_base.h"

namespace jutils
{
    namespace math
    {
        using vector4 = vector<4, float>;
        using ivector4 = vector<4, int32>;
        using uvector4 = vector<4, uint32>;
    }
}

JUTILS_STRING_FORMATTER_NOT_CONSTEXPR(jutils::math::vector4, jutils::math::vectorToString);
JUTILS_STRING_FORMATTER_NOT_CONSTEXPR(jutils::math::ivector4, jutils::math::vectorToString);
JUTILS_STRING_FORMATTER_NOT_CONSTEXPR(jutils::math::uvector4, jutils::math::vectorToString);
