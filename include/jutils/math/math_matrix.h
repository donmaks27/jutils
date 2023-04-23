// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "matrix4.h"
#include "vector2.h"
#include "vector3.h"

namespace jutils
{
    namespace math
    {
        template<vector_size_type Rows, vector_size_type Columns, typename Type1, typename Type2>
        constexpr bool isMatricesEqual(const math::matrix<Rows, Columns, Type1>& value1, const math::matrix<Rows, Columns, Type2>& value2, const float eps = EpsDefault<>)
        {
            for (vector_size_type i = 0; i < Rows; i++)
            {
                if (value1.rows[i] != value2.rows[i])
                {
                    return false;
                }
            }
            return true;
        }

        inline math::matrix4 viewMatrix_lookAt(const math::vector3& viewPosition, const math::vector3& targetPosition, const math::vector3& upDirection)
        {
            const math::vector3 forward = (targetPosition - viewPosition).normalize();
            const math::vector3 right = upDirection.cross(forward).normalize();
            const math::vector3 up = forward.cross(right);
            math::matrix4 result(1.0f);
            result[0][0] = right.x; result[0][1] = up.x; result[0][2] = forward.x;
            result[1][0] = right.y; result[1][1] = up.y; result[1][2] = forward.y;
            result[2][0] = right.z; result[2][1] = up.z; result[2][2] = forward.z;
            result[3][0] = -right.dot(viewPosition);
            result[3][1] = -up.dot(viewPosition);
            result[3][2] = -forward.dot(viewPosition);
            return result;
        }

        inline math::matrix4 projectionMatrix_perspective(const float angleFOV, const float aspect, const float zNear, const float zFar, const bool invertVertically = false)
        {
            if (math::isEqual(aspect, 0.0f) || jutils::math::isEqual(zNear, zFar))
            {
                return math::matrix4(1.0f);
            }
            const float angleTan = math::tan(angleFOV / 2);
            math::matrix4 matrix(0.0f);
            matrix[0][0] = 1.0f / (aspect * angleTan);
            matrix[1][1] = -1.0f / angleTan;
            matrix[2][2] = zFar / (zFar - zNear);
            matrix[2][3] = 1.0f;
            matrix[3][2] = -zFar * zNear / (zFar - zNear);
            if (invertVertically)
            {
                matrix[1][1] = -matrix[1][1];
            }
            return matrix;
        }
        constexpr matrix4 projectionMatrix_orthogonal(const math::vector2& viewSize, const float zNear, const float zFar, const bool invertVertically = false)
        {
            if (math::isEqual(viewSize.x, 0.0f) || math::isEqual(viewSize.y, 0.0f) || math::isEqual(zNear, zFar))
            {
                return math::matrix4(1.0f);
            }
            math::matrix4 matrix(1.0f);
            matrix[0][0] = 2.0f / viewSize.x;
            matrix[1][1] = -2.0f / viewSize.y;
            matrix[2][2] = 1.0f / (zFar - zNear);
            matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = -zNear / (zFar - zNear);
            if (invertVertically)
            {
                matrix[1][1] = -matrix[1][1];
            }
            return matrix;
        }
    }
}
