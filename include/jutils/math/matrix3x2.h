// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types/matrix3x2_base.h"

namespace jutils
{
    namespace math
    {
        using matrix3x2 = matrix<3, 2, float>;
    }
}

JUTILS_STRING_FORMATTER(jutils::math::matrix3x2, jutils::math::matrixToString);
