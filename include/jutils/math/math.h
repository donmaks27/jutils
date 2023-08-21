// Copyright © 2021-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../type_defines.h"

#include <cmath>
#include <concepts>
#include <numbers>
#include <initializer_list>

namespace jutils
{
    namespace math
    {
        template<typename T = float> requires std::floating_point<T>
        inline constexpr T EpsDefault = static_cast<T>(0.0001l);
        template<typename T = float> requires std::floating_point<T>
        inline constexpr T Pi = std::numbers::pi_v<T>;

        template<typename T>
        constexpr const T& max(const T& value1, const T& value2) noexcept( noexcept(value1 < value2) )
            { return std::max(value1, value2); }
        template<typename T1, typename T2> requires std::convertible_to<T2, T1>
        constexpr T1 max(const T1& value1, const T2& value2) noexcept( noexcept(value1 < static_cast<T1>(value2)) )
            { return value1 < static_cast<T1>(value2) ? static_cast<T1>(value2) : value1; }
        template<typename T>
        constexpr T max(std::initializer_list<T> values) { return std::max(values); }

        template<typename T>
        constexpr const T& min(const T& value1, const T& value2)
            { return value1 < value2 ? value1 : value2; }
        template<typename T1, typename T2> requires std::convertible_to<T2, T1>
        constexpr T1 min(const T1& value1, const T2& value2)
            { return value1 < static_cast<T1>(value2) ? value1 : static_cast<T1>(value2); }
        
        template<typename T1, typename T2, typename T3> requires std::convertible_to<T2, T1> && std::convertible_to<T3, T1>
        constexpr bool isWithin(const T1& value, const T2& minValue, const T3& maxValue)
            { return (static_cast<T1>(minValue) <= value) && (value <= static_cast<T1>(maxValue)); }

        template<typename T>
        constexpr const T& clamp(const T& value, const T& minValue, const T& maxValue)
            { return jutils::math::min(jutils::math::max(value, minValue), maxValue); }
        template<typename T1, typename T2, typename T3> requires std::convertible_to<T2, T1> && std::convertible_to<T3, T1>
        constexpr T1 clamp(const T1& value, const T2& minValue, const T3& maxValue)
            { return jutils::math::min(jutils::math::max(value, minValue), maxValue); }

        template<typename T> requires std::is_arithmetic_v<T>
        constexpr T abs(const T& value) noexcept { return value < static_cast<T>(0) ? -value : value; }
        
        template<typename T> requires std::floating_point<T>
        constexpr bool isEqual(const T value1, const T value2, const T eps = EpsDefault<T>) noexcept
            { return jutils::math::abs(value2 - value1) < eps; }
        template<typename T> requires std::floating_point<T>
        constexpr bool isNearlyZero(const T value, const T eps = EpsDefault<T>) noexcept
            { return jutils::math::isEqual(value, static_cast<T>(0), eps); }

        template<typename T> requires std::is_arithmetic_v<T>
        constexpr T sqr(const T value) noexcept { return value * value; }
        template<typename T, typename E> requires std::is_arithmetic_v<T> && std::is_arithmetic_v<E>
        auto pow(const T base, const E exp) { return std::pow(base, exp); }
        template<typename T> requires std::is_arithmetic_v<T>
        auto sqrt(const T value) { return std::sqrt(value); }
        
        template<typename R = int32, typename T = float> requires std::integral<R> && std::floating_point<T>
        R round(const T value) { return static_cast<R>(std::round(value)); }
        template<typename R = int32, typename T = float> requires std::integral<R> && std::floating_point<T>
        R roundDown(const T value) { return static_cast<R>(std::floor(value)); }
        template<typename R = int32, typename T = float> requires std::integral<R> && std::floating_point<T>
        R roundUp(const T value) { return static_cast<R>(std::ceil(value)); }

        template<typename T> requires std::is_arithmetic_v<T>
        constexpr auto degreesToRads(const T degrees)
        {
            using R = std::conditional_t<std::is_floating_point_v<T>, T, double>;
            return static_cast<R>(degrees) * (std::numbers::pi_v<R> / static_cast<R>(180));
        }
        template<typename T> requires std::is_arithmetic_v<T>
        constexpr auto radsToDegrees(const T rads)
        {
            using R = std::conditional_t<std::is_floating_point_v<T>, T, double>;
            return static_cast<R>(rads) * (static_cast<R>(180) / std::numbers::pi_v<R>);
        }
        template<typename T> requires std::is_arithmetic_v<T>
        auto sin(const T angleRad) noexcept { return std::sin(angleRad); }
        template<typename T> requires std::is_arithmetic_v<T>
        auto cos(const T angleRad) noexcept { return std::cos(angleRad); }
        template<typename T> requires std::is_arithmetic_v<T>
        auto tan(const T angleRad) noexcept { return std::tan(angleRad); }
        template<typename T> requires std::is_arithmetic_v<T>
        auto atan(const T angleRad) noexcept { return std::atan(angleRad); }
    }
}
