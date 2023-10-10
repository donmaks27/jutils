// Copyright © 2022-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vector_base.h"

namespace jutils
{
    namespace math
    {
        template<vector_size_type Size, typename T>
        class box;

        template<typename T>
        struct is_valid_box_type
        {
        private:
            template<typename T1> requires std::is_same_v<std::remove_cv_t<decltype(T1::size)>, vector_size_type>
                                        && std::is_same_v<T1, box<T1::size, typename T1::type>>
            static constexpr bool _is_valid_box(int32) noexcept { return true; }
            template<typename T1>
            static constexpr bool _is_valid_box(int8) noexcept { return false; }
        public:
            static constexpr bool value = _is_valid_box<std::remove_cvref_t<T>>(0);
        };
        template<typename T>
        constexpr bool is_valid_box_type_v = is_valid_box_type<T>::value;
        template<vector_size_type Size, typename T>
        constexpr bool is_valid_box_v = is_valid_box_type_v<box<Size, T>>;

        template<vector_size_type Size, typename T> requires is_valid_box_v<Size, T>
        jstring boxToString(const box<Size, T>& value) noexcept { return value.toString(); }
    }
}

template<jutils::math::vector_size_type Size, typename T> requires jutils::math::is_valid_box_v<Size, T>
struct jutils::string::formatter<jutils::math::box<Size, T>> : std::true_type
{
    static jstring format(const jutils::math::box<Size, T>& value) noexcept { return jutils::math::boxToString(value); }
};

#if defined(JUTILS_USE_FMT)
template<typename CharT, jutils::math::vector_size_type Size, typename T> requires jutils::math::is_valid_box_v<Size, T>
struct fmt::formatter<jutils::math::box<Size, T>, CharT> : fmt::formatter<const char*, CharT>
{
    template<typename FormatContext>
    auto format(const jutils::math::vector<Size, T>& value, FormatContext& ctx)
    {
        return fmt::formatter<const char*, CharT>::format(*jutils::math::boxToString(value), ctx);
    }
};
#else
template<typename CharT, jutils::math::vector_size_type Size, typename T> requires jutils::math::is_valid_box_v<Size, T>
struct std::formatter<jutils::math::box<Size, T>, CharT> : std::formatter<const char*, CharT>
{
    template<typename FormatContext>
    auto format(const jutils::math::vector<Size, T>& value, FormatContext& ctx)
    {
        return std::formatter<const char*, CharT>::format(*jutils::math::boxToString(value), ctx);
    }
};
#endif
