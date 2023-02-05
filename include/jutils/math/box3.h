// Copyright © 2022-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types/box3_base.h"

namespace jutils
{
    namespace math
    {
        using box3 = box<3, float>;
        using ibox3 = box<3, int32>;
        using ubox3 = box<3, uint32>;
    }
}

JUTILS_STRING_FORMATTER(jutils::math::box3, jutils::math::boxToString);
JUTILS_STRING_FORMATTER(jutils::math::ibox3, jutils::math::boxToString);
JUTILS_STRING_FORMATTER(jutils::math::ubox3, jutils::math::boxToString);
