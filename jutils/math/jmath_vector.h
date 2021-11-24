// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jmath.h"
#include "vector3.h"

namespace jutils
{
    namespace math
    {
        template<typename Type>
        constexpr float lengthSqr(const vector<2, Type>& value) { return value.x * value.x + value.y * value.y; }
        template<typename Type>
        constexpr float lengthSqr(const vector<3, Type>& value) { return value.x * value.x + value.y * value.y + value.z * value.z; }
        template<typename Type>
        constexpr float lengthSqr(const vector<4, Type>& value) { return value.x * value.x + value.y * value.y + value.z * value.z + value.w * value.w; }
        template<vector_size_type Size, typename Type>
        constexpr float length(const vector<Size, Type>& value) { return math::sqrt(lengthSqr(value)); }

        template<vector_size_type Size>
        constexpr vector<Size, float> normal(const vector<Size, float>& value, const float eps = EPSILON_DEFAULT)
        {
            const float lenSqr = math::lengthSqr(value);
            return math::isEqual(lenSqr, 0.0f, eps) ? vector<Size, float>(0.0f) : value / math::sqrt(lenSqr);
        }

        template<vector_size_type Size>
        constexpr float dot(const vector<Size, float>& value1, const vector<Size, float>& value2)
        {
            float sum = 0.0f;
            for (vector_size_type i = 0; i < Size; i++)
            {
                sum += value1[i] * value2[i];
            }
            return sum;
        }
        constexpr vector3 cross(const vector3& value1, const vector3& value2)
        {
            return {
                value1.y * value2.z - value1.z * value2.y,
                value1.z * value2.x - value1.x * value2.z,
                value1.x * value2.y - value1.y * value2.x
            };
        }
    }
}
