// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../../jstring.h"

namespace jutils
{
    namespace math
    {
        using vector_size_type = int8;

        template<vector_size_type Size, typename Type>
        class vector;

        template<vector_size_type Size, typename Type>
        jstring vectorToString(const vector<Size, Type>& value) { return value.toString(); }
    }
}