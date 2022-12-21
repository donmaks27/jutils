// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types/matrix4x2_base.h"

namespace jutils
{
    namespace math
    {
        using matrix4x2 = matrix<4, 2, float>;
    }
}

JUTILS_STRING_FORMATTER(jutils::math::matrix4x2, jutils::math::matrixToString);
