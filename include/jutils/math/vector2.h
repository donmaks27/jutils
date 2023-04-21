// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types/vector2_base.h"

namespace jutils
{
    namespace math
    {
        using vector2 = vector<2, float>;
        using ivector2 = vector<2, int32>;
        using uvector2 = vector<2, uint32>;
    }
}

JUTILS_STRING_FORMATTER_NOT_CONSTEXPR(jutils::math::vector2, jutils::math::vectorToString);
JUTILS_STRING_FORMATTER_NOT_CONSTEXPR(jutils::math::ivector2, jutils::math::vectorToString);
JUTILS_STRING_FORMATTER_NOT_CONSTEXPR(jutils::math::uvector2, jutils::math::vectorToString);
