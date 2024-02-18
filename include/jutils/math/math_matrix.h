// Copyright © 2021-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types/matrix_base.h"

namespace jutils::math
{
    template<vector_size_type R, vector_size_type C, typename T, typename T1>
    [[nodiscard]] constexpr matrix<R, C, T> operator+(const matrix<R, C, T>& value1, const matrix<R, C, T1>& value2) noexcept { return value1.copy() += value2; }
    template<vector_size_type R, vector_size_type C, typename T, typename T1>
    [[nodiscard]] constexpr matrix<R, C, T> operator+(matrix<R, C, T>&& value1, const matrix<R, C, T1>& value2) noexcept { return value1 += value2; }

    template<vector_size_type R, vector_size_type C, typename T, typename T1>
    [[nodiscard]] constexpr matrix<R, C, T> operator-(const matrix<R, C, T>& value1, const matrix<R, C, T1>& value2) noexcept { return value1.copy() -= value2; }
    template<vector_size_type R, vector_size_type C, typename T, typename T1>
    [[nodiscard]] constexpr matrix<R, C, T> operator-(matrix<R, C, T>&& value1, const matrix<R, C, T1>& value2) noexcept { return value1 -= value2; }

    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1>, vector_size_type R, vector_size_type C, typename T, typename T1)
    [[nodiscard]] constexpr matrix<R, C, T> operator*(const matrix<R, C, T>& value1, const T1 value2) noexcept { return value1.copy() *= value2; }
    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1>, vector_size_type R, vector_size_type C, typename T, typename T1)
    [[nodiscard]] constexpr matrix<R, C, T> operator*(matrix<R, C, T>&& value1, const T1 value2) noexcept { return value1 *= value2; }
    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1>, vector_size_type R, vector_size_type C, typename T, typename T1)
    [[nodiscard]] constexpr matrix<R, C, T> operator*(const T value1, const matrix<R, C, T1>& value2) noexcept { return matrix<R, C, T>(value2) *= value1; }



    template<typename T, typename T1> [[nodiscard]] constexpr matrix<2, 2, T> operator*(const matrix<2, 2, T>& value1, const matrix<2, 2, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<2, 3, T> operator*(const matrix<2, 2, T>& value1, const matrix<2, 3, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<2, 4, T> operator*(const matrix<2, 2, T>& value1, const matrix<2, 4, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<2, 2, T> operator*(const matrix<2, 3, T>& value1, const matrix<3, 2, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<2, 3, T> operator*(const matrix<2, 3, T>& value1, const matrix<3, 3, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<2, 4, T> operator*(const matrix<2, 3, T>& value1, const matrix<3, 4, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<2, 2, T> operator*(const matrix<2, 4, T>& value1, const matrix<4, 2, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<2, 3, T> operator*(const matrix<2, 4, T>& value1, const matrix<4, 3, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<2, 4, T> operator*(const matrix<2, 4, T>& value1, const matrix<4, 4, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<3, 2, T> operator*(const matrix<3, 2, T>& value1, const matrix<2, 2, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<3, 3, T> operator*(const matrix<3, 2, T>& value1, const matrix<2, 3, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<3, 4, T> operator*(const matrix<3, 2, T>& value1, const matrix<2, 4, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<3, 2, T> operator*(const matrix<3, 3, T>& value1, const matrix<3, 2, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<3, 3, T> operator*(const matrix<3, 3, T>& value1, const matrix<3, 3, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<3, 4, T> operator*(const matrix<3, 3, T>& value1, const matrix<3, 4, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<3, 2, T> operator*(const matrix<3, 4, T>& value1, const matrix<4, 2, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<3, 3, T> operator*(const matrix<3, 4, T>& value1, const matrix<4, 3, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<3, 4, T> operator*(const matrix<3, 4, T>& value1, const matrix<4, 4, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<4, 2, T> operator*(const matrix<4, 2, T>& value1, const matrix<2, 2, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<4, 3, T> operator*(const matrix<4, 2, T>& value1, const matrix<2, 3, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<4, 4, T> operator*(const matrix<4, 2, T>& value1, const matrix<2, 4, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<4, 2, T> operator*(const matrix<4, 3, T>& value1, const matrix<3, 2, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<4, 3, T> operator*(const matrix<4, 3, T>& value1, const matrix<3, 3, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<4, 4, T> operator*(const matrix<4, 3, T>& value1, const matrix<3, 4, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<4, 2, T> operator*(const matrix<4, 4, T>& value1, const matrix<4, 2, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<4, 3, T> operator*(const matrix<4, 4, T>& value1, const matrix<4, 3, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr matrix<4, 4, T> operator*(const matrix<4, 4, T>& value1, const matrix<4, 4, T1>& value2) noexcept;

    template<vector_size_type C, typename T, typename T1> [[nodiscard]] constexpr vector<2, T> operator*(const matrix<2, C, T>& value1, const vector<C, T1>& value2) noexcept;
    template<vector_size_type C, typename T, typename T1> [[nodiscard]] constexpr vector<3, T> operator*(const matrix<3, C, T>& value1, const vector<C, T1>& value2) noexcept;
    template<vector_size_type C, typename T, typename T1> [[nodiscard]] constexpr vector<4, T> operator*(const matrix<4, C, T>& value1, const vector<C, T1>& value2) noexcept;

    template<typename T, typename T1> [[nodiscard]] constexpr vector<2, T> operator*(const vector<2, T>& value1, const matrix<2, 2, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr vector<2, T> operator*(const vector<3, T>& value1, const matrix<3, 2, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr vector<2, T> operator*(const vector<4, T>& value1, const matrix<4, 2, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr vector<3, T> operator*(const vector<2, T>& value1, const matrix<2, 3, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr vector<3, T> operator*(const vector<3, T>& value1, const matrix<3, 3, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr vector<3, T> operator*(const vector<4, T>& value1, const matrix<4, 3, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr vector<4, T> operator*(const vector<2, T>& value1, const matrix<2, 4, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr vector<4, T> operator*(const vector<3, T>& value1, const matrix<3, 4, T1>& value2) noexcept;
    template<typename T, typename T1> [[nodiscard]] constexpr vector<4, T> operator*(const vector<4, T>& value1, const matrix<4, 4, T1>& value2) noexcept;

    template<typename T, typename T1>
    constexpr matrix<2, 2, T> operator*(const matrix<2, 2, T>& value1, const matrix<2, 2, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<2, 3, T> operator*(const matrix<2, 2, T>& value1, const matrix<2, 3, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<2, 4, T> operator*(const matrix<2, 2, T>& value1, const matrix<2, 4, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z),
            value1.rows[0].x * static_cast<T>(value2.rows[0].w) + value1.rows[0].y * static_cast<T>(value2.rows[1].w),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z),
            value1.rows[1].x * static_cast<T>(value2.rows[0].w) + value1.rows[1].y * static_cast<T>(value2.rows[1].w)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<2, 2, T> operator*(const matrix<2, 3, T>& value1, const matrix<3, 2, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<2, 3, T> operator*(const matrix<2, 3, T>& value1, const matrix<3, 3, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z) + value1.rows[0].z * static_cast<T>(value2.rows[2].z),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z) + value1.rows[1].z * static_cast<T>(value2.rows[2].z)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<2, 4, T> operator*(const matrix<2, 3, T>& value1, const matrix<3, 4, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z) + value1.rows[0].z * static_cast<T>(value2.rows[2].z),
            value1.rows[0].x * static_cast<T>(value2.rows[0].w) + value1.rows[0].y * static_cast<T>(value2.rows[1].w) + value1.rows[0].z * static_cast<T>(value2.rows[2].w),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z) + value1.rows[1].z * static_cast<T>(value2.rows[2].z),
            value1.rows[1].x * static_cast<T>(value2.rows[0].w) + value1.rows[1].y * static_cast<T>(value2.rows[1].w) + value1.rows[1].z * static_cast<T>(value2.rows[2].w)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<2, 2, T> operator*(const matrix<2, 4, T>& value1, const matrix<4, 2, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x) + value1.rows[0].w * static_cast<T>(value2.rows[3].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y) + value1.rows[0].w * static_cast<T>(value2.rows[3].y),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x) + value1.rows[1].w * static_cast<T>(value2.rows[3].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y) + value1.rows[1].w * static_cast<T>(value2.rows[3].y)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<2, 3, T> operator*(const matrix<2, 4, T>& value1, const matrix<4, 3, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x) + value1.rows[0].w * static_cast<T>(value2.rows[3].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y) + value1.rows[0].w * static_cast<T>(value2.rows[3].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z) + value1.rows[0].z * static_cast<T>(value2.rows[2].z) + value1.rows[0].w * static_cast<T>(value2.rows[3].z),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x) + value1.rows[1].w * static_cast<T>(value2.rows[3].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y) + value1.rows[1].w * static_cast<T>(value2.rows[3].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z) + value1.rows[1].z * static_cast<T>(value2.rows[2].z) + value1.rows[1].w * static_cast<T>(value2.rows[3].z)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<2, 4, T> operator*(const matrix<2, 4, T>& value1, const matrix<4, 4, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x) + value1.rows[0].w * static_cast<T>(value2.rows[3].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y) + value1.rows[0].w * static_cast<T>(value2.rows[3].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z) + value1.rows[0].z * static_cast<T>(value2.rows[2].z) + value1.rows[0].w * static_cast<T>(value2.rows[3].z),
            value1.rows[0].x * static_cast<T>(value2.rows[0].w) + value1.rows[0].y * static_cast<T>(value2.rows[1].w) + value1.rows[0].z * static_cast<T>(value2.rows[2].w) + value1.rows[0].w * static_cast<T>(value2.rows[3].w),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x) + value1.rows[1].w * static_cast<T>(value2.rows[3].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y) + value1.rows[1].w * static_cast<T>(value2.rows[3].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z) + value1.rows[1].z * static_cast<T>(value2.rows[2].z) + value1.rows[1].w * static_cast<T>(value2.rows[3].z),
            value1.rows[1].x * static_cast<T>(value2.rows[0].w) + value1.rows[1].y * static_cast<T>(value2.rows[1].w) + value1.rows[1].z * static_cast<T>(value2.rows[2].w) + value1.rows[1].w * static_cast<T>(value2.rows[3].w)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<3, 2, T> operator*(const matrix<3, 2, T>& value1, const matrix<2, 2, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<3, 3, T> operator*(const matrix<3, 2, T>& value1, const matrix<2, 3, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y),
            value1.rows[2].x * static_cast<T>(value2.rows[0].z) + value1.rows[2].y * static_cast<T>(value2.rows[1].z)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<3, 4, T> operator*(const matrix<3, 2, T>& value1, const matrix<2, 4, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z),
            value1.rows[0].x * static_cast<T>(value2.rows[0].w) + value1.rows[0].y * static_cast<T>(value2.rows[1].w),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z),
            value1.rows[1].x * static_cast<T>(value2.rows[0].w) + value1.rows[1].y * static_cast<T>(value2.rows[1].w),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y),
            value1.rows[2].x * static_cast<T>(value2.rows[0].z) + value1.rows[2].y * static_cast<T>(value2.rows[1].z),
            value1.rows[2].x * static_cast<T>(value2.rows[0].w) + value1.rows[2].y * static_cast<T>(value2.rows[1].w)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<3, 2, T> operator*(const matrix<3, 3, T>& value1, const matrix<3, 2, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x) + value1.rows[2].z * static_cast<T>(value2.rows[2].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y) + value1.rows[2].z * static_cast<T>(value2.rows[2].y)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<3, 3, T> operator*(const matrix<3, 3, T>& value1, const matrix<3, 3, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z) + value1.rows[0].z * static_cast<T>(value2.rows[2].z),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z) + value1.rows[1].z * static_cast<T>(value2.rows[2].z),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x) + value1.rows[2].z * static_cast<T>(value2.rows[2].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y) + value1.rows[2].z * static_cast<T>(value2.rows[2].y),
            value1.rows[2].x * static_cast<T>(value2.rows[0].z) + value1.rows[2].y * static_cast<T>(value2.rows[1].z) + value1.rows[2].z * static_cast<T>(value2.rows[2].z)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<3, 4, T> operator*(const matrix<3, 3, T>& value1, const matrix<3, 4, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z) + value1.rows[0].z * static_cast<T>(value2.rows[2].z),
            value1.rows[0].x * static_cast<T>(value2.rows[0].w) + value1.rows[0].y * static_cast<T>(value2.rows[1].w) + value1.rows[0].z * static_cast<T>(value2.rows[2].w),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z) + value1.rows[1].z * static_cast<T>(value2.rows[2].z),
            value1.rows[1].x * static_cast<T>(value2.rows[0].w) + value1.rows[1].y * static_cast<T>(value2.rows[1].w) + value1.rows[1].z * static_cast<T>(value2.rows[2].w),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x) + value1.rows[2].z * static_cast<T>(value2.rows[2].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y) + value1.rows[2].z * static_cast<T>(value2.rows[2].y),
            value1.rows[2].x * static_cast<T>(value2.rows[0].z) + value1.rows[2].y * static_cast<T>(value2.rows[1].z) + value1.rows[2].z * static_cast<T>(value2.rows[2].z),
            value1.rows[2].x * static_cast<T>(value2.rows[0].w) + value1.rows[2].y * static_cast<T>(value2.rows[1].w) + value1.rows[2].z * static_cast<T>(value2.rows[2].w)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<3, 2, T> operator*(const matrix<3, 4, T>& value1, const matrix<4, 2, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x) + value1.rows[0].w * static_cast<T>(value2.rows[3].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y) + value1.rows[0].w * static_cast<T>(value2.rows[3].y),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x) + value1.rows[1].w * static_cast<T>(value2.rows[3].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y) + value1.rows[1].w * static_cast<T>(value2.rows[3].y),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x) + value1.rows[2].z * static_cast<T>(value2.rows[2].x) + value1.rows[2].w * static_cast<T>(value2.rows[3].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y) + value1.rows[2].z * static_cast<T>(value2.rows[2].y) + value1.rows[2].w * static_cast<T>(value2.rows[3].y)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<3, 3, T> operator*(const matrix<3, 4, T>& value1, const matrix<4, 3, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x) + value1.rows[0].w * static_cast<T>(value2.rows[3].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y) + value1.rows[0].w * static_cast<T>(value2.rows[3].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z) + value1.rows[0].z * static_cast<T>(value2.rows[2].z) + value1.rows[0].w * static_cast<T>(value2.rows[3].z),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x) + value1.rows[1].w * static_cast<T>(value2.rows[3].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y) + value1.rows[1].w * static_cast<T>(value2.rows[3].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z) + value1.rows[1].z * static_cast<T>(value2.rows[2].z) + value1.rows[1].w * static_cast<T>(value2.rows[3].z),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x) + value1.rows[2].z * static_cast<T>(value2.rows[2].x) + value1.rows[2].w * static_cast<T>(value2.rows[3].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y) + value1.rows[2].z * static_cast<T>(value2.rows[2].y) + value1.rows[2].w * static_cast<T>(value2.rows[3].y),
            value1.rows[2].x * static_cast<T>(value2.rows[0].z) + value1.rows[2].y * static_cast<T>(value2.rows[1].z) + value1.rows[2].z * static_cast<T>(value2.rows[2].z) + value1.rows[2].w * static_cast<T>(value2.rows[3].z)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<3, 4, T> operator*(const matrix<3, 4, T>& value1, const matrix<4, 4, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x) + value1.rows[0].w * static_cast<T>(value2.rows[3].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y) + value1.rows[0].w * static_cast<T>(value2.rows[3].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z) + value1.rows[0].z * static_cast<T>(value2.rows[2].z) + value1.rows[0].w * static_cast<T>(value2.rows[3].z),
            value1.rows[0].x * static_cast<T>(value2.rows[0].w) + value1.rows[0].y * static_cast<T>(value2.rows[1].w) + value1.rows[0].z * static_cast<T>(value2.rows[2].w) + value1.rows[0].w * static_cast<T>(value2.rows[3].w),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x) + value1.rows[1].w * static_cast<T>(value2.rows[3].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y) + value1.rows[1].w * static_cast<T>(value2.rows[3].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z) + value1.rows[1].z * static_cast<T>(value2.rows[2].z) + value1.rows[1].w * static_cast<T>(value2.rows[3].z),
            value1.rows[1].x * static_cast<T>(value2.rows[0].w) + value1.rows[1].y * static_cast<T>(value2.rows[1].w) + value1.rows[1].z * static_cast<T>(value2.rows[2].w) + value1.rows[1].w * static_cast<T>(value2.rows[3].w),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x) + value1.rows[2].z * static_cast<T>(value2.rows[2].x) + value1.rows[2].w * static_cast<T>(value2.rows[3].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y) + value1.rows[2].z * static_cast<T>(value2.rows[2].y) + value1.rows[2].w * static_cast<T>(value2.rows[3].y),
            value1.rows[2].x * static_cast<T>(value2.rows[0].z) + value1.rows[2].y * static_cast<T>(value2.rows[1].z) + value1.rows[2].z * static_cast<T>(value2.rows[2].z) + value1.rows[2].w * static_cast<T>(value2.rows[3].z),
            value1.rows[2].x * static_cast<T>(value2.rows[0].w) + value1.rows[2].y * static_cast<T>(value2.rows[1].w) + value1.rows[2].z * static_cast<T>(value2.rows[2].w) + value1.rows[2].w * static_cast<T>(value2.rows[3].w)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<4, 2, T> operator*(const matrix<4, 2, T>& value1, const matrix<2, 2, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y),

            value1.rows[3].x * static_cast<T>(value2.rows[0].x) + value1.rows[3].y * static_cast<T>(value2.rows[1].x),
            value1.rows[3].x * static_cast<T>(value2.rows[0].y) + value1.rows[3].y * static_cast<T>(value2.rows[1].y)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<4, 3, T> operator*(const matrix<4, 2, T>& value1, const matrix<2, 3, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y),
            value1.rows[2].x * static_cast<T>(value2.rows[0].z) + value1.rows[2].y * static_cast<T>(value2.rows[1].z),

            value1.rows[3].x * static_cast<T>(value2.rows[0].x) + value1.rows[3].y * static_cast<T>(value2.rows[1].x),
            value1.rows[3].x * static_cast<T>(value2.rows[0].y) + value1.rows[3].y * static_cast<T>(value2.rows[1].y),
            value1.rows[3].x * static_cast<T>(value2.rows[0].z) + value1.rows[3].y * static_cast<T>(value2.rows[1].z)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<4, 4, T> operator*(const matrix<4, 2, T>& value1, const matrix<2, 4, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z),
            value1.rows[0].x * static_cast<T>(value2.rows[0].w) + value1.rows[0].y * static_cast<T>(value2.rows[1].w),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z),
            value1.rows[1].x * static_cast<T>(value2.rows[0].w) + value1.rows[1].y * static_cast<T>(value2.rows[1].w),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y),
            value1.rows[2].x * static_cast<T>(value2.rows[0].z) + value1.rows[2].y * static_cast<T>(value2.rows[1].z),
            value1.rows[2].x * static_cast<T>(value2.rows[0].w) + value1.rows[2].y * static_cast<T>(value2.rows[1].w),

            value1.rows[3].x * static_cast<T>(value2.rows[0].x) + value1.rows[3].y * static_cast<T>(value2.rows[1].x),
            value1.rows[3].x * static_cast<T>(value2.rows[0].y) + value1.rows[3].y * static_cast<T>(value2.rows[1].y),
            value1.rows[3].x * static_cast<T>(value2.rows[0].z) + value1.rows[3].y * static_cast<T>(value2.rows[1].z),
            value1.rows[3].x * static_cast<T>(value2.rows[0].w) + value1.rows[3].y * static_cast<T>(value2.rows[1].w)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<4, 2, T> operator*(const matrix<4, 3, T>& value1, const matrix<3, 2, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x) + value1.rows[2].z * static_cast<T>(value2.rows[2].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y) + value1.rows[2].z * static_cast<T>(value2.rows[2].y),

            value1.rows[3].x * static_cast<T>(value2.rows[0].x) + value1.rows[3].y * static_cast<T>(value2.rows[1].x) + value1.rows[3].z * static_cast<T>(value2.rows[2].x),
            value1.rows[3].x * static_cast<T>(value2.rows[0].y) + value1.rows[3].y * static_cast<T>(value2.rows[1].y) + value1.rows[3].z * static_cast<T>(value2.rows[2].y)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<4, 3, T> operator*(const matrix<4, 3, T>& value1, const matrix<3, 3, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z) + value1.rows[0].z * static_cast<T>(value2.rows[2].z),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z) + value1.rows[1].z * static_cast<T>(value2.rows[2].z),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x) + value1.rows[2].z * static_cast<T>(value2.rows[2].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y) + value1.rows[2].z * static_cast<T>(value2.rows[2].y),
            value1.rows[2].x * static_cast<T>(value2.rows[0].z) + value1.rows[2].y * static_cast<T>(value2.rows[1].z) + value1.rows[2].z * static_cast<T>(value2.rows[2].z),

            value1.rows[3].x * static_cast<T>(value2.rows[0].x) + value1.rows[3].y * static_cast<T>(value2.rows[1].x) + value1.rows[3].z * static_cast<T>(value2.rows[2].x),
            value1.rows[3].x * static_cast<T>(value2.rows[0].y) + value1.rows[3].y * static_cast<T>(value2.rows[1].y) + value1.rows[3].z * static_cast<T>(value2.rows[2].y),
            value1.rows[3].x * static_cast<T>(value2.rows[0].z) + value1.rows[3].y * static_cast<T>(value2.rows[1].z) + value1.rows[3].z * static_cast<T>(value2.rows[2].z)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<4, 4, T> operator*(const matrix<4, 3, T>& value1, const matrix<3, 4, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z) + value1.rows[0].z * static_cast<T>(value2.rows[2].z),
            value1.rows[0].x * static_cast<T>(value2.rows[0].w) + value1.rows[0].y * static_cast<T>(value2.rows[1].w) + value1.rows[0].z * static_cast<T>(value2.rows[2].w),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z) + value1.rows[1].z * static_cast<T>(value2.rows[2].z),
            value1.rows[1].x * static_cast<T>(value2.rows[0].w) + value1.rows[1].y * static_cast<T>(value2.rows[1].w) + value1.rows[1].z * static_cast<T>(value2.rows[2].w),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x) + value1.rows[2].z * static_cast<T>(value2.rows[2].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y) + value1.rows[2].z * static_cast<T>(value2.rows[2].y),
            value1.rows[2].x * static_cast<T>(value2.rows[0].z) + value1.rows[2].y * static_cast<T>(value2.rows[1].z) + value1.rows[2].z * static_cast<T>(value2.rows[2].z),
            value1.rows[2].x * static_cast<T>(value2.rows[0].w) + value1.rows[2].y * static_cast<T>(value2.rows[1].w) + value1.rows[2].z * static_cast<T>(value2.rows[2].w),

            value1.rows[3].x * static_cast<T>(value2.rows[0].x) + value1.rows[3].y * static_cast<T>(value2.rows[1].x) + value1.rows[3].z * static_cast<T>(value2.rows[2].x),
            value1.rows[3].x * static_cast<T>(value2.rows[0].y) + value1.rows[3].y * static_cast<T>(value2.rows[1].y) + value1.rows[3].z * static_cast<T>(value2.rows[2].y),
            value1.rows[3].x * static_cast<T>(value2.rows[0].z) + value1.rows[3].y * static_cast<T>(value2.rows[1].z) + value1.rows[3].z * static_cast<T>(value2.rows[2].z),
            value1.rows[3].x * static_cast<T>(value2.rows[0].w) + value1.rows[3].y * static_cast<T>(value2.rows[1].w) + value1.rows[3].z * static_cast<T>(value2.rows[2].w)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<4, 2, T> operator*(const matrix<4, 4, T>& value1, const matrix<4, 2, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x) + value1.rows[0].w * static_cast<T>(value2.rows[3].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y) + value1.rows[0].w * static_cast<T>(value2.rows[3].y),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x) + value1.rows[1].w * static_cast<T>(value2.rows[3].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y) + value1.rows[1].w * static_cast<T>(value2.rows[3].y),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x) + value1.rows[2].z * static_cast<T>(value2.rows[2].x) + value1.rows[2].w * static_cast<T>(value2.rows[3].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y) + value1.rows[2].z * static_cast<T>(value2.rows[2].y) + value1.rows[2].w * static_cast<T>(value2.rows[3].y),

            value1.rows[3].x * static_cast<T>(value2.rows[0].x) + value1.rows[3].y * static_cast<T>(value2.rows[1].x) + value1.rows[3].z * static_cast<T>(value2.rows[2].x) + value1.rows[3].w * static_cast<T>(value2.rows[3].x),
            value1.rows[3].x * static_cast<T>(value2.rows[0].y) + value1.rows[3].y * static_cast<T>(value2.rows[1].y) + value1.rows[3].z * static_cast<T>(value2.rows[2].y) + value1.rows[3].w * static_cast<T>(value2.rows[3].y)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<4, 3, T> operator*(const matrix<4, 4, T>& value1, const matrix<4, 3, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x) + value1.rows[0].w * static_cast<T>(value2.rows[3].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y) + value1.rows[0].w * static_cast<T>(value2.rows[3].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z) + value1.rows[0].z * static_cast<T>(value2.rows[2].z) + value1.rows[0].w * static_cast<T>(value2.rows[3].z),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x) + value1.rows[1].w * static_cast<T>(value2.rows[3].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y) + value1.rows[1].w * static_cast<T>(value2.rows[3].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z) + value1.rows[1].z * static_cast<T>(value2.rows[2].z) + value1.rows[1].w * static_cast<T>(value2.rows[3].z),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x) + value1.rows[2].z * static_cast<T>(value2.rows[2].x) + value1.rows[2].w * static_cast<T>(value2.rows[3].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y) + value1.rows[2].z * static_cast<T>(value2.rows[2].y) + value1.rows[2].w * static_cast<T>(value2.rows[3].y),
            value1.rows[2].x * static_cast<T>(value2.rows[0].z) + value1.rows[2].y * static_cast<T>(value2.rows[1].z) + value1.rows[2].z * static_cast<T>(value2.rows[2].z) + value1.rows[2].w * static_cast<T>(value2.rows[3].z),

            value1.rows[3].x * static_cast<T>(value2.rows[0].x) + value1.rows[3].y * static_cast<T>(value2.rows[1].x) + value1.rows[3].z * static_cast<T>(value2.rows[2].x) + value1.rows[3].w * static_cast<T>(value2.rows[3].x),
            value1.rows[3].x * static_cast<T>(value2.rows[0].y) + value1.rows[3].y * static_cast<T>(value2.rows[1].y) + value1.rows[3].z * static_cast<T>(value2.rows[2].y) + value1.rows[3].w * static_cast<T>(value2.rows[3].y),
            value1.rows[3].x * static_cast<T>(value2.rows[0].z) + value1.rows[3].y * static_cast<T>(value2.rows[1].z) + value1.rows[3].z * static_cast<T>(value2.rows[2].z) + value1.rows[3].w * static_cast<T>(value2.rows[3].z)
        };
    }
    template<typename T, typename T1>
    constexpr matrix<4, 4, T> operator*(const matrix<4, 4, T>& value1, const matrix<4, 4, T1>& value2) noexcept
    {
        return {
            value1.rows[0].x * static_cast<T>(value2.rows[0].x) + value1.rows[0].y * static_cast<T>(value2.rows[1].x) + value1.rows[0].z * static_cast<T>(value2.rows[2].x) + value1.rows[0].w * static_cast<T>(value2.rows[3].x),
            value1.rows[0].x * static_cast<T>(value2.rows[0].y) + value1.rows[0].y * static_cast<T>(value2.rows[1].y) + value1.rows[0].z * static_cast<T>(value2.rows[2].y) + value1.rows[0].w * static_cast<T>(value2.rows[3].y),
            value1.rows[0].x * static_cast<T>(value2.rows[0].z) + value1.rows[0].y * static_cast<T>(value2.rows[1].z) + value1.rows[0].z * static_cast<T>(value2.rows[2].z) + value1.rows[0].w * static_cast<T>(value2.rows[3].z),
            value1.rows[0].x * static_cast<T>(value2.rows[0].w) + value1.rows[0].y * static_cast<T>(value2.rows[1].w) + value1.rows[0].z * static_cast<T>(value2.rows[2].w) + value1.rows[0].w * static_cast<T>(value2.rows[3].w),

            value1.rows[1].x * static_cast<T>(value2.rows[0].x) + value1.rows[1].y * static_cast<T>(value2.rows[1].x) + value1.rows[1].z * static_cast<T>(value2.rows[2].x) + value1.rows[1].w * static_cast<T>(value2.rows[3].x),
            value1.rows[1].x * static_cast<T>(value2.rows[0].y) + value1.rows[1].y * static_cast<T>(value2.rows[1].y) + value1.rows[1].z * static_cast<T>(value2.rows[2].y) + value1.rows[1].w * static_cast<T>(value2.rows[3].y),
            value1.rows[1].x * static_cast<T>(value2.rows[0].z) + value1.rows[1].y * static_cast<T>(value2.rows[1].z) + value1.rows[1].z * static_cast<T>(value2.rows[2].z) + value1.rows[1].w * static_cast<T>(value2.rows[3].z),
            value1.rows[1].x * static_cast<T>(value2.rows[0].w) + value1.rows[1].y * static_cast<T>(value2.rows[1].w) + value1.rows[1].z * static_cast<T>(value2.rows[2].w) + value1.rows[1].w * static_cast<T>(value2.rows[3].w),

            value1.rows[2].x * static_cast<T>(value2.rows[0].x) + value1.rows[2].y * static_cast<T>(value2.rows[1].x) + value1.rows[2].z * static_cast<T>(value2.rows[2].x) + value1.rows[2].w * static_cast<T>(value2.rows[3].x),
            value1.rows[2].x * static_cast<T>(value2.rows[0].y) + value1.rows[2].y * static_cast<T>(value2.rows[1].y) + value1.rows[2].z * static_cast<T>(value2.rows[2].y) + value1.rows[2].w * static_cast<T>(value2.rows[3].y),
            value1.rows[2].x * static_cast<T>(value2.rows[0].z) + value1.rows[2].y * static_cast<T>(value2.rows[1].z) + value1.rows[2].z * static_cast<T>(value2.rows[2].z) + value1.rows[2].w * static_cast<T>(value2.rows[3].z),
            value1.rows[2].x * static_cast<T>(value2.rows[0].w) + value1.rows[2].y * static_cast<T>(value2.rows[1].w) + value1.rows[2].z * static_cast<T>(value2.rows[2].w) + value1.rows[2].w * static_cast<T>(value2.rows[3].w),

            value1.rows[3].x * static_cast<T>(value2.rows[0].x) + value1.rows[3].y * static_cast<T>(value2.rows[1].x) + value1.rows[3].z * static_cast<T>(value2.rows[2].x) + value1.rows[3].w * static_cast<T>(value2.rows[3].x),
            value1.rows[3].x * static_cast<T>(value2.rows[0].y) + value1.rows[3].y * static_cast<T>(value2.rows[1].y) + value1.rows[3].z * static_cast<T>(value2.rows[2].y) + value1.rows[3].w * static_cast<T>(value2.rows[3].y),
            value1.rows[3].x * static_cast<T>(value2.rows[0].z) + value1.rows[3].y * static_cast<T>(value2.rows[1].z) + value1.rows[3].z * static_cast<T>(value2.rows[2].z) + value1.rows[3].w * static_cast<T>(value2.rows[3].z),
            value1.rows[3].x * static_cast<T>(value2.rows[0].w) + value1.rows[3].y * static_cast<T>(value2.rows[1].w) + value1.rows[3].z * static_cast<T>(value2.rows[2].w) + value1.rows[3].w * static_cast<T>(value2.rows[3].w)
        };
    }

    template<vector_size_type C, typename T, typename T1>
    constexpr vector<2, T> operator*(const matrix<2, C, T>& value1, const vector<C, T1>& value2) noexcept
    {
        return { value1.rows[0].dot(value2), value1.rows[1].dot(value2) };
    }
    template<vector_size_type C, typename T, typename T1>
    constexpr vector<3, T> operator*(const matrix<3, C, T>& value1, const vector<C, T1>& value2) noexcept
    {
        return { value1.rows[0].dot(value2), value1.rows[1].dot(value2), value1.rows[2].dot(value2) };
    }
    template<vector_size_type C, typename T, typename T1>
    constexpr vector<4, T> operator*(const matrix<4, C, T>& value1, const vector<C, T1>& value2) noexcept
    {
        return { value1.rows[0].dot(value2), value1.rows[1].dot(value2), value1.rows[2].dot(value2), value1.rows[3].dot(value2) };
    }

    template<typename T, typename T1>
    constexpr vector<2, T> operator*(const vector<2, T>& value1, const matrix<2, 2, T1>& value2) noexcept
    {
        return {
            value1.x * static_cast<T>(value2.rows[0].x) + value1.y * static_cast<T>(value2.rows[1].x),
            value1.x * static_cast<T>(value2.rows[0].y) + value1.y * static_cast<T>(value2.rows[1].y)
        };
    }
    template<typename T, typename T1>
    constexpr vector<2, T> operator*(const vector<3, T>& value1, const matrix<3, 2, T1>& value2) noexcept
    {
        return {
            value1.x * static_cast<T>(value2.rows[0].x) + value1.y * static_cast<T>(value2.rows[1].x) + value1.z * static_cast<T>(value2.rows[2].x),
            value1.x * static_cast<T>(value2.rows[0].y) + value1.y * static_cast<T>(value2.rows[1].y) + value1.z * static_cast<T>(value2.rows[2].y)
        };
    }
    template<typename T, typename T1>
    constexpr vector<2, T> operator*(const vector<4, T>& value1, const matrix<4, 2, T1>& value2) noexcept
    {
        return {
            value1.x * static_cast<T>(value2.rows[0].x) + value1.y * static_cast<T>(value2.rows[1].x) + value1.z * static_cast<T>(value2.rows[2].x) + value1.w * static_cast<T>(value2.rows[3].x),
            value1.x * static_cast<T>(value2.rows[0].y) + value1.y * static_cast<T>(value2.rows[1].y) + value1.z * static_cast<T>(value2.rows[2].y) + value1.w * static_cast<T>(value2.rows[3].y)
        };
    }
    template<typename T, typename T1>
    constexpr vector<3, T> operator*(const vector<2, T>& value1, const matrix<2, 3, T1>& value2) noexcept
    {
        return {
            value1.x * static_cast<T>(value2.rows[0].x) + value1.y * static_cast<T>(value2.rows[1].x),
            value1.x * static_cast<T>(value2.rows[0].y) + value1.y * static_cast<T>(value2.rows[1].y),
            value1.x * static_cast<T>(value2.rows[0].z) + value1.y * static_cast<T>(value2.rows[1].z)
        };
    }
    template<typename T, typename T1>
    constexpr vector<3, T> operator*(const vector<3, T>& value1, const matrix<3, 3, T1>& value2) noexcept
    {
        return {
            value1.x * static_cast<T>(value2.rows[0].x) + value1.y * static_cast<T>(value2.rows[1].x) + value1.z * static_cast<T>(value2.rows[2].x),
            value1.x * static_cast<T>(value2.rows[0].y) + value1.y * static_cast<T>(value2.rows[1].y) + value1.z * static_cast<T>(value2.rows[2].y),
            value1.x * static_cast<T>(value2.rows[0].z) + value1.y * static_cast<T>(value2.rows[1].z) + value1.z * static_cast<T>(value2.rows[2].z)
        };
    }
    template<typename T, typename T1>
    constexpr vector<3, T> operator*(const vector<4, T>& value1, const matrix<4, 3, T1>& value2) noexcept
    {
        return {
            value1.x * static_cast<T>(value2.rows[0].x) + value1.y * static_cast<T>(value2.rows[1].x) + value1.z * static_cast<T>(value2.rows[2].x) + value1.w * static_cast<T>(value2.rows[3].x),
            value1.x * static_cast<T>(value2.rows[0].y) + value1.y * static_cast<T>(value2.rows[1].y) + value1.z * static_cast<T>(value2.rows[2].y) + value1.w * static_cast<T>(value2.rows[3].y),
            value1.x * static_cast<T>(value2.rows[0].z) + value1.y * static_cast<T>(value2.rows[1].z) + value1.z * static_cast<T>(value2.rows[2].z) + value1.w * static_cast<T>(value2.rows[3].z)
        };
    }
    template<typename T, typename T1>
    constexpr vector<4, T> operator*(const vector<2, T>& value1, const matrix<2, 4, T1>& value2) noexcept
    {
        return {
            value1.x * static_cast<T>(value2.rows[0].x) + value1.y * static_cast<T>(value2.rows[1].x),
            value1.x * static_cast<T>(value2.rows[0].y) + value1.y * static_cast<T>(value2.rows[1].y),
            value1.x * static_cast<T>(value2.rows[0].z) + value1.y * static_cast<T>(value2.rows[1].z),
            value1.x * static_cast<T>(value2.rows[0].w) + value1.y * static_cast<T>(value2.rows[1].w)
        };
    }
    template<typename T, typename T1>
    constexpr vector<4, T> operator*(const vector<3, T>& value1, const matrix<3, 4, T1>& value2) noexcept
    {
        return {
            value1.x * static_cast<T>(value2.rows[0].x) + value1.y * static_cast<T>(value2.rows[1].x) + value1.z * static_cast<T>(value2.rows[2].x),
            value1.x * static_cast<T>(value2.rows[0].y) + value1.y * static_cast<T>(value2.rows[1].y) + value1.z * static_cast<T>(value2.rows[2].y),
            value1.x * static_cast<T>(value2.rows[0].z) + value1.y * static_cast<T>(value2.rows[1].z) + value1.z * static_cast<T>(value2.rows[2].z),
            value1.x * static_cast<T>(value2.rows[0].w) + value1.y * static_cast<T>(value2.rows[1].w) + value1.z * static_cast<T>(value2.rows[2].w)
        };
    }
    template<typename T, typename T1>
    constexpr vector<4, T> operator*(const vector<4, T>& value1, const matrix<4, 4, T1>& value2) noexcept
    {
        return {
            value1.x * static_cast<T>(value2.rows[0].x) + value1.y * static_cast<T>(value2.rows[1].x) + value1.z * static_cast<T>(value2.rows[2].x) + value1.w * static_cast<T>(value2.rows[3].x),
            value1.x * static_cast<T>(value2.rows[0].y) + value1.y * static_cast<T>(value2.rows[1].y) + value1.z * static_cast<T>(value2.rows[2].y) + value1.w * static_cast<T>(value2.rows[3].y),
            value1.x * static_cast<T>(value2.rows[0].z) + value1.y * static_cast<T>(value2.rows[1].z) + value1.z * static_cast<T>(value2.rows[2].z) + value1.w * static_cast<T>(value2.rows[3].z),
            value1.x * static_cast<T>(value2.rows[0].w) + value1.y * static_cast<T>(value2.rows[1].w) + value1.z * static_cast<T>(value2.rows[2].w) + value1.w * static_cast<T>(value2.rows[3].w)
        };
    }
}
