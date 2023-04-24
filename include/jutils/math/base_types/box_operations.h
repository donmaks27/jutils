// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "box_base.h"

namespace jutils
{
    namespace math
    {
        template<vector_size_type Size, typename T> requires jutils::math::is_valid_box_v<Size, T>
        constexpr box<Size, T> abs(const box<Size, T>& value) noexcept { return value.abs(); }

        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr box<S, T1> operator+(const box<S, T1>& value1, const T2& value2) noexcept { return value1.copy() += value2; }
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr box<S, T1> operator+(box<S, T1>&& value1, const T2& value2) noexcept { return value1 += value2; }
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T1>
        constexpr box<S, T1> operator+(const T1& value1, const box<S, T2>& value2) noexcept { return value2.template copy<T1>() += value1; }
        template<vector_size_type S, typename T>
        constexpr box<S, T> operator+(const T& value1, box<S, T>&& value2) noexcept { return value2 += value1; }
        
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr box<S, T1> operator-(const box<S, T1>& value1, const T2& value2) noexcept { return value1.copy() -= value2; }
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr box<S, T1> operator-(box<S, T1>&& value1, const T2& value2) noexcept { return value1 -= value2; }
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T1>
        constexpr box<S, T1> operator-(const T1& value1, const box<S, T2>& value2) noexcept { return -value2.template copy<T1>() + value1; }
        
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr box<S, T1> operator*(const box<S, T1>& value1, const T2& value2) noexcept { return value1.copy() *= value2; }
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr box<S, T1> operator*(box<S, T1>&& value1, const T2& value2) noexcept { return value1 *= value2; }
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T1>
        constexpr box<S, T1> operator*(const T1& value1, const box<S, T2>& value2) noexcept { return value2.template copy<T1>() *= value1; }
        template<vector_size_type S, typename T>
        constexpr box<S, T> operator*(const T& value1, box<S, T>&& value2) noexcept { return value2 *= value1; }
        
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr box<S, T1> operator/(const box<S, T1>& value1, const T2& value2) noexcept { return value1.copy() /= value2; }
        template<vector_size_type S, typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr box<S, T1> operator/(box<S, T1>&& value1, const T2& value2) noexcept { return value1 /= value2; }



        template<vector_size_type S, typename T1, typename T2>
        constexpr box<S, T1> operator+(const box<S, T1>& value1, const vector<S, T2>& value2) noexcept { return value1.copy() += value2; }
        template<vector_size_type S, typename T1, typename T2>
        constexpr box<S, T1> operator+(box<S, T1>&& value1, const vector<S, T2>& value2) noexcept { return value1 += value2; }
        template<vector_size_type S, typename T1, typename T2>
        constexpr box<S, T1> operator+(const vector<S, T1>& value1, const box<S, T2>& value2) noexcept { return value2.template copy<T1>() += value1; }
        template<vector_size_type S, typename T>
        constexpr box<S, T> operator+(const vector<S, T>& value1, box<S, T>&& value2) noexcept { return value2 += value1; }

        template<vector_size_type S, typename T1, typename T2>
        constexpr box<S, T1> operator-(const box<S, T1>& value1, const vector<S, T2>& value2) noexcept { return value1.copy() -= value2; }
        template<vector_size_type S, typename T1, typename T2>
        constexpr box<S, T1> operator-(box<S, T1>&& value1, const vector<S, T2>& value2) noexcept { return value1 -= value2; }
        template<vector_size_type S, typename T1, typename T2>
        constexpr box<S, T1> operator-(const vector<S, T1>& value1, const box<S, T2>& value2) noexcept { return -value2.template copy<T1>() + value1; }

        template<vector_size_type S, typename T1, typename T2>
        constexpr box<S, T1> operator*(const box<S, T1>& value1, const vector<S, T2>& value2) noexcept { return value1.copy() *= value2; }
        template<vector_size_type S, typename T1, typename T2>
        constexpr box<S, T1> operator*(box<S, T1>&& value1, const vector<S, T2>& value2) noexcept { return value1 *= value2; }
        template<vector_size_type S, typename T1, typename T2>
        constexpr box<S, T1> operator*(const vector<S, T1>& value1, const box<S, T2>& value2) noexcept { return value2.template copy<T1>() *= value1; }
        template<vector_size_type S, typename T>
        constexpr box<S, T> operator*(const vector<S, T>& value1, box<S, T>&& value2) noexcept { return value2 *= value1; }

        template<vector_size_type S, typename T1, typename T2>
        constexpr box<S, T1> operator/(const box<S, T1>& value1, const vector<S, T2>& value2) noexcept { return value1.copy() /= value2; }
        template<vector_size_type S, typename T1, typename T2>
        constexpr box<S, T1> operator/(box<S, T1>&& value1, const vector<S, T2>& value2) noexcept { return value1 /= value2; }
    }
}
