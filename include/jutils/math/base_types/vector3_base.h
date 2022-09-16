// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vector2_base.h"

namespace jutils
{
    namespace math
    {
        template<typename Type>
        class vector<3, Type>
        {
        public:

            static constexpr vector_size_type size = 3;

            using type = Type;

            constexpr vector() = default;
            constexpr vector(const type x, const type y, const type z)
                : x(x), y(y), z(z)
            {}
            explicit constexpr vector(const type value)
                : x(value), y(value), z(value)
            {}
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector(const vector<size, OtherType>& value)
                : x(static_cast<type>(value.x)), y(static_cast<type>(value.y)), z(static_cast<type>(value.z))
            {}
            template<vector_size_type Size, typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type> && (Size > size))>
            explicit constexpr vector(const vector<Size, OtherType>& value)
                : x(static_cast<type>(value.x)), y(static_cast<type>(value.y)), z(static_cast<type>(value.z))
            {}
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            explicit constexpr vector(const vector<2, OtherType>& value)
                : x(static_cast<type>(value.x)), y(static_cast<type>(value.y)), z(0)
            {}

            template<vector_size_type Size, typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type> && (Size >= size))>
            constexpr vector& operator=(const vector<Size, OtherType>& value)
            {
                x = static_cast<type>(value.x); y = static_cast<type>(value.y); z = static_cast<type>(value.z);
                return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator=(const vector<2, OtherType>& value)
            {
                x = static_cast<type>(value.x); y = static_cast<type>(value.y); z = 0;
                return *this;
            }

            type x = 0;
            type y = 0;
            type z = 0;

            constexpr type& get(const vector_size_type index)
            {
                assert((index >= 0) && (index < size));
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
                assert((index >= 0) && (index < size));
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

            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr bool operator==(const vector<size, OtherType>& value) const
            {
                return (x == static_cast<type>(value.x)) && (y == static_cast<type>(value.y)) && (z == static_cast<type>(value.z));
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr bool operator!=(const vector<size, OtherType>& value) const { return !this->operator==(value); }

            jstring toString() const { return jstring(fmt::format("{{ {}; {}; {} }}", x, y, z)); }

            constexpr vector& operator++() { ++x; ++y; ++z; return *this; }
            constexpr vector& operator--() { --x; --y; --z; return *this; }
            constexpr vector operator++(int) { const vector temp = *this; this->operator++(); return temp; }
            constexpr vector operator--(int) { const vector temp = *this; this->operator--(); return temp; }
            
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator+=(const OtherType value)
            {
                x += value; y += value; z += value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator+=(const vector<size, OtherType>& value)
            {
                x += value.x; y += value.y; z += value.z; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator-=(const OtherType value)
            {
                x -= value; y -= value; z -= value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator-=(const vector<size, OtherType>& value)
            {
                x -= value.x; y -= value.y; z -= value.z; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator*=(const OtherType value)
            {
                x *= value; y *= value; z *= value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator*=(const vector<size, OtherType>& value)
            {
                x *= value.x; y *= value.y; z *= value.z; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator/=(const OtherType value)
            {
                x /= value; y /= value; z /= value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator/=(const vector<size, OtherType>& value)
            {
                x /= value.x; y /= value.y; z /= value.z; return *this;
            }
        };

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<3, Type1> operator+(const vector<3, Type1>& value1, const Type2 value2)
        {
            return vector<3, Type1>(value1) += value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<3, Type1> operator+(const Type1 value1, const vector<3, Type2>& value2)
        {
            return vector<3, Type1>(value2) += value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<3, Type1> operator+(const vector<3, Type1>& value1, const vector<3, Type2>& value2)
        {
            return vector<3, Type1>(value1) += value2;
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<3, Type1> operator-(const vector<3, Type1>& value1, const Type2 value2)
        {
            return vector<3, Type1>(value1) -= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<3, Type1> operator-(const Type1 value1, const vector<3, Type2>& value2)
        {
            return vector<3, Type1>(value2) -= value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<3, Type1> operator-(const vector<3, Type1>& value1, const vector<3, Type2>& value2)
        {
            return vector<3, Type1>(value1) -= value2;
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<3, Type1> operator*(const vector<3, Type1>& value1, const Type2 value2)
        {
            return vector<3, Type1>(value1) *= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<3, Type1> operator*(const Type1 value1, const vector<3, Type2>& value2)
        {
            return vector<3, Type1>(value2) *= value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<3, Type1> operator*(const vector<3, Type1>& value1, const vector<3, Type2>& value2)
        {
            return vector<3, Type1>(value1) *= value2;
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<3, Type1> operator/(const vector<3, Type1>& value1, const Type2 value2)
        {
            return vector<3, Type1>(value1) /= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<3, Type1> operator/(const Type1 value1, const vector<3, Type2>& value2)
        {
            return vector<3, Type1>(value2) /= value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<3, Type1> operator/(const vector<3, Type1>& value1, const vector<3, Type2>& value2)
        {
            return vector<3, Type1>(value1) /= value2;
        }
    }
}
