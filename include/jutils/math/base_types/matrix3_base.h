// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "matrix_operations.h"

namespace jutils
{
    namespace math
    {
        template<vector_size_type Columns, typename T>
        class _matrix3_base : public _matrix_base<3, Columns, T>
        {
            using base_type = _matrix_base<3, Columns, T>;

        public:
            
            using type = typename base_type::type;
            using row_type = typename base_type::row_type;

        protected:
            
            constexpr _matrix3_base() noexcept = default;
            constexpr _matrix3_base(const row_type& row1, const row_type& row2, const row_type& row3) noexcept
            {
                base_type::rows[0] = row1;
                base_type::rows[1] = row2;
                base_type::rows[2] = row3;
            }
            template<vector_size_type R, vector_size_type C, typename Other> requires (R >= base_type::rowCount)
            constexpr _matrix3_base(const _matrix_base<R, C, Other>& value) noexcept
            {
                base_type::rows[0] = value.rows[0];
                base_type::rows[1] = value.rows[1];
                base_type::rows[2] = value.rows[2];
            }
            template<vector_size_type C, typename Other>
            constexpr _matrix3_base(const _matrix_base<2, C, Other>& value) noexcept
            {
                base_type::rows[0] = value.rows[0];
                base_type::rows[1] = value.rows[1];
            }

        public:

            template<typename Other>
            constexpr bool operator==(const _matrix3_base<base_type::columnCount, Other>& value) const noexcept
            {
                return (base_type::rows[0] == value.rows[0])
                    && (base_type::rows[1] == value.rows[1])
                    && (base_type::rows[2] == value.rows[2]);
            }
            template<typename Other>
            constexpr bool operator!=(const _matrix3_base<base_type::columnCount, Other>& value) const noexcept
            {
                return !this->operator==(value);
            }
            
            jstring toString() const noexcept
            {
                return JSTR_FORMAT("{{ {}; {}; {} }}", base_type::rows[0], base_type::rows[1], base_type::rows[2]);
            }

        protected:

            template<vector_size_type R, vector_size_type C, typename Other> requires (R >= base_type::rowCount)
            constexpr void _assign(const _matrix_base<R, C, Other>& value) noexcept
            {
                base_type::rows[0] = value.rows[0];
                base_type::rows[1] = value.rows[1];
                base_type::rows[2] = value.rows[2];
            }
            template<vector_size_type C, typename Other>
            constexpr void _assign(const _matrix_base<2, C, Other>& value) noexcept
            {
                base_type::rows[0] = value.rows[0];
                base_type::rows[1] = value.rows[1];
                base_type::rows[2] = base_type::row_type(0);
            }

            template<typename R>
            constexpr R _absTemplate() const noexcept
            {
                return {
                    jutils::math::abs(base_type::rows[0]),
                    jutils::math::abs(base_type::rows[1]),
                    jutils::math::abs(base_type::rows[2])
                };
            }

            template<typename Other>
            constexpr void _add(const _matrix3_base<base_type::columnCount, Other>& value) noexcept
            {
                base_type::rows[0] += value.rows[0];
                base_type::rows[1] += value.rows[1];
                base_type::rows[2] += value.rows[2];
            }
            template<typename Other>
            constexpr void _subtract(const _matrix3_base<base_type::columnCount, Other>& value) noexcept
            {
                base_type::rows[0] -= value.rows[0];
                base_type::rows[1] -= value.rows[1];
                base_type::rows[2] -= value.rows[2];
            }
            template<typename Other>
            constexpr void _multiply(const Other& value) noexcept
            {
                base_type::rows[0] *= value;
                base_type::rows[1] *= value;
                base_type::rows[2] *= value;
            }
        };
    }
}
