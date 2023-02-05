// Copyright © 2022-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types/box2_base.h"

namespace jutils
{
    namespace math
    {
        using box2 = box<2, float>;
        using ibox2 = box<2, int32>;
        using ubox2 = box<2, uint32>;
    }
}

JUTILS_STRING_FORMATTER(jutils::math::box2, jutils::math::boxToString);
JUTILS_STRING_FORMATTER(jutils::math::ibox2, jutils::math::boxToString);
JUTILS_STRING_FORMATTER(jutils::math::ubox2, jutils::math::boxToString);
