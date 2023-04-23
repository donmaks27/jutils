// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "matrix_operations.h"

namespace jutils
{
    namespace math
    {
        template<vector_size_type Columns, typename T>
        class _matrix2_base : public _matrix_base<2, Columns, T>
        {
            using base_type = _matrix_base<2, Columns, T>;

        public:
            
            using type = typename base_type::type;
            using row_type = typename base_type::row_type;

        protected:
            
            constexpr _matrix2_base() noexcept = default;
            constexpr _matrix2_base(const row_type& row1, const row_type& row2) noexcept
            {
                base_type::rows[0] = row1;
                base_type::rows[1] = row2;
            }
            template<vector_size_type R, vector_size_type C, typename Other>
            constexpr _matrix2_base(const _matrix_base<R, C, Other>& value) noexcept
            {
                base_type::rows[0] = value.rows[0];
                base_type::rows[1] = value.rows[1];
            }

        public:

            template<typename Other>
            constexpr bool operator==(const _matrix2_base<base_type::columnCount, Other>& value) const noexcept
            {
                return (base_type::rows[0] == value.rows[0]) && (base_type::rows[1] == value.rows[1]);
            }
            template<typename Other>
            constexpr bool operator!=(const _matrix2_base<base_type::columnCount, Other>& value) const noexcept
            {
                return !this->operator==(value);
            }

            jstring toString() const noexcept { return JSTR_FORMAT("{{ {}; {} }}", base_type::rows[0], base_type::rows[1]); }

        protected:

            template<vector_size_type R, vector_size_type C, typename Other>
            constexpr void _assign(const _matrix_base<R, C, Other>& value) noexcept
            {
                base_type::rows[0] = value.rows[0];
                base_type::rows[1] = value.rows[1];
            }

            template<typename R>
            constexpr R _absTemplate() const noexcept { return { jutils::math::abs(base_type::rows[0]), jutils::math::abs(base_type::rows[1]) }; }

            template<typename Other>
            constexpr void _add(const _matrix2_base<base_type::columnCount, Other>& value) noexcept
            {
                base_type::rows[0] += value.rows[0];
                base_type::rows[1] += value.rows[1];
            }
            template<typename Other>
            constexpr void _subtract(const _matrix2_base<base_type::columnCount, Other>& value) noexcept
            {
                base_type::rows[0] -= value.rows[0];
                base_type::rows[1] -= value.rows[1];
            }
            template<typename Other>
            constexpr void _multiply(const Other& value) noexcept
            {
                base_type::rows[0] *= value;
                base_type::rows[1] *= value;
            }
        };
    }
}
