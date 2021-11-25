// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "matrix_base.h"
#include "../../type_checks.h"
#include "../../jstring.h"
#include "../jmath_vector.h"

#include <cassert>

namespace jutils
{
    namespace math
    {
        template<typename Type>
        class matrix<2, 2, Type>
        {
        public:

            static constexpr vector_size_type rows_count = 2;
            static constexpr vector_size_type columns_count = 2;

            using type = Type;
            using column_type = vector<rows_count, type>;
            using row_type = vector<columns_count, type>;
            using transpose_type = matrix<columns_count, rows_count, type>;

            constexpr matrix() = default;
            constexpr explicit matrix(const type value)
                : rows{ row_type(value, 0), row_type(0, value) }
            {}
            constexpr matrix(const type x1, const type y1, const type x2, const type y2)
                : rows{ row_type(x1, y1), row_type(x2, y2) }
            {}
            constexpr matrix(const row_type& row1, const row_type& row2)
                : rows{ row1, row2 }
            {}
            template<typename OtherType>
            constexpr matrix(const matrix<2, 2, OtherType>& value)
                : rows{ value.rows[0], value.rows[1] }
            {}
            template<vector_size_type RowsCount, vector_size_type ColumnsCount, typename OtherType, TEMPLATE_ENABLE((RowsCount > 2) || (ColumnsCount > 2))>
            explicit constexpr matrix(const matrix<RowsCount, ColumnsCount, OtherType>& value)
                : rows{ row_type(value.rows[0]), row_type(value.rows[1]) }
            {}

            template<vector_size_type RowsCount, vector_size_type ColumnsCount, typename OtherType>
            constexpr matrix& operator=(const matrix<RowsCount, ColumnsCount, OtherType>& value)
            {
                if (this != &value)
                {
                    rows[0] = value.rows[0];
                    rows[1] = value.rows[1];
                }
                return *this;
            }

            row_type rows[rows_count];

            constexpr row_type& get(const vector_size_type index)
            {
                assert((index >= 0) && (index < rows_count));
                return rows[index];
            }
            constexpr const row_type& get(const vector_size_type index) const
            {
                assert((index >= 0) && (index < rows_count));
                return rows[index];
            }
            constexpr row_type& operator[](const vector_size_type index) { return get(index); }
            constexpr const row_type& operator[](const vector_size_type index) const { return get(index); }

            constexpr column_type getColumn(const vector_size_type index) const
            {
                assert((index >= 0) && (index < columns_count));
                return index == 0 ? column_type(rows[0].x, rows[1].x) : column_type(rows[0].y, rows[1].y);
            }

            template<typename OtherType>
            constexpr matrix& operator+=(const matrix<2, 2, OtherType>& value);
            template<typename OtherType>
            constexpr matrix& operator-=(const matrix<2, 2, OtherType>& value);
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr matrix& operator*=(const OtherType& value);
            template<typename OtherType>
            constexpr matrix& operator*=(const matrix<2, 2, OtherType>& value);

            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr bool operator==(const matrix<2, 2, OtherType>& value) const
            {
                return (rows[0] == value.rows[0]) && (rows[1] == value.rows[1]);
            }
            template<typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, type>)>
            constexpr bool operator!=(const matrix<2, 2, OtherType>& value) const { return !this->operator==(value); }

            jstring toString() const { return JSTR("{ ") + rows[0].toString() + JSTR("; ") + rows[1].toString() + JSTR(" }"); }

            constexpr transpose_type transpose() const { return { rows[0].x, rows[1].x, rows[0].y, rows[1].y }; }
        };

        template<typename Type, typename OtherType>
        constexpr matrix<2, 2, Type> operator+(const matrix<2, 2, Type>& value1, const matrix<2, 2, OtherType>& value2);
        template<typename Type, typename OtherType>
        constexpr matrix<2, 2, Type> operator-(const matrix<2, 2, Type>& value1, const matrix<2, 2, OtherType>& value2);

