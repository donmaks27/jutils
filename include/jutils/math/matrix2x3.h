﻿// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types/matrix2x3_base.h"

namespace jutils
{
    namespace math
    {
        using matrix2x3 = matrix<2, 3, float>;
    }
}

JUTILS_STRING_FORMATTER_NOT_CONSTEXPR(jutils::math::matrix2x3, jutils::math::matrixToString);
