// Copyright © 2021-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "matrix2_base.h"
#include "vector2_base.h"

namespace jutils::math
{
    template<typename T>
    class matrix<2, 2, T> : public _matrix2_base<2, T>
    {
        using base_type = _matrix2_base<2, T>;

    public:

        using type = typename base_type::type;
        using row_type = typename base_type::row_type;

        constexpr matrix() noexcept = default;
        explicit constexpr matrix(const type value) noexcept
            : base_type(row_type(value, 0), row_type(0, value))
        {}
        constexpr matrix(const type x1, const type y1,
                         const type x2, const type y2) noexcept
            : base_type(row_type(x1, y1), row_type(x2, y2))
        {}
        constexpr matrix(const row_type& row1, const row_type& row2) noexcept
            : base_type(row1, row2)
        {}
        template<vector_size_type R1, vector_size_type C1, typename T1>
        constexpr matrix(const matrix<R1, C1, T1>& value) noexcept
            : base_type(value)
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
