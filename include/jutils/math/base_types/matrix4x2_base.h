// Copyright © 2021-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "matrix4_base.h"
#include "vector4_base.h"

namespace jutils
{
    namespace math
    {
        template<typename T>
        class matrix<4, 2, T> : public _matrix4_base<2, T>
        {
            using base_type = _matrix4_base<2, T>;
        public:
            
            using type = typename base_type::type;
            using row_type = typename base_type::row_type;

            constexpr matrix() noexcept = default;
            constexpr matrix(const type x1, const type y1, 
                             const type x2, const type y2, 
                             const type x3, const type y3, 
                             const type x4, const type y4) noexcept
                : base_type(row_type(x1, y1), row_type(x2, y2), row_type(x3, y3), row_type(x4, y4))
            {}
            constexpr matrix(const row_type& row1, const row_type& row2, const row_type& row3, const row_type& row4) noexcept
                : base_type(row1, row2, row3, row4)
            {}
            template<vector_size_type R, vector_size_type C, typename Other>
            constexpr matrix(const matrix<R, C, Other>& value) noexcept
                : base_type(value)
            {}
            
            template<vector_size_type R, vector_size_type C, typename Other>
            constexpr matrix& operator=(const matrix<R, C, Other>& value) noexcept
            {
                base_type::_assign(value);
                return *this;
            }
            
            template<typename R = type>
            constexpr matrix<base_type::rowCount, base_type::columnCount, R> copy() const noexcept { return *this; }
            constexpr matrix abs() const noexcept { return base_type::template _absTemplate<matrix>(); }

            template<typename Other>
            constexpr matrix& operator+=(const matrix<base_type::rowCount, base_type::columnCount, Other>& value) noexcept
            {
                base_type::_add(value);
                return *this;
            }
            template<typename Other>
            constexpr matrix& operator-=(const matrix<base_type::rowCount, base_type::columnCount, Other>& value) noexcept
            {
                base_type::_subtract(value);
                return *this;
            }
            template<typename Other> requires std::is_arithmetic_v<Other>
            constexpr matrix& operator*=(const Other& value) noexcept
            {
                base_type::_multiply(value);
                return *this;
            }
        };
    }
}
