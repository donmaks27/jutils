// Copyright © 2022-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vector2_base.h"

namespace jutils
{
    namespace math
    {
        template<typename T> requires std::is_arithmetic_v<T>
        class vector<3, T>
        {
        public:

            static constexpr vector_size_type size = 3;
            using type = T;

            constexpr vector() noexcept = default;
            constexpr vector(const type x, const type y, const type z) noexcept
                : x(x), y(y), z(z)
            {}
            explicit constexpr vector(const type value) noexcept
                : x(value), y(value), z(value)
            {}
            template<vector_size_type Size, typename Other> requires (Size >= size)
            constexpr vector(const vector<Size, Other>& value) noexcept
                : x(static_cast<type>(value.x)), y(static_cast<type>(value.y)), z(static_cast<type>(value.z))
            {}
            template<typename Other>
            constexpr vector(const vector<2, Other>& value) noexcept
                : x(static_cast<type>(value.x)), y(static_cast<type>(value.y)), z(0)
            {}

            template<vector_size_type Size, typename Other> requires (Size >= size)
            constexpr vector& operator=(const vector<Size, Other>& value) noexcept
            {
                x = static_cast<type>(value.x); y = static_cast<type>(value.y); z = static_cast<type>(value.z);
                return *this;
            }
            template<typename Other>
            constexpr vector& operator=(const vector<2, Other>& value) noexcept
            {
                x = static_cast<type>(value.x); y = static_cast<type>(value.y); z = 0;
                return *this;
            }

            type x = 0;
            type y = 0;
            type z = 0;

            template<typename Other>
            constexpr bool operator==(const vector<size, Other>& value) const noexcept
            {
                if constexpr (std::is_integral_v<type>)
                {
                    return (x == static_cast<type>(value.x)) && (y == static_cast<type>(value.y)) && (z == static_cast<type>(value.z));
                }
                else
                {
                    return jutils::math::isEqual(x, static_cast<type>(value.x))
                        && jutils::math::isEqual(y, static_cast<type>(value.y))
                        && jutils::math::isEqual(z, static_cast<type>(value.z));
                }
            }
            template<typename Other>
            constexpr bool operator!=(const vector<size, Other>& value) const noexcept { return !this->operator==(value); }

            constexpr type& get(const vector_size_type index)
            {
                _checkIndexValid(index);
                switch (index)
                {
                case 0: return x;
                case 1: return y;
                default: ;
                }
                return z;
            }
            constexpr const type& get(const vector_size_type index) const
            {
                _checkIndexValid(index);
                switch (index)
                {
                case 0: return x;
                case 1: return y;
                default: ;
                }
                return z;
            }
            constexpr type& operator[](const vector_size_type index) { return get(index); }
            constexpr const type& operator[](const vector_size_type index) const { return get(index); }
            
            constexpr type* getData() { return &x; }
            constexpr const type* getData() const { return &x; }
            
            template<typename R = type>
            constexpr vector<size, R> copy() const noexcept { return *this; }
            constexpr vector abs() const noexcept { return { jutils::math::abs(x), jutils::math::abs(y), jutils::math::abs(z) }; }
            constexpr type lengthSqr() const noexcept { return x * x + y * y + z * z; }
            template<typename Other>
            constexpr type dot(const vector<size, Other>& value) const noexcept
            {
                return x * static_cast<type>(value.x) + y * static_cast<type>(value.y) + z * static_cast<type>(value.z);
            }
            template<typename Other>
            constexpr vector cross(const vector<size, Other>& value) const noexcept
            {
                return vector(
                    y * static_cast<type>(value.z) - z * static_cast<type>(value.y),
                    z * static_cast<type>(value.x) - x * static_cast<type>(value.z),
                    x * static_cast<type>(value.y) - y * static_cast<type>(value.x)
                );
            }

            template<typename R = std::conditional_t<std::is_floating_point_v<type>, type, float>> requires std::floating_point<R>
            R length() const noexcept { return jutils::math::vectorLength<size, type, R>(*this); }
            template<typename R = std::conditional_t<std::is_floating_point_v<type>, type, float>> requires std::floating_point<R>
            vector<size, R> normalize(const R eps = jutils::math::EpsDefault<R>) const noexcept
            {
                return jutils::math::vectorNormalize<size, type, R>(*this, eps);
            }

            constexpr vector& operator++() noexcept { ++x; ++y; ++z; return *this; }
            constexpr vector& operator--() noexcept { --x; --y; --z; return *this; }
            constexpr vector operator++(int) noexcept { vector temp = copy(); this->operator++(); return temp; }
            constexpr vector operator--(int) noexcept { vector temp = copy(); this->operator--(); return temp; }
            constexpr vector operator-() const noexcept { return { -x, -y, -z }; }
            
