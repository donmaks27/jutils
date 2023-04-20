// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "box2_base.h"
#include "vector3_base.h"

namespace jutils
{
    namespace math
    {
        template<typename Type>
        class box<3, Type>
        {
        public:

            static constexpr vector_size_type size = 3;

            using type = Type;
            using vector_type = vector<size, type>;

            constexpr box() = default;
            constexpr box(const vector_type& v0, const vector_type& v1, const vector_type& v2)
                : v0(v0), v1(v1), v2(v2)
            {}
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr box(const box<size, OtherType>& value)
                : v0(value.v0), v1(value.v1), v2(value.v2)
            {}
            template<vector_size_type Size, typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type> && (Size > size))>
            explicit constexpr box(const box<Size, OtherType>& value)
                : v0(value.v0), v1(value.v1), v2(value.v2)
            {}
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            explicit constexpr box(const box<2, OtherType>& value)
                : v0(value.v0), v1(value.v1), v2(0)
            {}

            template<vector_size_type Size, typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type> && (Size >= size))>
            constexpr box& operator=(const box<Size, OtherType>& value)
            {
                v0 = value.v0; v1 = value.v1; v2 = value.v2;
                return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr box& operator=(const box<2, OtherType>& value)
            {
                v0 = value.v0; v1 = value.v1; v2 = 0;
                return *this;
            }

            vector_type v0;
            vector_type v1;
            vector_type v2;

            constexpr vector_type& get(const vector_size_type index)
            {
                assert((index >= 0) && (index < size));
                switch (index)
                {
                case 0: return v0;
                case 1: return v1;
                default: ;
                }
                return v2;
            }
            constexpr const vector_type& get(const vector_size_type index) const
            {
                assert((index >= 0) && (index < size));
                switch (index)
                {
                case 0: return v0;
                case 1: return v1;
                default: ;
                }
                return v2;
            }
            constexpr vector_type& operator[](const vector_size_type index) { return get(index); }
            constexpr const vector_type& operator[](const vector_size_type index) const { return get(index); }

            constexpr type* getData() { return v0.getData(); }
            constexpr const type* getData() const { return v0.getData(); }

            template<typename OtherType>
            constexpr bool operator==(const box<size, OtherType>& value) const
            {
                return (v0 == value.v0) && (v1 == value.v2) && (v2 == value.v2);
            }
            template<typename OtherType>
            constexpr bool operator!=(const box<size, OtherType>& value) const
            {
                return !this->operator==(value);
            }

            jstring toString() const { return JSTR_FORMAT("{{ {}; {}; {} }}", v0, v1, v2); }

            constexpr box& operator++() { ++v0; ++v1; ++v2; return *this; }
            constexpr box& operator--() { --v0; --v1; --v2; return *this; }
            constexpr box operator++(int) { const box temp = *this; this->operator++(); return temp; }
            constexpr box operator--(int) { const box temp = *this; this->operator--(); return temp; }

            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr box& operator+=(const OtherType value)
            {
                v0 += value; v1 += value; v2 += value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr box& operator+=(const vector<size, OtherType>& value)
            {
                v0 += value; v1 += value; v2 += value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr box& operator+=(const box<size, OtherType>& value)
            {
                v0 += value.v0; v1 += value.v1; v2 += value.v2; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr box& operator-=(const OtherType value)
            {
                v0 -= value; v1 -= value; v2 -= value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr box& operator-=(const vector<size, OtherType>& value)
            {
                v0 -= value; v1 -= value; v2 -= value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr box& operator-=(const box<size, OtherType>& value)
            {
                v0 -= value.v0; v1 -= value.v1; v2 -= value.v2; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr box& operator*=(const OtherType value)
            {
                v0 *= value; v1 *= value; v2 *= value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr box& operator*=(const vector<size, OtherType>& value)
            {
                v0 *= value; v1 *= value; v2 *= value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr box& operator*=(const box<size, OtherType>& value)
            {
                v0 *= value.v0; v1 *= value.v1; v2 *= value.v2; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr box& operator/=(const OtherType value)
            {
                v0 /= value; v1 /= value; v2 /= value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr box& operator/=(const vector<size, OtherType>& value)
            {
                v0 /= value; v1 /= value; v2 /= value; return *this;
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr box& operator/=(const box<size, OtherType>& value)
            {
                v0 /= value.v0; v1 /= value.v1; v2 /= value.v2; return *this;
            }
        };

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator+(const box<3, Type1>& value1, const Type2 value2)
        {
            return box<3, Type1>(value1) += value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator+(const Type1 value1, const box<3, Type2>& value2)
        {
            return box<3, Type1>(value2) += value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator+(const box<3, Type1>& value1, const vector<3, Type2>& value2)
        {
            return box<3, Type1>(value1) += value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator+(const vector<3, Type1>& value1, const box<3, Type2>& value2)
        {
            return box<3, Type1>(value2) += value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator+(const box<3, Type1>& value1, const box<3, Type2>& value2)
        {
            return box<3, Type1>(value1) += value2;
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator-(const box<3, Type1>& value1, const Type2 value2)
        {
            return box<3, Type1>(value1) -= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator-(const Type1 value1, const box<3, Type2>& value2)
        {
            return box<3, Type1>(value2) -= value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator-(const box<3, Type1>& value1, const vector<3, Type2>& value2)
        {
            return box<3, Type1>(value1) -= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator-(const vector<3, Type1>& value1, const box<3, Type2>& value2)
        {
            return box<3, Type1>(value2) -= value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator-(const box<3, Type1>& value1, const box<3, Type2>& value2)
        {
            return box<3, Type1>(value1) -= value2;
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator*(const box<3, Type1>& value1, const Type2 value2)
        {
            return box<3, Type1>(value1) *= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator*(const Type1 value1, const box<3, Type2>& value2)
        {
            return box<3, Type1>(value2) *= value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator*(const box<3, Type1>& value1, const vector<3, Type2>& value2)
        {
            return box<3, Type1>(value1) *= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator*(const vector<3, Type1>& value1, const box<3, Type2>& value2)
        {
            return box<3, Type1>(value2) *= value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator*(const box<3, Type1>& value1, const box<3, Type2>& value2)
        {
            return box<3, Type1>(value1) *= value2;
        }

        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator/(const box<3, Type1>& value1, const Type2 value2)
        {
            return box<3, Type1>(value1) /= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator/(const Type1 value1, const box<3, Type2>& value2)
        {
            return box<3, Type1>(value2) /= value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator/(const box<3, Type1>& value1, const vector<3, Type2>& value2)
        {
            return box<3, Type1>(value1) /= value2;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator/(const vector<3, Type1>& value1, const box<3, Type2>& value2)
        {
            return box<3, Type1>(value2) /= value1;
        }
        template<typename Type1, typename Type2, TEMPLATE_ENABLE(is_castable<Type2, Type1>)>
        constexpr box<3, Type1> operator/(const box<3, Type1>& value1, const box<3, Type2>& value2)
        {
            return box<3, Type1>(value1) /= value2;
        }
    }
}
