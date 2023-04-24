// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vector_base.h"

namespace jutils
{
    namespace math
    {
        template<vector_size_type Size, typename T> requires jutils::math::is_valid_vector_v<Size, T>
        constexpr vector<Size, T> abs(const vector<Size, T>& value) noexcept { return value.abs(); }

        template<vector_size_type Size, typename T, typename R = std::conditional_t<std::is_floating_point_v<T>, T, float>>
            requires is_valid_vector_v<Size, T> && std::floating_point<R>
        R vectorLength(const vector<Size, T>& value) noexcept { return static_cast<R>(jutils::math::sqrt(static_cast<R>(value.lengthSqr()))); }
        template<vector_size_type Size, typename T, typename R = std::conditional_t<std::is_floating_point_v<T>, T, float>>
            requires is_valid_vector_v<Size, T> && std::floating_point<R>
        vector<Size, R> vectorNormalize(const vector<Size, T>& value, const R eps = jutils::math::EpsDefault<R>) noexcept
        {
            const R lengthSqr = static_cast<R>(value.lengthSqr());
            if (jutils::math::isNearlyZero(lengthSqr, eps))
            {
                return vector<Size, R>(0);
            }
            return value.template copy<R>() / static_cast<R>(jutils::math::sqrt(lengthSqr));
        }

        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<S, T1> operator+(const vector<S, T1>& value1, const T2 value2) noexcept { return value1.copy() += value2; }
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<S, T1> operator+(vector<S, T1>&& value1, const T2 value2) noexcept { return value1 += value2; }
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T1>
        constexpr vector<S, T1> operator+(const T1 value1, const vector<S, T2>& value2) noexcept { return value2.template copy<T1>() += value1; }
        template<vector_size_type S, typename T>
        constexpr vector<S, T> operator+(const T value1, vector<S, T>&& value2) noexcept { return value2 += value1; }

        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<S, T1> operator-(const vector<S, T1>& value1, const T2 value2) noexcept { return value1.copy() -= value2; }
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<S, T1> operator-(vector<S, T1>&& value1, const T2 value2) noexcept { return value1 -= value2; }
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T1>
        constexpr vector<S, T1> operator-(const T1 value1, const vector<S, T2>& value2) noexcept { return vector<S, T1>(value1) -= value2; }
        
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<S, T1> operator*(const vector<S, T1>& value1, const T2 value2) noexcept { return value1.copy() *= value2; }
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<S, T1> operator*(vector<S, T1>&& value1, const T2 value2) noexcept { return value1 *= value2; }
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T1>
        constexpr vector<S, T1> operator*(const T1 value1, const vector<S, T2>& value2) noexcept { return value2.template copy<T1>() *= value1; }
        template<vector_size_type S, typename T>
        constexpr vector<S, T> operator*(const T value1, vector<S, T>&& value2) noexcept { return value2 *= value1; }

        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<S, T1> operator/(const vector<S, T1>& value1, const T2 value2) noexcept { return value1.copy() /= value2; }
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<S, T1> operator/(vector<S, T1>&& value1, const T2 value2) noexcept { return value1 /= value2; }
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T1>
        constexpr vector<S, T1> operator/(const T1 value1, const vector<S, T2>& value2) noexcept { return vector<S, T1>(value1) /= value2; }



        template<vector_size_type S, typename T1, typename T2>
        constexpr vector<S, T1> operator+(const vector<S, T1>& value1, const vector<S, T2>& value2) noexcept { return value1.copy() += value2; }
        template<vector_size_type S, typename T1, typename T2>
        constexpr vector<S, T1> operator+(vector<S, T1>&& value1, const vector<S, T2>& value2) noexcept { return value1 += value2; }
        template<vector_size_type S, typename T1, typename T2>
        constexpr vector<S, T1> operator+(vector<S, T1>&& value1, vector<S, T2>&& value2) noexcept { return value1 += value2; }
        template<vector_size_type S, typename T>
        constexpr vector<S, T> operator+(const vector<S, T>& value1, vector<S, T>&& value2) noexcept { return value2 += value1; }
        
        template<vector_size_type S, typename T1, typename T2>
        constexpr vector<S, T1> operator-(const vector<S, T1>& value1, const vector<S, T2>& value2) noexcept { return value1.copy() -= value2; }
        template<vector_size_type S, typename T1, typename T2>
        constexpr vector<S, T1> operator-(vector<S, T1>&& value1, const vector<S, T2>& value2) noexcept { return value1 -= value2; }
        
        template<vector_size_type S, typename T1, typename T2>
        constexpr vector<S, T1> operator*(const vector<S, T1>& value1, const vector<S, T2>& value2) noexcept { return value1.copy() *= value2; }
        template<vector_size_type S, typename T1, typename T2>
        constexpr vector<S, T1> operator*(vector<S, T1>&& value1, const vector<S, T2>& value2) noexcept { return value1 *= value2; }
        template<vector_size_type S, typename T1, typename T2>
        constexpr vector<S, T1> operator*(vector<S, T1>&& value1, vector<S, T2>&& value2) noexcept { return value1 *= value2; }
        template<vector_size_type S, typename T>
        constexpr vector<S, T> operator*(const vector<S, T>& value1, vector<S, T>&& value2) noexcept { return value2 *= value1; }

        template<vector_size_type S, typename T1, typename T2>
        constexpr vector<S, T1> operator/(const vector<S, T1>& value1, const vector<S, T2>& value2) noexcept { return value1.copy() /= value2; }
        template<vector_size_type S, typename T1, typename T2>
        constexpr vector<S, T1> operator/(vector<S, T1>&& value1, const vector<S, T2>& value2) noexcept { return value1 /= value2; }
    }
}
