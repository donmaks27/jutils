// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types/matrix2x4_base.h"

namespace jutils
{
    namespace math
    {
        using matrix2x4 = matrix<2, 4, float>;
    }
}

JUTILS_STRING_FORMATTER_NOT_CONSTEXPR(jutils::math::matrix2x4, jutils::math::matrixToString);
