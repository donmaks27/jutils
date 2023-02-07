// Copyright © 2022-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vector3_base.h"

namespace jutils
{
    namespace math
    {
        template<typename Type>
        class vector<4, Type>
        {
        public:

            static constexpr vector_size_type size = 4;

            using type = Type;

            constexpr vector() = default;
            constexpr vector(const type x, const type y, const type z, const type w)
                : x(x), y(y), z(z), w(w)
            {}
            explicit constexpr vector(const type value)
                : x(value), y(value), z(value), w(value)
            {}
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector(const vector<size, OtherType>& value)
                : x(static_cast<type>(value.x)), y(static_cast<type>(value.y)), z(static_cast<type>(value.z))
                , w(static_cast<type>(value.w))
            {}
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            explicit constexpr vector(const vector<3, OtherType>& value)
                : x(static_cast<type>(value.x)), y(static_cast<type>(value.y)), z(static_cast<type>(value.z))
                , w(0)
            {}
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            explicit constexpr vector(const vector<2, OtherType>& value)
                : x(static_cast<type>(value.x)), y(static_cast<type>(value.y)), z(0), w(0)
            {}

            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator=(const vector<size, OtherType>& value)
            {
                x = static_cast<type>(value.x); y = static_cast<type>(value.y); z = static_cast<type>(value.z);
                w = static_cast<type>(value.w);
                return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator=(const vector<3, OtherType>& value)
            {
                x = static_cast<type>(value.x); y = static_cast<type>(value.y); z = static_cast<type>(value.z);
                w = 0;
                return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator=(const vector<2, OtherType>& value)
            {
                x = static_cast<type>(value.x); y = static_cast<type>(value.y); z = 0; w = 0;
                return *this;
            }

            type x = 0;
            type y = 0;
            type z = 0;
            type w = 0;

            constexpr type& get(const vector_size_type index)
            {
                assert((index >= 0) && (index < size));
                switch (index)
                {
                case 0: return x;
                case 1: return y;
                case 2: return z;
                default: ;
                }
                return w;
            }
            constexpr const type& get(const vector_size_type index) const
            {
                assert((index >= 0) && (index < size));
                switch (index)
                {
                case 0: return x;
                case 1: return y;
                case 2: return z;
                default: ;
                }
                return w;
            }
            constexpr type& operator[](const vector_size_type index) { return get(index); }
            constexpr const type& operator[](const vector_size_type index) const { return get(index); }
            
            constexpr type* getData() { return &x; }
            constexpr const type* getData() const { return &x; }

            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr bool operator==(const vector<size, OtherType>& value) const
            {
                return (x == static_cast<type>(value.x)) && (y == static_cast<type>(value.y)) && (z == static_cast<type>(value.z)) && (w == static_cast<type>(value.w));
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr bool operator!=(const vector<size, OtherType>& value) const { return !this->operator==(value); }

            jstring toString() const { return jstring::format("{{ {}; {}; {}; {} }}", x, y, z, w); }
            
            constexpr vector& operator++() { ++x; ++y; ++z; ++w; return *this; }
            constexpr vector& operator--() { --x; --y; --z; --w; return *this; }
            constexpr vector operator++(int) { const vector temp = *this; this->operator++(); return temp; }
            constexpr vector operator--(int) { const vector temp = *this; this->operator--(); return temp; }
            constexpr vector operator-() const { return { -x, -y, -z, -w }; }
            
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator+=(const OtherType value)
            {
                x += value; y += value; z += value; w += value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator+=(const vector<size, OtherType>& value)
            {
                x += value.x; y += value.y; z += value.z; w += value.w; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator-=(const OtherType value)
            {
                x -= value; y -= value; z -= value; w -= value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator-=(const vector<size, OtherType>& value)
            {
                x -= value.x; y -= value.y; z -= value.z; w -= value.w; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator*=(const OtherType value)
            {
                x *= value; y *= value; z *= value; w *= value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator*=(const vector<size, OtherType>& value)
            {
                x *= value.x; y *= value.y; z *= value.z; w *= value.w; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator/=(const OtherType value)
            {
                x /= value; y /= value; z /= value; w /= value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr vector& operator/=(const vector<size, OtherType>& value)
            {
                x /= value.x; y /= value.y; z /= value.z; w /= value.w; return *this;
            }
        };

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<4, Type1> operator+(const vector<4, Type1>& value1, const Type2 value2)
        {
            return vector<4, Type1>(value1) += value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<4, Type1> operator+(const Type1 value1, const vector<4, Type2>& value2)
        {
            return vector<4, Type1>(value1) += value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<4, Type1> operator+(const vector<4, Type1>& value1, const vector<4, Type2>& value2)
        {
            return vector<4, Type1>(value1) += value2;
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<4, Type1> operator-(const vector<4, Type1>& value1, const Type2 value2)
        {
            return vector<4, Type1>(value1) -= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<4, Type1> operator-(const Type1 value1, const vector<4, Type2>& value2)
        {
            return vector<4, Type1>(value1) -= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<4, Type1> operator-(const vector<4, Type1>& value1, const vector<4, Type2>& value2)
        {
            return vector<4, Type1>(value1) -= value2;
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<4, Type1> operator*(const vector<4, Type1>& value1, const Type2 value2)
        {
            return vector<4, Type1>(value1) *= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<4, Type1> operator*(const Type1 value1, const vector<4, Type2>& value2)
        {
            return vector<4, Type1>(value1) *= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<4, Type1> operator*(const vector<4, Type1>& value1, const vector<4, Type2>& value2)
        {
            return vector<4, Type1>(value1) *= value2;
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<4, Type1> operator/(const vector<4, Type1>& value1, const Type2 value2)
        {
            return vector<4, Type1>(value1) /= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<4, Type1> operator/(const Type1 value1, const vector<4, Type2>& value2)
        {
            return vector<4, Type1>(value1) /= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr vector<4, Type1> operator/(const vector<4, Type1>& value1, const vector<4, Type2>& value2)
        {
            return vector<4, Type1>(value1) /= value2;
        }
    }
}
