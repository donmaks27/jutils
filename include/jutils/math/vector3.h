// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types/vector3_base.h"

namespace jutils
{
    namespace math
    {
        using vector3 = vector<3, float>;
        using ivector3 = vector<3, int32>;
        using uvector3 = vector<3, uint32>;
    }
}

JUTILS_STRING_FORMATTER(jutils::math::vector3, jutils::math::vectorToString);
JUTILS_STRING_FORMATTER(jutils::math::ivector3, jutils::math::vectorToString);
JUTILS_STRING_FORMATTER(jutils::math::uvector3, jutils::math::vectorToString);