// Copyright © 2021-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vector2_base.h"

namespace jutils::math
{
    template<typename T>
    class vector<3, T>
    {
        static_assert(std::is_arithmetic_v<T>);

        using default_float_type = std::conditional_t<std::is_floating_point_v<T>, T, float>;

    public:

        static constexpr vector_size_type size = 3;
        using type = T;

        type x = 0;
        type y = 0;
        type z = 0;

        constexpr vector() noexcept = default;
        constexpr vector(const type x, const type y, const type z) noexcept
            : x(x), y(y), z(z)
        {}
        explicit constexpr vector(const type value) noexcept
            : x(value), y(value), z(value)
        {}
        JUTILS_TEMPLATE_CONDITION((S1 >= size), vector_size_type S1, typename T1)
        constexpr vector(const vector<S1, T1>& value) noexcept
            : x(static_cast<type>(value.x)), y(static_cast<type>(value.y)), z(static_cast<type>(value.z))
        {}
        template<typename T1>
        constexpr vector(const vector<2, T1>& value, const type z = 0) noexcept
            : x(static_cast<type>(value.x)), y(static_cast<type>(value.y)), z(z)
        {}

        JUTILS_TEMPLATE_CONDITION((S1 >= size), vector_size_type S1, typename T1)
        constexpr vector& operator=(const vector<S1, T1>& value) noexcept
        {
            x = static_cast<type>(value.x);
            y = static_cast<type>(value.y);
            z = static_cast<type>(value.z);
            return *this;
        }
        template<typename T1>
        constexpr vector& operator=(const vector<2, T1>& value) noexcept
        {
            x = static_cast<type>(value.x);
            y = static_cast<type>(value.y);
            z = 0;
            return *this;
        }

        [[nodiscard]] constexpr type* getData() noexcept { return &x; }
        [[nodiscard]] constexpr const type* getData() const noexcept { return &x; }

        [[nodiscard]] constexpr type& get(const vector_size_type index) noexcept
        {
            _checkIndexValid(index);
            return index == 0 ? x : index == 1 ? y : z;
        }
        [[nodiscard]] constexpr const type& get(const vector_size_type index) const noexcept
        {
            _checkIndexValid(index);
            return index == 0 ? x : index == 1 ? y : z;
        }
        [[nodiscard]] constexpr type& operator[](const vector_size_type index) noexcept { return get(index); }
        [[nodiscard]] constexpr const type& operator[](const vector_size_type index) const noexcept { return get(index); }

        template<typename T1 = type>
        [[nodiscard]] constexpr vector<size, T1> copy() const noexcept { return *this; }

        template<typename T1>
        [[nodiscard]] constexpr bool isEqual(const vector<size, T1>& value, const default_float_type eps) const noexcept
        {
            if constexpr (std::is_floating_point_v<type>)
            {
                return jutils::math::isEqual(x, static_cast<type>(value.x), eps)
                    && jutils::math::isEqual(y, static_cast<type>(value.y), eps)
                    && jutils::math::isEqual(z, static_cast<type>(value.z), eps);
            }
            else
            {
                return isEqual(value);
            }
        }
        template<typename T1>
        [[nodiscard]] constexpr bool isEqual(const vector<size, T1>& value) const noexcept
        {
            if constexpr (std::is_floating_point_v<type>)
            {
                return isEqual(value, jutils::math::eps<type>);
            }
            else
            {
                return (x == static_cast<type>(value.x)) && (y == static_cast<type>(value.y)) && (z == static_cast<type>(value.z));
            }
        }

        [[nodiscard]] constexpr vector abs() const noexcept { return { jutils::math::abs(x), jutils::math::abs(y), jutils::math::abs(z) }; }
        [[nodiscard]] constexpr type lengthSqr() const noexcept { return x * x + y * y + z * z; }
        JUTILS_TEMPLATE_CONDITION(std::is_floating_point_v<T1>, typename T1 = default_float_type)
        [[nodiscard]] T1 length() const noexcept { return std::sqrt(static_cast<T1>(lengthSqr())); }
        JUTILS_TEMPLATE_CONDITION(std::is_floating_point_v<T1>, typename T1 = default_float_type)
        [[nodiscard]] vector<size, T1> normalize(T1 eps = jutils::math::eps<T1>) const noexcept
        {
            static constexpr vector zero;
            return isEqual(zero) ? zero : copy<T1>() /= length<T1>();
        }

