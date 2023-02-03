// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vector_base.h"

namespace jutils
{
    namespace math
    {
        template<vector_size_type Size, typename Type>
        class box;

        template<vector_size_type Size, typename Type>
        jstring boxToString(const box<Size, Type>& value) { return value.toString(); }
    }
}
