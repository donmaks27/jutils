// Copyright © 2021-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../../format.h"

namespace jutils::math
{
    using vector_size_type = int8;

    template<vector_size_type Size, typename T>
    class vector;

    template<vector_size_type Size, typename T>
    struct is_valid_vector
    {
    private:
        template<typename T1 = void>
        static constexpr auto _helper_type(int32) -> decltype(vector<Size, T>::type);
        template<typename T1 = void>
        static constexpr void _helper_type(int8);
        template<typename T1 = void>
        static constexpr vector_size_type _helper_size(int32) noexcept { return vector<Size, T>::size; }
        template<typename T1 = void>
        static constexpr vector_size_type _helper_size(int8) noexcept { return 0; }
    public:
        static constexpr bool value = (_helper_size(0) == Size) && std::is_same_v<decltype(_helper_type(0)), T>;
    };
    template<vector_size_type Size, typename T>
    constexpr bool is_valid_vector_v = is_valid_vector<Size, T>::value;

    JUTILS_TEMPLATE_CONDITION((is_valid_vector_v<Size, T>), vector_size_type Size, typename T)
    [[nodiscard]] jstring vectorToString(const vector<Size, T>& value) noexcept { return value.toString(); }
}

template<jutils::math::vector_size_type Size, typename T>
struct jutils::formatter<jutils::math::vector<Size, T>> : std::true_type
{
    [[nodiscard]] static jstring format(const jutils::math::vector<Size, T>& value) noexcept { return jutils::math::vectorToString(value); }
};

#if defined(JUTILS_USE_FMT)
    template<typename CharT, jutils::math::vector_size_type Size, typename T>
    struct fmt::formatter<jutils::math::vector<Size, T>, CharT> : fmt::formatter<const char*, CharT>
    {
        template<typename FormatContext>
        auto format(const jutils::math::vector<Size, T>& value, FormatContext& ctx)
        {
            return fmt::formatter<const char*, CharT>::format(*jutils::math::vectorToString(value), ctx);
        }
    };
#else
    template<typename CharT, jutils::math::vector_size_type Size, typename T>
    struct std::formatter<jutils::math::vector<Size, T>, CharT> : std::formatter<const char*, CharT>
    {
        template<typename FormatContext>
        auto format(const jutils::math::vector<Size, T>& value, FormatContext& ctx)
        {
            return std::formatter<const char*, CharT>::format(*jutils::math::vectorToString(value), ctx);
        }
    };
#endif