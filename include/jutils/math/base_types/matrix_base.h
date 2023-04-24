// Copyright © 2021-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vector_base.h"

namespace jutils
{
    namespace math
    {
        template<vector_size_type Rows, vector_size_type Columns, typename T> requires std::is_arithmetic_v<T>
        class _matrix_base
        {
        protected:

            constexpr _matrix_base() noexcept = default;

        public:

            static constexpr vector_size_type rowCount = Rows;
            static constexpr vector_size_type columnCount = Columns;
            using type = T;
            using row_type = vector<columnCount, type>;
            
            row_type rows[rowCount];

            constexpr row_type& get(const vector_size_type rowIndex) JUTILS_VECTOR_THROW_FUNCTION
            {
#ifndef JUTILS_VECTOR_DISABLE_THROW_ERRORS
                _checkRowIndexValid(rowIndex);
#endif
                return rows[jutils::math::clamp(rowIndex, 0, rowCount - 1)];
            }
            constexpr const row_type& get(const vector_size_type rowIndex) const JUTILS_VECTOR_THROW_FUNCTION
            {
#ifndef JUTILS_VECTOR_DISABLE_THROW_ERRORS
                _checkRowIndexValid(rowIndex);
#endif
                return rows[jutils::math::clamp(rowIndex, 0, rowCount - 1)];
            }
            constexpr row_type& operator[](const vector_size_type rowIndex) JUTILS_VECTOR_THROW_FUNCTION { return get(rowIndex); }
            constexpr const row_type& operator[](const vector_size_type rowIndex) const JUTILS_VECTOR_THROW_FUNCTION { return get(rowIndex); }
            
            constexpr type* getData() noexcept { return rows[0].getData(); }
            constexpr const type* getData() const noexcept { return rows[0].getData(); }

#ifndef JUTILS_VECTOR_DISABLE_THROW_ERRORS
        private:
            static constexpr void _checkRowIndexValid(const vector_size_type rowIndex)
            {
                if ((rowIndex < 0) || (rowIndex >= rowCount))
                {
                    throw std::out_of_range("Invalid row index");
                }
            }
#endif
        };

        template<vector_size_type Rows, vector_size_type Columns, typename T>
        class matrix;

        template<typename T>
        struct is_valid_matrix_type
        {
        private:
            template<typename T1> requires std::is_same_v<std::remove_cv_t<decltype(T1::rowCount)>, vector_size_type>
                                        && std::is_same_v<std::remove_cv_t<decltype(T1::columnCount)>, vector_size_type>
                                        && std::is_same_v<T1, matrix<T1::rowCount, T1::columnCount, typename T1::type>>
            static constexpr bool _is_valid_matrix_type(int32) noexcept { return true; }
            template<typename T1>
            static constexpr bool _is_valid_matrix_type(int8) noexcept { return false; }
        public:
            static constexpr bool value = _is_valid_matrix_type<std::remove_cvref_t<T>>(0);
        };
        template<typename T>
        constexpr bool is_valid_matrix_type_v = is_valid_matrix_type<T>::value;
        template<vector_size_type Rows, vector_size_type Columns, typename T>
        constexpr bool is_valid_matrix_v = is_valid_matrix_type_v<matrix<Rows, Columns, T>>;

        template<vector_size_type Rows, vector_size_type Columns, typename T> requires is_valid_matrix_v<Rows, Columns, T>
        jstring matrixToString(const matrix<Rows, Columns, T>& value) noexcept { return value.toString(); }
    }
}

template<jutils::math::vector_size_type R, jutils::math::vector_size_type C, typename T> requires jutils::math::is_valid_matrix_v<R, C, T>
struct jutils::string::formatter<jutils::math::matrix<R, C, T>> : std::true_type
{
    static jstring format(const jutils::math::matrix<R, C, T>& value) noexcept { return jutils::math::matrixToString(value); }
};

template<typename CharT, jutils::math::vector_size_type R, jutils::math::vector_size_type C, typename T>
    requires jutils::math::is_valid_matrix_v<R, C, T>
struct std::formatter<jutils::math::matrix<R, C, T>, CharT> : std::formatter<const char*, CharT>
{
    template<typename FormatContext>
    auto format(const jutils::math::matrix<R, C, T>& value, FormatContext& ctx)
    {
        return std::formatter<const char*, CharT>::format(*jutils::math::matrixToString(value), ctx);
    }
};
