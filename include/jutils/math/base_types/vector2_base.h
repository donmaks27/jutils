﻿// Copyright © 2021-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vector_base.h"

#include <cassert>

namespace jutils
{
    namespace math
    {
        template<typename T> requires std::is_arithmetic_v<T>
        class vector<2, T>
        {
        public:

            static constexpr vector_size_type size = 2;
            using type = T;
            
            constexpr vector() noexcept = default;
            constexpr vector(const type x, const type y) noexcept
                : x(x), y(y)
            {}
            explicit constexpr vector(const type value) noexcept
                : x(value), y(value)
            {}
            template<vector_size_type Size, typename Other> requires (Size >= size)
            constexpr vector(const vector<Size, Other>& value) noexcept
                : x(static_cast<type>(value.x)), y(static_cast<type>(value.y))
            {}

            template<vector_size_type Size, typename Other> requires (Size >= size)
            constexpr vector& operator=(const vector<Size, Other>& value) noexcept
            {
                x = static_cast<type>(value.x);
                y = static_cast<type>(value.y);
                return *this;
            }

            type x = 0;
            type y = 0;
            
            template<typename Other>
            constexpr bool operator==(const vector<size, Other>& value) const noexcept
            {
                if constexpr (std::is_integral_v<type>)
                {
                    return (x == static_cast<type>(value.x)) && (y == static_cast<type>(value.y));
                }
                else
                {
                    return jutils::math::isEqual(x, static_cast<type>(value.x))
                        && jutils::math::isEqual(y, static_cast<type>(value.y));
                }
            }
            template<typename Other>
            constexpr bool operator!=(const vector<size, Other>& value) const noexcept { return !this->operator==(value); }

            constexpr type& get(const vector_size_type index)
            {
                _checkIndexValid(index);
                return index == 0 ? x : y;
            }
            constexpr const type& get(const vector_size_type index) const
            {
                _checkIndexValid(index);
                return index == 0 ? x : y;
            }
            constexpr type& operator[](const vector_size_type index) { return get(index); }
            constexpr const type& operator[](const vector_size_type index) const { return get(index); }

            constexpr type* getData() noexcept { return &x; }
            constexpr const type* getData() const noexcept { return &x; }

            template<typename R = type>
            constexpr vector<size, R> copy() const noexcept { return *this; }
            constexpr vector abs() const noexcept { return { jutils::math::abs(x), jutils::math::abs(y) }; }
            constexpr type lengthSqr() const noexcept { return x * x + y * y; }
            template<typename Other>
            constexpr type dot(const vector<size, Other>& value) const noexcept
            {
                return x * static_cast<type>(value.x) + y * static_cast<type>(value.y);
            }

            template<typename R = std::conditional_t<std::is_floating_point_v<type>, type, float>> requires std::floating_point<R>
            R length() const noexcept { return jutils::math::vectorLength<size, type, R>(*this); }
            template<typename R = std::conditional_t<std::is_floating_point_v<type>, type, float>> requires std::floating_point<R>
            vector<size, R> normalize(const R eps = jutils::math::EpsDefault<R>) const noexcept
            {
                return jutils::math::vectorNormalize<size, type, R>(*this, eps);
            }

            constexpr vector& operator++() noexcept { ++x; ++y; return *this; }
            constexpr vector& operator--() noexcept { --x; --y; return *this; }
            constexpr vector operator++(int) noexcept { vector temp = copy(); this->operator++(); return temp; }
            constexpr vector operator--(int) noexcept { vector temp = copy(); this->operator--(); return temp; }
            constexpr vector operator-() const noexcept { return { -x, -y }; }

