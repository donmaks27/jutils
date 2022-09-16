// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vector_base.h"
#include "../../type_checks.h"
#include "../../jstring.h"

#include <cassert>
#include <fmt/core.h>

namespace jutils
{
    namespace math
    {
        template<typename Type>
        class vector<2, Type>
        {
        public:

            static constexpr vector_size_type size = 2;

            using type = Type;

            constexpr vector() = default;
            constexpr vector(const type x, const type y)
                : x(x), y(y)
            {}
            explicit constexpr vector(const type value)
                : x(value), y(value)
            {}
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector(const vector<size, OtherType>& value)
                : x(static_cast<type>(value.x)), y(static_cast<type>(value.y))
            {}
            template<vector_size_type Size, typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type> && (Size > size))>
            explicit constexpr vector(const vector<Size, OtherType>& value)
                : x(static_cast<type>(value.x)), y(static_cast<type>(value.y))
            {}

            template<vector_size_type Size, typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type> && (Size >= size))>
            constexpr vector& operator=(const vector<Size, OtherType>& value)
            {
                x = static_cast<type>(value.x); y = static_cast<type>(value.y);
                return *this;
            }

            type x = 0;
            type y = 0;

            constexpr type& get(const vector_size_type index)
            {
                assert((index >= 0) && (index < size));
                return index == 0 ? x : y;
            }
            constexpr const type& get(const vector_size_type index) const
            {
                assert((index >= 0) && (index < size));
                return index == 0 ? x : y;
            }
            constexpr type& operator[](const vector_size_type index) { return get(index); }
            constexpr const type& operator[](const vector_size_type index) const { return get(index); }

            constexpr type* getData() { return &x; }
            constexpr const type* getData() const { return &x; }

            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr bool operator==(const vector<size, OtherType>& value) const
            {
                return (x == static_cast<type>(value.x)) && (y == static_cast<type>(value.y));
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr bool operator!=(const vector<size, OtherType>& value) const
            {
                return !this->operator==(value);
            }

            jstring toString() const { return jstring(fmt::format("{{ {}; {} }}", x, y)); }

            constexpr vector& operator++() { ++x; ++y; return *this; }
            constexpr vector& operator--() { --x; --y; return *this; }
            constexpr vector operator++(int) { const vector temp = *this; this->operator++(); return temp; }
            constexpr vector operator--(int) { const vector temp = *this; this->operator--(); return temp; }

            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator+=(const OtherType value)
            {
                x += value; y += value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator+=(const vector<size, OtherType>& value)
            {
                x += value.x; y += value.y; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator-=(const OtherType value)
            {
                x -= value; y -= value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator-=(const vector<size, OtherType>& value)
            {
                x -= value.x; y -= value.y; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator*=(const OtherType value)
            {
                x *= value; y *= value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator*=(const vector<size, OtherType>& value)
            {
                x *= value.x; y *= value.y; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator/=(const OtherType value)
            {
                x /= value; y /= value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator/=(const vector<size, OtherType>& value)
            {
                x /= value.x; y /= value.y; return *this;
            }
        };

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<2, Type1> operator+(const vector<2, Type1>& value1, const Type2 value2)
        {
            return vector<2, Type1>(value1) += value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<2, Type1> operator+(const Type1 value1, const vector<2, Type2>& value2)
        {
            return vector<2, Type1>(value2) += value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<2, Type1> operator+(const vector<2, Type1>& value1, const vector<2, Type2>& value2)
        {
            return vector<2, Type1>(value1) += value2;
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<2, Type1> operator-(const vector<2, Type1>& value1, const Type2 value2)
        {
            return vector<2, Type1>(value1) -= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<2, Type1> operator-(const Type1 value1, const vector<2, Type2>& value2)
        {
            return vector<2, Type1>(value2) -= value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<2, Type1> operator-(const vector<2, Type1>& value1, const vector<2, Type2>& value2)
        {
            return vector<2, Type1>(value1) -= value2;
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<2, Type1> operator*(const vector<2, Type1>& value1, const Type2 value2)
        {
            return vector<2, Type1>(value1) *= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<2, Type1> operator*(const Type1 value1, const vector<2, Type2>& value2)
        {
            return vector<2, Type1>(value2) *= value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<2, Type1> operator*(const vector<2, Type1>& value1, const vector<2, Type2>& value2)
        {
            return vector<2, Type1>(value1) *= value2;
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<2, Type1> operator/(const vector<2, Type1>& value1, const Type2 value2)
        {
            return vector<2, Type1>(value1) /= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<2, Type1> operator/(const Type1 value1, const vector<2, Type2>& value2)
        {
            return vector<2, Type1>(value2) /= value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<2, Type1> operator/(const vector<2, Type1>& value1, const vector<2, Type2>& value2)
        {
            return vector<2, Type1>(value1) /= value2;
        }
    }
}
