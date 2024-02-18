// Copyright © 2021-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types/matrix4x4_base.h"

namespace jutils
{
    namespace math
    {
        using matrix4x4 = matrix<4, 4, float>;
        using imatrix4x4 = matrix<4, 4, int32>;
        using umatrix4x4 = matrix<4, 4, uint32>;

        using matrix4 = matrix4x4;
        using imatrix4 = imatrix4x4;
        using umatrix4 = umatrix4x4;
    }
}
