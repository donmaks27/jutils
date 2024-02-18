// Copyright © 2021-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "matrix4_base.h"
#include "vector4_base.h"

namespace jutils::math
{
    template<typename T>
    class matrix<4, 3, T> : public _matrix4_base<3, T>
    {
        using base_type = _matrix4_base<3, T>;

    public:

        using type = typename base_type::type;
        using row_type = typename base_type::row_type;

        constexpr matrix() noexcept = default;
        constexpr matrix(const type x1, const type y1, const type z1,
                         const type x2, const type y2, const type z2,
                         const type x3, const type y3, const type z3,
                         const type x4, const type y4, const type z4) noexcept
            : base_type(row_type(x1, y1, z1), row_type(x2, y2, z2), row_type(x3, y3, z3), row_type(x4, y4, z4))
        {}
        constexpr matrix(const row_type& row1, const row_type& row2, const row_type& row3, const row_type& row4) noexcept
            : base_type(row1, row2, row3, row4)
        {}
        JUTILS_TEMPLATE_CONDITION((R1 >= base_type::rowCount), vector_size_type R1, vector_size_type C1, typename T1)
        constexpr matrix(const matrix<R1, C1, T1>& value) noexcept
            : base_type(value)
        {}
        template<vector_size_type C1, typename T1>
        constexpr matrix(const matrix<3, C1, T1>& value, const row_type& row4 = row_type()) noexcept
            : base_type(value, row4)
        {}
        template<vector_size_type C1, typename T1>
        constexpr matrix(const matrix<2, C1, T1>& value, const row_type& row3 = row_type(), const row_type& row4 = row_type()) noexcept
            : base_type(value, row3, row4)
        {}

        template<vector_size_type R1, vector_size_type C1, typename T1>
        constexpr matrix& operator=(const matrix<R1, C1, T1>& value) noexcept
        {
            base_type::_assign(value);
            return *this;
        }

        template<typename Ret = type>
        [[nodiscard]] constexpr matrix<base_type::rowCount, base_type::columnCount, Ret> copy() const noexcept { return *this; }
        [[nodiscard]] constexpr matrix abs() const noexcept { return base_type::template _abs<matrix>(); }

        template<typename T1>
        constexpr matrix& operator+=(const matrix<base_type::rowCount, base_type::columnCount, T1>& value) noexcept
        {
            base_type::_add(value);
            return *this;
        }
        template<typename T1>
        constexpr matrix& operator-=(const matrix<base_type::rowCount, base_type::columnCount, T1>& value) noexcept
        {
            base_type::_subtract(value);
            return *this;
        }
        JUTILS_TEMPLATE_CONDITION(std::is_arithmetic_v<T1>, typename T1)
        constexpr matrix& operator*=(const T1 value) noexcept
        {
            base_type::_multiply(value);
            return *this;
        }
    };
}