        template<typename T1>
        [[nodiscard]] constexpr type dot(const vector<size, T1>& value) const noexcept
        {
            return x * static_cast<type>(value.x) + y * static_cast<type>(value.y) + z * static_cast<type>(value.z);
        }
        template<typename T1>
        constexpr vector cross(const vector<size, T1>& value) const noexcept
        {
            return vector(
                y * static_cast<type>(value.z) - z * static_cast<type>(value.y),
                z * static_cast<type>(value.x) - x * static_cast<type>(value.z),
                x * static_cast<type>(value.y) - y * static_cast<type>(value.x)
            );
        }

        [[nodiscard]] jstring toString() const noexcept { return jutils::format("{{ {}; {}; {} }}", x, y, z); }

        template<typename T1>
        [[nodiscard]] constexpr bool operator==(const vector<size, T1>& value) const noexcept { return isEqual(value); }
        template<typename T1>
        [[nodiscard]] constexpr bool operator!=(const vector<size, T1>& value) const noexcept { return !isEqual(value); }

        constexpr vector& operator++() noexcept { ++x; ++y; ++z; return *this; }
        constexpr vector& operator--() noexcept { --x; --y; --z; return *this; }
        [[nodiscard]] constexpr vector operator++(int) noexcept { vector temp = copy(); this->operator++(); return temp; }
        [[nodiscard]] constexpr vector operator--(int) noexcept { vector temp = copy(); this->operator--(); return temp; }

        [[nodiscard]] constexpr vector operator-() const noexcept { return { -x, -y, -z }; }

        JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1>, typename T1)
        constexpr vector& operator+=(const T1 value) noexcept
        {
            x += static_cast<type>(value); y += static_cast<type>(value); z += static_cast<type>(value);
            return *this;
        }
        JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1>, typename T1)
        constexpr vector& operator-=(const T1 value) noexcept
        {
            x -= static_cast<type>(value); y -= static_cast<type>(value); z -= static_cast<type>(value);
            return *this;
        }
        JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1>, typename T1)
        constexpr vector& operator*=(const T1 value) noexcept
        {
            x *= static_cast<type>(value); y *= static_cast<type>(value); z *= static_cast<type>(value);
            return *this;
        }
        JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1>, typename T1)
        constexpr vector& operator/=(const T1 value) noexcept
        {
            x /= static_cast<type>(value); y /= static_cast<type>(value); z /= static_cast<type>(value);
            return *this;
        }

        JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1> && (S1 >= size), vector_size_type S1, typename T1)
        constexpr vector& operator+=(const vector<S1, T1>& value) noexcept
        {
            x += static_cast<type>(value.x); y += static_cast<type>(value.y); z += static_cast<type>(value.z);
            return *this;
        }
        JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1> && (S1 >= size), vector_size_type S1, typename T1)
        constexpr vector& operator-=(const vector<S1, T1>& value) noexcept
        {
            x -= static_cast<type>(value.x); y -= static_cast<type>(value.y); z -= static_cast<type>(value.z);
            return *this;
        }
        JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1> && (S1 >= size), vector_size_type S1, typename T1)
        constexpr vector& operator*=(const vector<S1, T1>& value) noexcept
        {
            x *= static_cast<type>(value.x); y *= static_cast<type>(value.y); z *= static_cast<type>(value.z);
            return *this;
        }
        JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1> && (S1 >= size), vector_size_type S1, typename T1)
        constexpr vector& operator/=(const vector<S1, T1>& value) noexcept
        {
            x /= static_cast<type>(value.x); y /= static_cast<type>(value.y); z /= static_cast<type>(value.z);
            return *this;
        }

    private:

        static constexpr void _checkIndexValid(const vector_size_type index)
        {
            if ((index < 0) || (index >= size))
            {
                throw std::out_of_range("Invalid index");
            }
        }
    };
}
