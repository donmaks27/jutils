// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types/matrix4x4_base.h"

namespace jutils
{
    namespace math
    {
        template<typename T = float>
        auto viewMatrix_lookAt(const vector<3, T>& viewPosition, const vector<3, T>& targetPosition, const vector<3, T>& upDirection) noexcept
        {
            using R = std::conditional_t<std::is_floating_point_v<T>, T, float>;

            const vector<3, R> forward = (targetPosition.template copy<R>() - viewPosition).normalize();
            vector<3, R> right;
            if constexpr (std::is_same_v<T, R>)
            {
                right = upDirection.cross(forward).normalize();
            }
            else
            {
                right = upDirection.template copy<R>().cross(forward).normalize();
            }
            const vector<3, R> up = forward.cross(right);

            matrix<4, 4, R> result(static_cast<R>(1));
            result[0][0] = right.x; result[0][1] = up.x; result[0][2] = forward.x;
            result[1][0] = right.y; result[1][1] = up.y; result[1][2] = forward.y;
            result[2][0] = right.z; result[2][1] = up.z; result[2][2] = forward.z;
            result[3][0] = -right.dot(viewPosition);
            result[3][1] = -up.dot(viewPosition);
            result[3][2] = -forward.dot(viewPosition);
            return result;
        }

        template<typename T = float> requires std::floating_point<T>
        matrix<4, 4, T> projectionMatrix_perspective(const T angleFOV, const T aspect, const T zNear, const T zFar, const bool invertVertically = false) noexcept
        {
            if (jutils::math::isNearlyZero(aspect, 0.0f) || jutils::math::isEqual(zNear, zFar))
            {
                return matrix<4, 4, T>(static_cast<T>(1));
            }
            const T angleTan = math::tan(angleFOV / 2);
            matrix<4, 4, T> matrix;
            matrix[0][0] = static_cast<T>(1) / (aspect * angleTan);
            matrix[1][1] = -static_cast<T>(1) / angleTan;
            matrix[2][2] = zFar / (zFar - zNear);
            matrix[2][3] = static_cast<T>(1);
            matrix[3][2] = -zFar * zNear / (zFar - zNear);
            if (invertVertically)
            {
                matrix[1][1] = -matrix[1][1];
            }
            return matrix;
        }
        template<typename T = float> requires std::floating_point<T>
        constexpr matrix<4, 4, T> projectionMatrix_orthogonal(const vector<2, T>& viewSize, const T zNear, const T zFar, const bool invertVertically = false)
        {
            if (jutils::math::isNearlyZero(viewSize.x) || jutils::math::isNearlyZero(viewSize.y) || jutils::math::isEqual(zNear, zFar))
            {
                return matrix<4, 4, T>(static_cast<T>(1));
            }
            matrix<4, 4, T> result(static_cast<T>(1));
            result[0][0] = static_cast<T>(2) / viewSize.x;
            result[1][1] = -static_cast<T>(2) / viewSize.y;
            result[2][2] = static_cast<T>(1) / (zFar - zNear);
            result[3][0] = 0; result[3][1] = 0; result[3][2] = -zNear / (zFar - zNear);
            if (invertVertically)
            {
                result[1][1] = -result[1][1];
            }
            return result;
        }
    }
}
