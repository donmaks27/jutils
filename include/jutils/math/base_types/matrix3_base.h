﻿// Copyright © 2023-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../math_matrix.h"

namespace jutils::math
{
    template<vector_size_type C, typename T>
    class _matrix3_base : public _matrix_base<3, C, T>
    {
        using base_type = _matrix_base<3, C, T>;
        using default_float_type = typename base_type::default_float_type;

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
        JUTILS_TEMPLATE_CONDITION((R1 >= base_type::rowCount), vector_size_type R1, vector_size_type C1, typename T1)
        constexpr _matrix3_base(const _matrix_base<R1, C1, T1>& value) noexcept
        {
            base_type::rows[0] = value.rows[0];
            base_type::rows[1] = value.rows[1];
            base_type::rows[2] = value.rows[2];
        }
        template<vector_size_type C1, typename T1>
        constexpr _matrix3_base(const _matrix_base<2, C1, T1>& value, const row_type& row3) noexcept
        {
            base_type::rows[0] = value.rows[0];
            base_type::rows[1] = value.rows[1];
            base_type::rows[2] = row3;
        }

    public:

        template<typename T1>
        [[nodiscard]] constexpr bool isEqual(const _matrix3_base<base_type::columnCount, T1>& value, const default_float_type eps) const noexcept
        {
            return base_type::rows[0].isEqual(value.rows[0], eps)
                && base_type::rows[1].isEqual(value.rows[1], eps)
                && base_type::rows[2].isEqual(value.rows[2], eps);
        }
        template<typename T1>
        [[nodiscard]] constexpr bool isEqual(const _matrix3_base<base_type::columnCount, T1>& value) const noexcept
        {
            return base_type::rows[0].isEqual(value.rows[0])
                && base_type::rows[1].isEqual(value.rows[1])
                && base_type::rows[2].isEqual(value.rows[2]);
        }

        template<typename Other>
        [[nodiscard]] constexpr bool operator==(const _matrix3_base<base_type::columnCount, Other>& value) const noexcept
        {
            return (base_type::rows[0] == value.rows[0])
                && (base_type::rows[1] == value.rows[1])
                && (base_type::rows[2] == value.rows[2]);
        }
        template<typename Other>
        [[nodiscard]] constexpr bool operator!=(const _matrix3_base<base_type::columnCount, Other>& value) const noexcept
        {
            return !this->operator==(value);
        }

        [[nodiscard]] jstring toString() const noexcept
        {
            return jutils::format("{{ {}; {}; {} }}", base_type::rows[0], base_type::rows[1], base_type::rows[2]);
        }

    protected:

        JUTILS_TEMPLATE_CONDITION((R1 >= base_type::rowCount), vector_size_type R1, vector_size_type C1, typename T1)
        constexpr void _assign(const _matrix_base<R1, C1, T1>& value) noexcept
        {
            base_type::rows[0] = value.rows[0];
            base_type::rows[1] = value.rows[1];
            base_type::rows[2] = value.rows[2];
        }
        template<vector_size_type C1, typename T1>
        constexpr void _assign(const _matrix_base<2, C1, T1>& value) noexcept
        {
            base_type::rows[0] = value.rows[0];
            base_type::rows[1] = value.rows[1];
            base_type::rows[2] = base_type::row_type(0);
        }

        template<typename MatrixT>
        constexpr MatrixT _abs() const noexcept
        {
            return { base_type::rows[0].abs(), base_type::rows[1].abs(), base_type::rows[2].abs() };
        }

        template<typename T1>
        constexpr void _add(const _matrix3_base<base_type::columnCount, T1>& value) noexcept
        {
            base_type::rows[0] += value.rows[0];
            base_type::rows[1] += value.rows[1];
            base_type::rows[2] += value.rows[2];
        }
        template<typename T1>
        constexpr void _subtract(const _matrix3_base<base_type::columnCount, T1>& value) noexcept
        {
            base_type::rows[0] -= value.rows[0];
            base_type::rows[1] -= value.rows[1];
            base_type::rows[2] -= value.rows[2];
        }
        template<typename T1>
        constexpr void _multiply(const T1 value) noexcept
        {
            base_type::rows[0] *= value;
            base_type::rows[1] *= value;
            base_type::rows[2] *= value;
        }
    };
}
