﻿// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vector2_base.h"

namespace jutils
{
    namespace math
    {
        template<typename Type>
        class vector_base<3, Type>
        {
        public:

            using type = Type;

            static constexpr vector_size_type size = 3;

            constexpr vector_base() = default;
            constexpr vector_base(const type x, const type y, const type z)
                : x(x), y(y), z(z)
            {}
            explicit constexpr vector_base(const type value)
                : x(value), y(value), z(value)
            {}
            template<vector_size_type Size, typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type> && (Size >= 3))>
            constexpr vector_base(const vector_base<Size, OtherType>& value)
                : x(static_cast<type>(value.x)), y(static_cast<type>(value.y)), z(static_cast<type>(value.z))
            {}
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base(const vector_base<2, OtherType>& value)
                : x(static_cast<type>(value.x)), y(static_cast<type>(value.y)), z(0)
            {}

            template<vector_size_type Size, typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type> && (Size >= 3))>
            constexpr vector_base& operator=(const vector_base<Size, OtherType>& value)
            {
                if (this != &value)
                {
                    x = static_cast<type>(value.x); y = static_cast<type>(value.y); z = static_cast<type>(value.z);
                }
                return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator=(const vector_base<2, OtherType>& value)
            {
                if (this != &value)
                {
                    x = static_cast<type>(value.x); y = static_cast<type>(value.y); z = 0;
                }
                return *this;
            }

            type x = 0;
            type y = 0;
            type z = 0;

            constexpr vector_base<2, type> xy() const { return { x, y }; }

            constexpr vector_size_type getSize() const { return this->size; }
            constexpr type& get(const vector_size_type index)
            {
                assert((index >= 0) && (index < getSize()));
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
                assert((index >= 0) && (index < getSize()));
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
            
            constexpr type* getData() { return &get(0); }
            constexpr const type* getData() const { return &get(0); }

            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator+=(OtherType value);
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator+=(const vector_base<3, OtherType>& value);
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator-=(OtherType value);
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator-=(const vector_base<3, OtherType>& value);
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator*=(OtherType value);
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator*=(const vector_base<3, OtherType>& value);
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator/=(OtherType value);
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector_base& operator/=(const vector_base<3, OtherType>& value);

            constexpr vector_base& operator++();
            constexpr vector_base& operator--();
            constexpr vector_base operator++(int);
            constexpr vector_base operator--(int);

            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr bool operator==(const vector_base<3, OtherType>& value) const
            {
                return (x == static_cast<type>(value.x)) && (y == static_cast<type>(value.y)) && (z == static_cast<type>(value.z));
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr bool operator!=(const vector_base<3, OtherType>& value) const { return !this->operator==(value); }
        };

        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<3, Type>& vector_base<3, Type>::operator+=(OtherType value)
        {
            x += static_cast<Type>(value); y += static_cast<Type>(value); z += static_cast<Type>(value);
            return *this;
        }
        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<3, Type>& vector_base<3, Type>::operator+=(const vector_base<3, OtherType>& value)
        {
            x += static_cast<Type>(value.x); y += static_cast<Type>(value.y); z += static_cast<Type>(value.z);
            return *this;
        }
        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<3, Type>& vector_base<3, Type>::operator-=(OtherType value)
        {
            x -= static_cast<Type>(value); y -= static_cast<Type>(value); z -= static_cast<Type>(value);
            return *this;
        }
        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<3, Type>& vector_base<3, Type>::operator-=(const vector_base<3, OtherType>& value)
        {
            x -= static_cast<Type>(value.x); y -= static_cast<Type>(value.y); z -= static_cast<Type>(value.z);
            return *this;
        }
        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<3, Type>& vector_base<3, Type>::operator*=(OtherType value)
        {
            x *= static_cast<Type>(value); y *= static_cast<Type>(value); z *= static_cast<Type>(value);
            return *this;
        }
        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<3, Type>& vector_base<3, Type>::operator*=(const vector_base<3, OtherType>& value)
        {
            x *= static_cast<Type>(value.x); y *= static_cast<Type>(value.y); z *= static_cast<Type>(value.z);
            return *this;
        }
        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<3, Type>& vector_base<3, Type>::operator/=(OtherType value)
        {
            x /= static_cast<Type>(value); y /= static_cast<Type>(value); z /= static_cast<Type>(value);
            return *this;
        }
        template <typename Type>
        template <typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr vector_base<3, Type>& vector_base<3, Type>::operator/=(const vector_base<3, OtherType>& value)
        {
            x /= static_cast<Type>(value.x); y /= static_cast<Type>(value.y); z /= static_cast<Type>(value.z);
            return *this;
        }

        template <typename Type>
        constexpr vector_base<3, Type>& vector_base<3, Type>::operator++()
        {
            ++x; ++y; ++z;
            return *this;
        }
        template <typename Type>
        constexpr vector_base<3, Type>& vector_base<3, Type>::operator--()
        {
            --x; --y; --z;
            return *this;
        }
        template <typename Type>
        constexpr vector_base<3, Type> vector_base<3, Type>::operator++(int)
        {
            vector_base<3, Type> temp = *this;
            ++x; ++y; ++z;
            return temp;
        }
        template <typename Type>
        constexpr vector_base<3, Type> vector_base<3, Type>::operator--(int)
        {
            vector_base<3, Type> temp = *this;
            --x; --y; --z;
            return temp;
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<3, Type1> operator+(const vector_base<3, Type1>& value1, const Type2 value2)
        {
            return { value1.x + static_cast<Type1>(value2), value1.y + static_cast<Type1>(value2), value1.z + static_cast<Type1>(value2) };
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<3, Type1> operator+(const Type1 value1, const vector_base<3, Type2>& value2)
        {
            return { value1 + static_cast<Type1>(value2.x), value1 + static_cast<Type1>(value2.y), value1 + static_cast<Type1>(value2.z) };
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<3, Type1> operator+(const vector_base<3, Type1>& value1, const vector_base<3, Type2>& value2)
        {
            return { value1.x + static_cast<Type1>(value2.x), value1.y + static_cast<Type1>(value2.y), value1.z + static_cast<Type1>(value2.z) };
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<3, Type1> operator-(const vector_base<3, Type1>& value1, const Type2 value2)
        {
            return { value1.x - static_cast<Type1>(value2), value1.y - static_cast<Type1>(value2), value1.z - static_cast<Type1>(value2) };
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<3, Type1> operator-(const Type1 value1, const vector_base<3, Type2>& value2)
        {
            return { value1 - static_cast<Type1>(value2.x), value1 - static_cast<Type1>(value2.y), value1 - static_cast<Type1>(value2.z) };
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<3, Type1> operator-(const vector_base<3, Type1>& value1, const vector_base<3, Type2>& value2)
        {
            return { value1.x - static_cast<Type1>(value2.x), value1.y - static_cast<Type1>(value2.y), value1.z - static_cast<Type1>(value2.z) };
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<3, Type1> operator*(const vector_base<3, Type1>& value1, const Type2 value2)
        {
            return { value1.x * static_cast<Type1>(value2), value1.y * static_cast<Type1>(value2), value1.z * static_cast<Type1>(value2) };
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<3, Type1> operator*(const Type1 value1, const vector_base<3, Type2>& value2)
        {
            return { value1 * static_cast<Type1>(value2.x), value1 * static_cast<Type1>(value2.y), value1 * static_cast<Type1>(value2.z) };
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<3, Type1> operator*(const vector_base<3, Type1>& value1, const vector_base<3, Type2>& value2)
        {
            return { value1.x * static_cast<Type1>(value2.x), value1.y * static_cast<Type1>(value2.y), value1.z * static_cast<Type1>(value2.z) };
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<3, Type1> operator/(const vector_base<3, Type1>& value1, const Type2 value2)
        {
            return { value1.x / static_cast<Type1>(value2), value1.y / static_cast<Type1>(value2), value1.z / static_cast<Type1>(value2) };
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<3, Type1> operator/(const Type1 value1, const vector_base<3, Type2>& value2)
        {
            return { value1 / static_cast<Type1>(value2.x), value1 / static_cast<Type1>(value2.y), value1 / static_cast<Type1>(value2.z) };
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector_base<3, Type1> operator/(const vector_base<3, Type1>& value1, const vector_base<3, Type2>& value2)
        {
            return { value1.x / static_cast<Type1>(value2.x), value1.y / static_cast<Type1>(value2.y), value1.z / static_cast<Type1>(value2.z) };
        }
    }
}
