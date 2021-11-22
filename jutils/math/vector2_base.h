﻿// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vector_base.h"
#include "../type_checks.h"

#include <cassert>

namespace jutils
{
    namespace math
    {
        template<typename Type>
        class vector_base<2, Type>
        {
        public:

            using type = Type;

            static constexpr vector_size_type size = 2;

            constexpr vector_base() = default;
            constexpr vector_base(const type x, const type y)
                : x(x), y(y)
            {}
            explicit constexpr vector_base(const type value)
                : x(value), y(value)
            {}
            template<vector_size_type Size, typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type> && (Size >= 2))>
            constexpr vector_base(const vector_base<Size, OtherType>& value)
                : x(static_cast<type>(value.x)), y(static_cast<type>(value.y))
            {}

            template<vector_size_type Size, typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type> && (Size >= 2))>
            constexpr vector_base& operator=(const vector_base<Size, OtherType>& value)
            {
                if (this != &value)
                {
                    x = static_cast<type>(value.x); y = static_cast<type>(value.y);
                }
                return *this;
            }

            type x = 0;
            type y = 0;

            constexpr vector_size_type getSize() const { return this->size; }
            constexpr type& get(const vector_size_type index)
            {
                assert((index >= 0) && (index < getSize()));
                return index == 0 ? x : y;
            }
            constexpr const type& get(const vector_size_type index) const
            {
                assert((index >= 0) && (index < getSize()));
                return index == 0 ? x : y;
            }
            constexpr type& operator[](const vector_size_type index) { return get(index); }
            constexpr const type& operator[](const vector_size_type index) const { return get(index); }

            constexpr type* getData() { return &get(0); }
            constexpr const type* getData() const { return &get(0); }

            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator+=(OtherType value);
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator+=(const vector_base<2, OtherType>& value);
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator-=(OtherType value);
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator-=(const vector_base<2, OtherType>& value);
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator*=(OtherType value);
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator*=(const vector_base<2, OtherType>& value);
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator/=(OtherType value);
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator/=(const vector_base<2, OtherType>& value);

            constexpr vector_base& operator++();
            constexpr vector_base& operator--();
            constexpr vector_base operator++(int);
            constexpr vector_base operator--(int);

            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr bool operator==(const vector_base<2, OtherType>& value) const
            {
                return (x == static_cast<type>(value.x)) && (y == static_cast<type>(value.y));
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr bool operator!=(const vector_base<2, OtherType>& value) const { return !this->operator==(value); }
        };

        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<2, Type>& vector_base<2, Type>::operator+=(const OtherType value) { x += value; y += value; return *this; }
        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<2, Type>& vector_base<2, Type>::operator+=(const vector_base<2, OtherType>& value) { x += static_cast<type>(value.x); y += static_cast<type>(value.y); return *this; }
        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<2, Type>& vector_base<2, Type>::operator-=(const OtherType value) { x -= value; y -= value; return *this; }
        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<2, Type>& vector_base<2, Type>::operator-=(const vector_base<2, OtherType>& value) { x -= static_cast<type>(value.x); y -= static_cast<type>(value.y); return *this; }
        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<2, Type>& vector_base<2, Type>::operator*=(const OtherType value) { x *= value; y *= value; return *this; }
        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<2, Type>& vector_base<2, Type>::operator*=(const vector_base<2, OtherType>& value) { x *= static_cast<type>(value.x); y *= static_cast<type>(value.y); return *this; }
        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<2, Type>& vector_base<2, Type>::operator/=(const OtherType value) { x /= value; y /= value; return *this; }
        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<2, Type>& vector_base<2, Type>::operator/=(const vector_base<2, OtherType>& value) { x /= static_cast<type>(value.x); y /= static_cast<type>(value.y); return *this; }

        template <typename Type>
        constexpr vector_base<2, Type>& vector_base<2, Type>::operator++() { ++x; ++y; return *this; }
        template <typename Type>
        constexpr vector_base<2, Type>& vector_base<2, Type>::operator--() { --x; --y; return *this; }
        template <typename Type>
        constexpr vector_base<2, Type> vector_base<2, Type>::operator++(int) { vector_base temp = *this; ++x; ++y; return temp; }
        template <typename Type>
        constexpr vector_base<2, Type> vector_base<2, Type>::operator--(int) { vector_base temp = *this; --x; --y; return temp; }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<2, Type1> operator+(const vector_base<2, Type1>& value1, const Type2 value2) { return { value1.x + static_cast<Type1>(value2), value1.y + static_cast<Type1>(value2) }; }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<2, Type1> operator+(const Type1 value1, const vector_base<2, Type2>& value2) { return { value1 + static_cast<Type1>(value2.x), value1 + static_cast<Type1>(value2.y) }; }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<2, Type1> operator+(const vector_base<2, Type1>& value1, const vector_base<2, Type2>& value2) { return { value1.x + static_cast<Type1>(value2.x), value1.y + static_cast<Type1>(value2.y) }; }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<2, Type1> operator-(const vector_base<2, Type1>& value1, const Type2 value2) { return { value1.x - static_cast<Type1>(value2), value1.y - static_cast<Type1>(value2) }; }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<2, Type1> operator-(const Type1 value1, const vector_base<2, Type2>& value2) { return { value1 - static_cast<Type1>(value2.x), value1 - static_cast<Type1>(value2.y) }; }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<2, Type1> operator-(const vector_base<2, Type1>& value1, const vector_base<2, Type2>& value2) { return { value1.x - static_cast<Type1>(value2.x), value1.y - static_cast<Type1>(value2.y) }; }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<2, Type1> operator*(const vector_base<2, Type1>& value1, const Type2 value2) { return { value1.x * static_cast<Type1>(value2), value1.y * static_cast<Type1>(value2) }; }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<2, Type1> operator*(const Type1 value1, const vector_base<2, Type2>& value2) { return { value1 * static_cast<Type1>(value2.x), value1 * static_cast<Type1>(value2.y) }; }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<2, Type1> operator*(const vector_base<2, Type1>& value1, const vector_base<2, Type2>& value2) { return { value1.x * static_cast<Type1>(value2.x), value1.y * static_cast<Type1>(value2.y) }; }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<2, Type1> operator/(const vector_base<2, Type1>& value1, const Type2 value2) { return { value1.x / static_cast<Type1>(value2), value1.y / static_cast<Type1>(value2) }; }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<2, Type1> operator/(const Type1 value1, const vector_base<2, Type2>& value2) { return { value1 / static_cast<Type1>(value2.x), value1 / static_cast<Type1>(value2.y) }; }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<2, Type1> operator/(const vector_base<2, Type1>& value1, const vector_base<2, Type2>& value2) { return { value1.x / static_cast<Type1>(value2.x), value1.y / static_cast<Type1>(value2.y) }; }
    }
}