            template<typename Other> requires std::is_arithmetic_v<Other>
            constexpr vector& operator+=(const Other value) noexcept
            {
                x += value; y += value;
                return *this;
            }
            template<typename Other>
            constexpr vector& operator+=(const vector<size, Other>& value) noexcept
            {
                x += value.x; y += value.y;
                return *this;
            }
            template<typename Other> requires std::is_arithmetic_v<Other>
            constexpr vector& operator-=(const Other value) noexcept
            {
                x -= value; y -= value;
                return *this;
            }
            template<typename Other>
            constexpr vector& operator-=(const vector<size, Other>& value) noexcept
            {
                x -= value.x; y -= value.y;
                return *this;
            }
            template<typename Other> requires std::is_arithmetic_v<Other>
            constexpr vector& operator*=(const Other value) noexcept
            {
                x *= value; y *= value;
                return *this;
            }
            template<typename Other>
            constexpr vector& operator*=(const vector<size, Other>& value) noexcept
            {
                x *= value.x; y *= value.y;
                return *this;
            }
            template<typename Other> requires std::is_arithmetic_v<Other>
            constexpr vector& operator/=(const Other value) noexcept
            {
                x /= value; y /= value;
                return *this;
            }
            template<typename Other>
            constexpr vector& operator/=(const vector<size, Other>& value) noexcept
            {
                x /= value.x; y /= value.y;
                return *this;
            }

            jstring toString() const noexcept { return JSTR_FORMAT("{{ {}; {} }}", x, y); }

        private:

            static constexpr void _checkIndexValid(const vector_size_type index)
            {
                if ((index < 0) || (index >= size))
                {
                    throw std::out_of_range("Invalid index");
                }
            }
        };

        template<typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<2, T1> operator+(const vector<2, T1>& value1, const T2 value2) noexcept { return value1.copy() += value2; }
        template<typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<2, T1> operator+(vector<2, T1>&& value1, const T2 value2) noexcept { return value1 += value2; }
        template<typename T1, typename T2>
        constexpr vector<2, T1> operator+(const vector<2, T1>& value1, const vector<2, T2>& value2) noexcept { return value1.copy() += value2; }
        template<typename T1, typename T2>
        constexpr vector<2, T1> operator+(vector<2, T1>&& value1, const vector<2, T2>& value2) noexcept { return value1 += value2; }

        template<typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<2, T1> operator-(const vector<2, T1>& value1, const T2 value2) noexcept { return value1.copy() -= value2; }
        template<typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<2, T1> operator-(vector<2, T1>&& value1, const T2 value2) noexcept { return value1 -= value2; }
        template<typename T1, typename T2>
        constexpr vector<2, T1> operator-(const vector<2, T1>& value1, const vector<2, T2>& value2) noexcept { return value1.copy() -= value2; }
        template<typename T1, typename T2>
        constexpr vector<2, T1> operator-(vector<2, T1>&& value1, const vector<2, T2>& value2) noexcept { return value1 -= value2; }

        template<typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<2, T1> operator*(const vector<2, T1>& value1, const T2 value2) noexcept { return value1.copy() *= value2; }
        template<typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<2, T1> operator*(vector<2, T1>&& value1, const T2 value2) noexcept { return value1 *= value2; }
        template<typename T1, typename T2> requires std::is_arithmetic_v<T1>
        constexpr vector<2, T1> operator*(const T1 value1, const vector<2, T2>& value2) noexcept { return vector<2, T1>(value1) *= value2; }
        template<typename T1, typename T2>
        constexpr vector<2, T1> operator*(const vector<2, T1>& value1, const vector<2, T2>& value2) noexcept { return value1.copy() *= value2; }
        template<typename T1, typename T2>
        constexpr vector<2, T1> operator*(vector<2, T1>&& value1, const vector<2, T2>& value2) noexcept { return value1 *= value2; }

        template<typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<2, T1> operator/(const vector<2, T1>& value1, const T2 value2) noexcept { return value1.copy() /= value2; }
        template<typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<2, T1> operator/(vector<2, T1>&& value1, const T2 value2) noexcept { return value1 /= value2; }
        template<typename T1, typename T2> requires std::is_arithmetic_v<T1>
        constexpr vector<2, T1> operator/(const T1 value1, const vector<2, T2>& value2) noexcept { return vector<2, T1>(value1) /= value2; }
        template<typename T1, typename T2>
        constexpr vector<2, T1> operator/(const vector<2, T1>& value1, const vector<2, T2>& value2) noexcept { return value1.copy() /= value2; }
        template<typename T1, typename T2>
        constexpr vector<2, T1> operator/(vector<2, T1>&& value1, const vector<2, T2>& value2) noexcept { return value1 /= value2; }
    }
}
