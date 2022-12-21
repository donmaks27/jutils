// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vector_base.h"

namespace jutils
{
    namespace math
    {
        template<vector_size_type Rows, vector_size_type Columns, typename Type>
        class matrix;

        template<vector_size_type Rows, vector_size_type Columns, typename Type>
        jstring matrixToString(const matrix<Rows, Columns, Type>& value) { return value.toString(); }
    }
}