            template<typename Other> requires std::is_arithmetic_v<Other>
            constexpr vector& operator+=(const Other value) noexcept
            {
                x += value; y += value; z += value;
                return *this;
            }
            template<typename Other>
            constexpr vector& operator+=(const vector<size, Other>& value) noexcept
            {
                x += value.x; y += value.y; z += value.z;
                return *this;
            }
            template<typename Other> requires std::is_arithmetic_v<Other>
            constexpr vector& operator-=(const Other value) noexcept
            {
                x -= value; y -= value; z -= value;
                return *this;
            }
            template<typename Other>
            constexpr vector& operator-=(const vector<size, Other>& value) noexcept
            {
                x -= value.x; y -= value.y; z -= value.z;
                return *this;
            }
            template<typename Other> requires std::is_arithmetic_v<Other>
            constexpr vector& operator*=(const Other value) noexcept
            {
                x *= value; y *= value; z *= value;
                return *this;
            }
            template<typename Other>
            constexpr vector& operator*=(const vector<size, Other>& value) noexcept
            {
                x *= value.x; y *= value.y; z *= value.z;
                return *this;
            }
            template<typename Other> requires std::is_arithmetic_v<Other>
            constexpr vector& operator/=(const Other value) noexcept
            {
                x /= value; y /= value; z /= value;
                return *this;
            }
            template<typename Other>
            constexpr vector& operator/=(const vector<size, Other>& value) noexcept
            {
                x /= value.x; y /= value.y; z /= value.z;
                return *this;
            }

            jstring toString() const noexcept { return JSTR_FORMAT("{{ {}; {}; {} }}", x, y, z); }

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
        constexpr vector<3, T1> operator+(const vector<3, T1>& value1, const T2 value2) noexcept { return value1.copy() += value2; }
        template<typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<3, T1> operator+(vector<3, T1>&& value1, const T2 value2) noexcept { return value1 += value2; }
        template<typename T1, typename T2>
        constexpr vector<3, T1> operator+(const vector<3, T1>& value1, const vector<3, T2>& value2) noexcept { return value1.copy() += value2; }
        template<typename T1, typename T2>
        constexpr vector<3, T1> operator+(vector<3, T1>&& value1, const vector<3, T2>& value2) noexcept { return value1 += value2; }

        template<typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<3, T1> operator-(const vector<3, T1>& value1, const T2 value2) noexcept { return value1.copy() -= value2; }
        template<typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<3, T1> operator-(vector<3, T1>&& value1, const T2 value2) noexcept { return value1 -= value2; }
        template<typename T1, typename T2>
        constexpr vector<3, T1> operator-(const vector<3, T1>& value1, const vector<3, T2>& value2) noexcept { return value1.copy() -= value2; }
        template<typename T1, typename T2>
        constexpr vector<3, T1> operator-(vector<3, T1>&& value1, const vector<3, T2>& value2) noexcept { return value1 -= value2; }

        template<typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<3, T1> operator*(const vector<3, T1>& value1, const T2 value2) noexcept { return value1.copy() *= value2; }
        template<typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<3, T1> operator*(vector<3, T1>&& value1, const T2 value2) noexcept { return value1 *= value2; }
        template<typename T1, typename T2> requires std::is_arithmetic_v<T1>
        constexpr vector<3, T1> operator*(const T1 value1, const vector<3, T2>& value2) noexcept { return vector<3, T1>(value1) *= value2; }
        template<typename T1, typename T2>
        constexpr vector<3, T1> operator*(const vector<3, T1>& value1, const vector<3, T2>& value2) noexcept { return value1.copy() *= value2; }
        template<typename T1, typename T2>
        constexpr vector<3, T1> operator*(vector<3, T1>&& value1, const vector<3, T2>& value2) noexcept { return value1 *= value2; }

        template<typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<3, T1> operator/(const vector<3, T1>& value1, const T2 value2) noexcept { return value1.copy() /= value2; }
        template<typename T1, typename T2> requires std::is_arithmetic_v<T2>
        constexpr vector<3, T1> operator/(vector<3, T1>&& value1, const T2 value2) noexcept { return value1 /= value2; }
        template<typename T1, typename T2> requires std::is_arithmetic_v<T1>
        constexpr vector<3, T1> operator/(const T1 value1, const vector<3, T2>& value2) noexcept { return vector<3, T1>(value1) /= value2; }
        template<typename T1, typename T2>
        constexpr vector<3, T1> operator/(const vector<3, T1>& value1, const vector<3, T2>& value2) noexcept { return value1.copy() /= value2; }
        template<typename T1, typename T2>
        constexpr vector<3, T1> operator/(vector<3, T1>&& value1, const vector<3, T2>& value2) noexcept { return value1 /= value2; }
    }
}
