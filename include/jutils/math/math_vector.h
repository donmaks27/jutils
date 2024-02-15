// Copyright © 2024 Leonov Maksim. All rights reserved.

#pragma once

#include "./math.h"
#include "base_types/vector_base.h"

namespace jutils::math
{
    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T2>, vector_size_type S, typename T1, typename T2)
    [[nodiscard]] constexpr vector<S, T1> operator+(const vector<S, T1>& value1, const T2 value2) noexcept { return value1.copy() += value2; }
    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T2>, vector_size_type S, typename T1, typename T2)
    [[nodiscard]] constexpr vector<S, T1> operator+(vector<S, T1>&& value1, const T2 value2) noexcept { return value1 += value2; }
    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1>, vector_size_type S, typename T1, typename T2)
    [[nodiscard]] constexpr vector<S, T1> operator+(const T1 value1, const vector<S, T2>& value2) noexcept { return vector<S, T1>(value1) += value2; }
    template<vector_size_type S, typename T>
    [[nodiscard]] constexpr vector<S, T> operator+(const T value1, vector<S, T>&& value2) noexcept { return value2 += value1; }

    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T2>, vector_size_type S, typename T1, typename T2)
    [[nodiscard]] constexpr vector<S, T1> operator-(const vector<S, T1>& value1, const T2 value2) noexcept { return value1.copy() -= value2; }
    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T2>, vector_size_type S, typename T1, typename T2)
    [[nodiscard]] constexpr vector<S, T1> operator-(vector<S, T1>&& value1, const T2 value2) noexcept { return value1 -= value2; }
    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1>, vector_size_type S, typename T1, typename T2)
    [[nodiscard]] constexpr vector<S, T1> operator-(const T1 value1, const vector<S, T2>& value2) noexcept { return vector<S, T1>(value1) -= value2; }

    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T2>, vector_size_type S, typename T1, typename T2)
    [[nodiscard]] constexpr vector<S, T1> operator*(const vector<S, T1>& value1, const T2 value2) noexcept { return value1.copy() *= value2; }
    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T2>, vector_size_type S, typename T1, typename T2)
    [[nodiscard]] constexpr vector<S, T1> operator*(vector<S, T1>&& value1, const T2 value2) noexcept { return value1 *= value2; }
    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1>, vector_size_type S, typename T1, typename T2)
    [[nodiscard]] constexpr vector<S, T1> operator*(const T1 value1, const vector<S, T2>& value2) noexcept { return vector<S, T1>(value1) *= value2; }
    template<vector_size_type S, typename T>
    [[nodiscard]] constexpr vector<S, T> operator*(const T value1, vector<S, T>&& value2) noexcept { return value2 *= value1; }

    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T2>, vector_size_type S, typename T1, typename T2)
    [[nodiscard]] constexpr vector<S, T1> operator/(const vector<S, T1>& value1, const T2 value2) noexcept { return value1.copy() /= value2; }
    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T2>, vector_size_type S, typename T1, typename T2)
    [[nodiscard]] constexpr vector<S, T1> operator/(vector<S, T1>&& value1, const T2 value2) noexcept { return value1 /= value2; }
    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1>, vector_size_type S, typename T1, typename T2)
    [[nodiscard]] constexpr vector<S, T1> operator/(const T1 value1, const vector<S, T2>& value2) noexcept { return vector<S, T1>(value1) /= value2; }



    template<vector_size_type S, typename T1, typename T2>
    [[nodiscard]] constexpr vector<S, T1> operator+(const vector<S, T1>& value1, const vector<S, T2>& value2) noexcept { return value1.copy() += value2; }
    template<vector_size_type S, typename T1, typename T2>
    [[nodiscard]] constexpr vector<S, T1> operator+(vector<S, T1>&& value1, const vector<S, T2>& value2) noexcept { return value1 += value2; }
    template<vector_size_type S, typename T1, typename T2>
    [[nodiscard]] constexpr vector<S, T1> operator+(vector<S, T1>&& value1, vector<S, T2>&& value2) noexcept { return value1 += value2; }
    template<vector_size_type S, typename T>
    [[nodiscard]] constexpr vector<S, T> operator+(const vector<S, T>& value1, vector<S, T>&& value2) noexcept { return value2 += value1; }

    template<vector_size_type S, typename T1, typename T2>
    [[nodiscard]] constexpr vector<S, T1> operator-(const vector<S, T1>& value1, const vector<S, T2>& value2) noexcept { return value1.copy() -= value2; }
    template<vector_size_type S, typename T1, typename T2>
    [[nodiscard]] constexpr vector<S, T1> operator-(vector<S, T1>&& value1, const vector<S, T2>& value2) noexcept { return value1 -= value2; }

    template<vector_size_type S, typename T1, typename T2>
    [[nodiscard]] constexpr vector<S, T1> operator*(const vector<S, T1>& value1, const vector<S, T2>& value2) noexcept { return value1.copy() *= value2; }
    template<vector_size_type S, typename T1, typename T2>
    [[nodiscard]] constexpr vector<S, T1> operator*(vector<S, T1>&& value1, const vector<S, T2>& value2) noexcept { return value1 *= value2; }
    template<vector_size_type S, typename T1, typename T2>
    [[nodiscard]] constexpr vector<S, T1> operator*(vector<S, T1>&& value1, vector<S, T2>&& value2) noexcept { return value1 *= value2; }
    template<vector_size_type S, typename T>
    [[nodiscard]] constexpr vector<S, T> operator*(const vector<S, T>& value1, vector<S, T>&& value2) noexcept { return value2 *= value1; }

    template<vector_size_type S, typename T1, typename T2>
    [[nodiscard]] constexpr vector<S, T1> operator/(const vector<S, T1>& value1, const vector<S, T2>& value2) noexcept { return value1.copy() /= value2; }
    template<vector_size_type S, typename T1, typename T2>
    [[nodiscard]] constexpr vector<S, T1> operator/(vector<S, T1>&& value1, const vector<S, T2>& value2) noexcept { return value1 /= value2; }
}
