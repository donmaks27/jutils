﻿// Copyright © 2021-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../../jstring.h"

/**
 * #define JUTILS_VECTOR_DISABLE_THROW_ERRORS - disable throwing out_of_range exception
 */
#ifdef JUTILS_VECTOR_DISABLE_THROW_ERRORS
    #define JUTILS_VECTOR_THROW_FUNCTION noexcept
#else
    #define JUTILS_VECTOR_THROW_FUNCTION
#endif

namespace jutils
{
    namespace math
    {
        using vector_size_type = int8;

        template<vector_size_type Size, typename T>
        class vector;

        template<typename T>
        struct is_valid_vector_type
        {
        private:
            template<typename T1> requires std::is_same_v<std::remove_cv_t<decltype(T1::size)>, vector_size_type>
                                        && std::is_same_v<T1, vector<T1::size, typename T1::type>>
            static constexpr bool _is_valid_vector(int32) noexcept { return true; }
            template<typename T1>
            static constexpr bool _is_valid_vector(int8) noexcept { return false; }
        public:
            static constexpr bool value = _is_valid_vector<std::remove_cvref_t<T>>(0);
        };
        template<typename T>
        constexpr bool is_valid_vector_type_v = is_valid_vector_type<T>::value;
        template<vector_size_type Size, typename T>
        constexpr bool is_valid_vector_v = is_valid_vector_type_v<vector<Size, T>>;

        template<vector_size_type Size, typename T> requires is_valid_vector_v<Size, T>
        jstring vectorToString(const vector<Size, T>& value) noexcept { return value.toString(); }
    }
}

template<jutils::math::vector_size_type Size, typename T> requires jutils::math::is_valid_vector_v<Size, T>
struct jutils::string::formatter<jutils::math::vector<Size, T>> : std::true_type
{
    static jstring format(const jutils::math::vector<Size, T>& value) noexcept { return jutils::math::vectorToString(value); }
};

#if defined(JUTILS_USE_FMT)
template<typename CharT, jutils::math::vector_size_type Size, typename T> requires jutils::math::is_valid_vector_v<Size, T>
struct fmt::formatter<jutils::math::vector<Size, T>, CharT> : fmt::formatter<const char*, CharT>
{
    template<typename FormatContext>
    auto format(const jutils::math::vector<Size, T>& value, FormatContext& ctx)
    {
        return fmt::formatter<const char*, CharT>::format(*jutils::math::vectorToString(value), ctx);
    }
};
#else
template<typename CharT, jutils::math::vector_size_type Size, typename T> requires jutils::math::is_valid_vector_v<Size, T>
struct std::formatter<jutils::math::vector<Size, T>, CharT> : std::formatter<const char*, CharT>
{
    template<typename FormatContext>
    auto format(const jutils::math::vector<Size, T>& value, FormatContext& ctx)
    {
        return std::formatter<const char*, CharT>::format(*jutils::math::vectorToString(value), ctx);
    }
};
#endif