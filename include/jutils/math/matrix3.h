// Copyright © 2021-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types/matrix3x3_base.h"

namespace jutils
{
    namespace math
    {
        using matrix3x3 = matrix<3, 3, float>;
        using imatrix3x3 = matrix<3, 3, int32>;
        using umatrix3x3 = matrix<3, 3, uint32>;

        using matrix3 = matrix3x3;
        using imatrix3 = imatrix3x3;
        using umatrix3 = umatrix3x3;
    }
}
