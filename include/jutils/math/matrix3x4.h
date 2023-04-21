// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types/matrix3x4_base.h"

namespace jutils
{
    namespace math
    {
        using matrix3x4 = matrix<3, 4, float>;
    }
}

JUTILS_STRING_FORMATTER_NOT_CONSTEXPR(jutils::math::matrix3x4, jutils::math::matrixToString);
