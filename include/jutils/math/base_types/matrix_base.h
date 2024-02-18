// Copyright © 2021-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vector_base.h"

namespace jutils::math
{
#if JUTILS_STD_VERSION >= JUTILS_STD20
    template<vector_size_type R, vector_size_type C, typename T> requires std::is_arithmetic_v<T>
    class _matrix_base
    {
#else
        template<vector_size_type R, vector_size_type C, typename T>
    class _matrix_base
    {
        static_assert(std::is_arithmetic_v<T>);
#endif
    protected:
        
        using default_float_type = std::conditional_t<std::is_floating_point_v<T>, T, float>;

        constexpr _matrix_base() noexcept = default;

    public:

        static constexpr vector_size_type rowCount = R;
        static constexpr vector_size_type columnCount = C;
        using type = T;
        using row_type = vector<columnCount, type>;

        row_type rows[rowCount];

        [[nodiscard]] constexpr row_type& get(const vector_size_type rowIndex) noexcept
        {
            _checkRowIndexValid(rowIndex);
            return rows[rowIndex];
        }
        [[nodiscard]] constexpr const row_type& get(const vector_size_type rowIndex) const noexcept
        {
            _checkRowIndexValid(rowIndex);
            return rows[rowIndex];
        }
        [[nodiscard]] constexpr row_type& operator[](const vector_size_type rowIndex) noexcept { return get(rowIndex); }
        [[nodiscard]] constexpr const row_type& operator[](const vector_size_type rowIndex) const noexcept { return get(rowIndex); }

        [[nodiscard]] constexpr type* getData() noexcept { return rows[0].getData(); }
        [[nodiscard]] constexpr const type* getData() const noexcept { return rows[0].getData(); }

    private:

        static constexpr void _checkRowIndexValid(const vector_size_type rowIndex)
        {
            if ((rowIndex < 0) || (rowIndex >= rowCount))
            {
                throw std::out_of_range("Invalid row index");
            }
        }
    };

    template<vector_size_type R, vector_size_type C, typename T>
    class matrix;

    template<vector_size_type R, vector_size_type C, typename T>
    struct is_valid_matrix
    {
    private:
        template<typename T1 = void>
        static constexpr vector_size_type _helper_size_rows(int32) noexcept { return matrix<R, C, T>::rowCount; }
        template<typename T1 = void>
        static constexpr vector_size_type _helper_size_rows(int8) noexcept { return 0; }
        template<typename T1 = void>
        static constexpr vector_size_type _helper_size_columns(int32) noexcept { return matrix<R, C, T>::columnCount; }
        template<typename T1 = void>
        static constexpr vector_size_type _helper_size_columns(int8) noexcept { return 0; }
        template<typename T1 = void>
        static constexpr auto _helper_type(int32) -> decltype(matrix<R, C, T>::type);
        template<typename T1 = void>
        static constexpr void _helper_type(int8);
    public:
        static constexpr bool value = (_helper_size_rows(0) == R)
                                   && (_helper_size_columns(0) == C)
                                   && std::is_same_v<decltype(_helper_type(0)), T>;
    };
    template<vector_size_type R, vector_size_type C, typename T>
    constexpr bool is_valid_matrix_v = is_valid_matrix<R, C, T>::value;

    JUTILS_TEMPLATE_CONDITION((is_valid_matrix_v<R, C, T>), vector_size_type R, vector_size_type C, typename T)
    [[nodiscard]] jstring matrixToString(const matrix<R, C, T>& value) noexcept { return value.toString(); }
}

template<jutils::math::vector_size_type R, jutils::math::vector_size_type C, typename T>
struct jutils::formatter<jutils::math::matrix<R, C, T>> : std::true_type
{
    [[nodiscard]] static jstring format(const jutils::math::matrix<R, C, T>& value) noexcept { return jutils::math::matrixToString(value); }
};

#if defined(JUTILS_USE_FMT)
    template<typename CharT, jutils::math::vector_size_type R, jutils::math::vector_size_type C, typename T>
    struct fmt::formatter<jutils::math::matrix<R, C, T>, CharT> : fmt::formatter<const char*, CharT>
    {
        template<typename FormatContext>
        auto format(const jutils::math::matrix<R, C, T>& value, FormatContext& ctx)
        {
            return fmt::formatter<const char*, CharT>::format(*jutils::math::matrixToString(value), ctx);
        }
    };
#else
    template<typename CharT, jutils::math::vector_size_type R, jutils::math::vector_size_type C, typename T>
    struct std::formatter<jutils::math::matrix<R, C, T>, CharT> : std::formatter<const char*, CharT>
    {
        template<typename FormatContext>
        auto format(const jutils::math::matrix<R, C, T>& value, FormatContext& ctx)
        {
            return std::formatter<const char*, CharT>::format(*jutils::math::matrixToString(value), ctx);
        }
    };
#endif
