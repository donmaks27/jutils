// Copyright © 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../core.h"
#include "../macro_template_condition.h"

#ifndef JUTILS_MODULE
    #include "../base_types.h"
    #include "../type_traits.h"
    #include <cmath>
    #include <initializer_list>
    #if JUTILS_STD_VERSION >= JUTILS_STD20
        #include <numbers>
    #endif
    #ifdef JUTILS_USE_GLM
        #include <glm/detail/qualifier.hpp>
    #endif
#endif

JUTILS_MODULE_EXPORT namespace jutils::math
{
    JUTILS_TEMPLATE_CONDITION(std::is_floating_point_v<T>, typename T = float)
    inline constexpr T eps = static_cast<T>(0.00000001l);
    JUTILS_TEMPLATE_CONDITION(std::is_floating_point_v<T>, typename T = float)
    inline constexpr T pi =
#if JUTILS_STD_VERSION >= JUTILS_STD20
        std::numbers::pi_v<T>;
#else
        static_cast<T>(3.141592653589793238462643383279502884L);
#endif

    template<typename T>
    [[nodiscard]] constexpr const T& max(const T& value1, const T& value2)
        { return std::max(value1, value2); }
    JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, T, T>), typename T, typename Pred)
    [[nodiscard]] constexpr const T& max(const T& value1, const T& value2, Pred pred)
        { return std::max(value1, value2, pred); }
    JUTILS_TEMPLATE_CONDITION((std::is_convertible_v<T2, T1>), typename T1, typename T2)
    [[nodiscard]] constexpr T1 max(const T1& value1, const T2& value2)
        { return value1 < static_cast<T1>(value2) ? static_cast<T1>(value2) : value1; }
    JUTILS_TEMPLATE_CONDITION((std::is_convertible_v<T2, T1> && jutils::is_predicate_v<Pred, T1, T2>), typename T1, typename T2, typename Pred)
    [[nodiscard]] constexpr T1 max(const T1& value1, const T2& value2, Pred pred)
        { return pred(value1, value2) ? static_cast<T1>(value2) : value1; }
    template<typename T>
    [[nodiscard]] constexpr T max(std::initializer_list<T> values)
        { return std::max(values); }
    JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, T, T>), typename T, typename Pred)
    [[nodiscard]] constexpr T max(std::initializer_list<T> values, Pred pred)
        { return std::max(values, pred); }

    template<typename T>
    [[nodiscard]] constexpr const T& min(const T& value1, const T& value2)
        { return std::min(value1, value2); }
    JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, T, T>), typename T, typename Pred)
    [[nodiscard]] constexpr const T& min(const T& value1, const T& value2, Pred pred)
        { return std::min(value1, value2, pred); }
    JUTILS_TEMPLATE_CONDITION((std::is_convertible_v<T2, T1>), typename T1, typename T2)
    [[nodiscard]] constexpr T1 min(const T1& value1, const T2& value2)
        { return value1 < static_cast<T1>(value2) ? value1 : static_cast<T1>(value2); }
    JUTILS_TEMPLATE_CONDITION((std::is_convertible_v<T2, T1> && jutils::is_predicate_v<Pred, T1, T2>), typename T1, typename T2, typename Pred)
    [[nodiscard]] constexpr T1 min(const T1& value1, const T2& value2, Pred pred)
        { return pred(value1, value2) ? value1 : static_cast<T1>(value2); }
    template<typename T>
    [[nodiscard]] constexpr T min(std::initializer_list<T> values)
        { return std::min(values); }
    JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, T, T>), typename T, typename Pred)
    [[nodiscard]] constexpr T min(std::initializer_list<T> values, Pred pred)
        { return std::min(values, pred); }

    JUTILS_TEMPLATE_CONDITION((std::is_convertible_v<T2, T1> && std::is_convertible_v<T3, T1>), typename T1, typename T2, typename T3)
    [[nodiscard]] constexpr bool isWithin(const T1& value, const T2& minValue, const T3& maxValue)
        { return (static_cast<T1>(minValue) <= value) && (value <= static_cast<T1>(maxValue)); }

    template<typename T>
    [[nodiscard]] constexpr const T& clamp(const T& value, const T& minValue, const T& maxValue)
        { return jutils::math::min(jutils::math::max(value, minValue), maxValue); }
    JUTILS_TEMPLATE_CONDITION((std::is_convertible_v<T2, T1> && std::is_convertible_v<T3, T1>), typename T1, typename T2, typename T3)
    [[nodiscard]] constexpr T1 clamp(const T1& value, const T2& minValue, const T3& maxValue)
        { return jutils::math::min(jutils::math::max(value, minValue), maxValue); }

    JUTILS_TEMPLATE_CONDITION((std::is_arithmetic_v<T> && std::is_signed_v<T>), typename T)
    [[nodiscard]] constexpr T abs(const T value) noexcept { return value < 0 ? -value : value; }

    JUTILS_TEMPLATE_CONDITION(std::is_floating_point_v<T>, typename T)
    [[nodiscard]] constexpr bool isEqual(const T value1, const T value2, const T eps = jutils::math::eps<T>) noexcept
        { return jutils::math::abs(value2 - value1) < eps; }
    JUTILS_TEMPLATE_CONDITION(std::is_floating_point_v<T>, typename T)
    [[nodiscard]] constexpr bool isNearlyZero(const T value, const T eps = jutils::math::eps<T>) noexcept
        { return jutils::math::isEqual<T>(value, 0, eps); }

    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T>, typename T)
    [[nodiscard]] constexpr auto sqr(const T value) noexcept { return value * value; }
    JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T>, typename T)
    [[nodiscard]] auto sqrt(const T value) { return std::sqrt(value); }

    JUTILS_TEMPLATE_CONDITION(std::is_integral_v<R> && std::is_floating_point_v<T>, typename R = int32, typename T = float)
    [[nodiscard]] R round(const T value) { return static_cast<R>(std::round(value)); }
    JUTILS_TEMPLATE_CONDITION(std::is_integral_v<R> && std::is_floating_point_v<T>, typename R = int32, typename T = float)
    [[nodiscard]] R roundDown(const T value) { return static_cast<R>(std::floor(value)); }
    JUTILS_TEMPLATE_CONDITION(std::is_integral_v<R> && std::is_floating_point_v<T>, typename R = int32, typename T = float)
    [[nodiscard]] R roundUp(const T value) { return static_cast<R>(std::ceil(value)); }

#ifdef JUTILS_USE_GLM

    JUTILS_TEMPLATE_CONDITION(
        (std::is_floating_point_v<T> && jutils::math::isWithin(Size, 2, 4)), 
        glm::length_t Size, typename T, glm::qualifier Q
    )
    [[nodiscard]] constexpr bool isEqual(const glm::vec<Size, T, Q>& value1, const glm::vec<Size, T, Q>& value2, const T eps = jutils::math::eps<T>) noexcept
    {
        for (glm::length_t i = 0; i < Size; i++)
        {
            if (!jutils::math::isEqual(value1[i], value2[i]))
            {
                return false;
            }
        }
        return true;
    }
    JUTILS_TEMPLATE_CONDITION(
        (std::is_floating_point_v<T> && jutils::math::isWithin(Size, 2, 4)), 
        glm::length_t Size, typename T, glm::qualifier Q
    )
    [[nodiscard]] constexpr bool isNearlyZero(const glm::vec<Size, T, Q>& value, const T eps = jutils::math::eps<T>) noexcept
    {
        for (glm::length_t i = 0; i < Size; i++)
        {
            if (!jutils::math::isNearlyZero(value[i]))
            {
                return false;
            }
        }
        return true;
    }

#endif
}


