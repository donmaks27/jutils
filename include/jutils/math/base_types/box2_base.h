﻿// Copyright © 2022-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "box_operations.h"
#include "vector2_base.h"

namespace jutils
{
    namespace math
    {
        template<typename T> requires std::is_arithmetic_v<T>
        class box<2, T>
        {
        public:

            static constexpr vector_size_type size = 2;
            using type = T;
            using vector_type = vector<size, type>;

            constexpr box() noexcept = default;
            constexpr box(const vector_type& v0, const vector_type& v1) noexcept
                : v0(v0), v1(v1)
            {}
            template<vector_size_type Size, typename Other> requires (Size >= size)
            constexpr box(const box<Size, Other>& value) noexcept
                : v0(value.v0), v1(value.v1)
            {}

            template<vector_size_type Size, typename Other> requires (Size >= size)
            constexpr box& operator=(const box<Size, Other>& value) noexcept
            {
                v0 = value.v0; v1 = value.v1;
                return *this;
            }

            vector_type v0;
            vector_type v1;

            template<typename Other>
            constexpr bool operator==(const box<size, Other>& value) const noexcept { return (v0 == value.v0) && (v1 == value.v1); }
            template<typename Other>
            constexpr bool operator!=(const box<size, Other>& value) const noexcept { return !this->operator==(value); }

            constexpr vector_type& get(const vector_size_type index) JUTILS_VECTOR_THROW_FUNCTION
            {
#ifndef JUTILS_VECTOR_DISABLE_THROW_ERRORS
                _checkIndex(index);
#endif
                return index <= 0 ? v0 : v1;
            }
            constexpr const vector_type& get(const vector_size_type index) const JUTILS_VECTOR_THROW_FUNCTION
            {
#ifndef JUTILS_VECTOR_DISABLE_THROW_ERRORS
                _checkIndex(index);
#endif
                return index <= 0 ? v0 : v1;
            }
            constexpr vector_type& operator[](const vector_size_type index) JUTILS_VECTOR_THROW_FUNCTION { return get(index); }
            constexpr const vector_type& operator[](const vector_size_type index) const JUTILS_VECTOR_THROW_FUNCTION { return get(index); }

            constexpr type* getData() noexcept { return v0.getData(); }
            constexpr const type* getData() const noexcept { return v0.getData(); }

            template<typename R = type>
            constexpr box<size, R> copy() const noexcept { return *this; }
            constexpr box abs() const noexcept { return { jutils::math::abs(v0), jutils::math::abs(v1) }; }

            constexpr box& operator++() noexcept { ++v0; ++v1; return *this; }
            constexpr box& operator--() noexcept { --v0; --v1; return *this; }
            constexpr box operator++(int) noexcept { box temp = copy(); this->operator++(); return temp; }
            constexpr box operator--(int) noexcept { box temp = copy(); this->operator--(); return temp; }
            constexpr box operator-() const noexcept { return { -v0, -v1 }; }

            template<typename Other> requires std::is_arithmetic_v<Other>
            constexpr box& operator+=(const Other value) noexcept
            {
                v0 += value; v1 += value;
                return *this;
            }
            template<typename Other>
            constexpr box& operator+=(const vector<size, Other>& value) noexcept
            {
                v0 += value; v1 += value;
                return *this;
            }
            template<typename Other> requires std::is_arithmetic_v<Other>
            constexpr box& operator-=(const Other value) noexcept
            {
                v0 -= value; v1 -= value;
                return *this;
            }
            template<typename Other>
            constexpr box& operator-=(const vector<size, Other>& value) noexcept
            {
                v0 -= value; v1 -= value;
                return *this;
            }
            template<typename Other> requires std::is_arithmetic_v<Other>
            constexpr box& operator*=(const Other value) noexcept
            {
                v0 *= value; v1 *= value;
                return *this;
            }
            template<typename Other>
            constexpr box& operator*=(const vector<size, Other>& value) noexcept
            {
                v0 *= value; v1 *= value;
                return *this;
            }
            template<typename Other> requires std::is_arithmetic_v<Other>
            constexpr box& operator/=(const Other value) noexcept
            {
                v0 /= value; v1 /= value;
                return *this;
            }
            template<typename Other>
            constexpr box& operator/=(const vector<size, Other>& value) noexcept
            {
                v0 /= value; v1 /= value;
                return *this;
            }

            jstring toString() const noexcept { return JSTR_FORMAT("{{ {}; {} }}", v0, v1); }

#ifndef JUTILS_VECTOR_DISABLE_THROW_ERRORS
        private:
            static constexpr void _checkIndex(const vector_size_type index)
            {
                if ((index < 0) || (index >= size))
                {
                    throw std::out_of_range("Invalid index");
                }
            }
#endif
        };

        template<typename T1, typename T2> requires std::is_arithmetic_v<T1>
        constexpr box<2, T1> operator/(const T1& value1, const box<2, T2>& value2) noexcept { return { value1 / value2.v0, value1 / value2.v1 }; }
        template<typename T1, typename T2>
        constexpr box<2, T1> operator/(const vector<2, T1>& value1, const box<2, T2>& value2) noexcept { return { value1 / value2.v0, value1 / value2.v1 }; }
    }
}