        template<typename Type, typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, Type>)>
        constexpr matrix<2, 2, Type> operator*(const matrix<2, 2, Type>& value1, const OtherType& value2);
        template<typename Type, typename OtherType, TEMPLATE_ENABLE(is_castable<OtherType, Type>)>
        constexpr matrix<2, 2, Type> operator*(const Type& value1, const matrix<2, 2, OtherType>& value2);
        template<typename Type, typename OtherType>
        constexpr matrix<2, 2, Type> operator*(const matrix<2, 2, Type>& value1, const matrix<2, 2, OtherType>& value2);
        template<typename Type, typename OtherType>
        constexpr vector<2, Type> operator*(const matrix<2, 2, Type>& value1, const vector<2, OtherType>& value2);
        template<typename Type, typename OtherType>
        constexpr vector<2, Type> operator*(const vector<2, Type>& value1, const matrix<2, 2, OtherType>& value2);



        template<typename Type>
        template<typename OtherType>
        constexpr matrix<2, 2, Type>& matrix<2, 2, Type>::operator+=(const matrix<2, 2, OtherType>& value)
        {
            this->rows[0] += value.rows[0];
            this->rows[1] += value.rows[1];
            return *this;
        }
        template<typename Type>
        template<typename OtherType>
        constexpr matrix<2, 2, Type>& matrix<2, 2, Type>::operator-=(const matrix<2, 2, OtherType>& value)
        {
            this->rows[0] -= value.rows[0];
            this->rows[1] -= value.rows[1];
            return *this;
        }
        template<typename Type>
        template<typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr matrix<2, 2, Type>& matrix<2, 2, Type>::operator*=(const OtherType& value)
        {
            this->rows[0] *= value;
            this->rows[1] *= value;
            return *this;
        }
        template<typename Type>
        template<typename OtherType>
        constexpr matrix<2, 2, Type>& matrix<2, 2, Type>::operator*=(const matrix<2, 2, OtherType>& value)
        {
            return *this = *this * value;
        }

        template<typename Type, typename OtherType>
        constexpr matrix<2, 2, Type> operator+(const matrix<2, 2, Type>& value1, const matrix<2, 2, OtherType>& value2)
        {
            return {
                value1.rows[0].x + static_cast<Type>(value2.rows[0].x), value1.rows[0].y + static_cast<Type>(value2.rows[0].y),
                value1.rows[1].x + static_cast<Type>(value2.rows[1].x), value1.rows[1].y + static_cast<Type>(value2.rows[1].y)
            };
        }
        template<typename Type, typename OtherType>
        constexpr matrix<2, 2, Type> operator-(const matrix<2, 2, Type>& value1, const matrix<2, 2, OtherType>& value2)
        {
            return {
                value1.rows[0].x - static_cast<Type>(value2.rows[0].x), value1.rows[0].y - static_cast<Type>(value2.rows[0].y),
                value1.rows[1].x - static_cast<Type>(value2.rows[1].x), value1.rows[1].y - static_cast<Type>(value2.rows[1].y)
            };
        }

        template<typename Type, typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr matrix<2, 2, Type> operator*(const matrix<2, 2, Type>& value1, const OtherType& value2)
        {
            return matrix<2, 2, Type>(value1) *= value2;
        }
        template<typename Type, typename OtherType, TEMPLATE_ENABLE_IMPL(is_castable<OtherType, Type>)>
        constexpr matrix<2, 2, Type> operator*(const Type& value1, const matrix<2, 2, OtherType>& value2)
        {
            return matrix<2, 2, Type>(value2) *= value1;
        }
        template<typename Type, typename OtherType>
        constexpr matrix<2, 2, Type> operator*(const matrix<2, 2, Type>& value1, const matrix<2, 2, OtherType>& value2)
        {
            return {
                value1.rows[0].x * static_cast<Type>(value2.rows[0].x) + value1.rows[0].y * static_cast<Type>(value2.rows[1].x),
                value1.rows[0].x * static_cast<Type>(value2.rows[0].y) + value1.rows[0].y * static_cast<Type>(value2.rows[1].y),

                value1.rows[1].x * static_cast<Type>(value2.rows[0].x) + value1.rows[1].y * static_cast<Type>(value2.rows[1].x),
                value1.rows[1].x * static_cast<Type>(value2.rows[0].y) + value1.rows[1].y * static_cast<Type>(value2.rows[1].y)
            };
        }
        template<typename Type, typename OtherType>
        constexpr vector<2, Type> operator*(const matrix<2, 2, Type>& value1, const vector<2, OtherType>& value2)
        {
            return { math::dot(value1.rows[0], value2), math::dot(value1.rows[1], value2) };
        }
        template<typename Type, typename OtherType>
        constexpr vector<2, Type> operator*(const vector<2, Type>& value1, const matrix<2, 2, OtherType>& value2)
        {
            return {
                value1.x * static_cast<Type>(value2.rows[0].x) + value1.y * static_cast<Type>(value2.rows[1].x),
                value1.x * static_cast<Type>(value2.rows[0].y) + value1.y * static_cast<Type>(value2.rows[1].y)
            };
        }
    }
}
