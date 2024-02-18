// Copyright © 2021-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types/matrix2x2_base.h"

namespace jutils
{
    namespace math
    {
        using matrix2x2 = matrix<2, 2, float>;
        using imatrix2x2 = matrix<2, 2, int32>;
        using umatrix2x2 = matrix<2, 2, uint32>;

        using matrix2 = matrix2x2;
        using imatrix2 = imatrix2x2;
        using umatrix2 = umatrix2x2;
    }
}
