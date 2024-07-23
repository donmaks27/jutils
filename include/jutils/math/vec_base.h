// Copyright © 2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../format.h"

#include <glm/detail/qualifier.hpp>

#if defined(JUTILS_USE_FMT)
    template<typename CharT, glm::length_t Size, typename T, glm::qualifier Q>
    struct fmt::formatter<glm::vec<Size, T, Q>, CharT> : fmt::formatter<jutils::jstring, CharT>
    {
        template<typename FormatContext>
        auto format(const glm::vec<Size, T, Q>& value, FormatContext& ctx)
        {
            return fmt::formatter<jutils::jstring, CharT>::format(jutils::toString(value), ctx);
        }
    };
#else
    template<typename CharT, glm::length_t Size, typename T, glm::qualifier Q>
    struct std::formatter<glm::vec<Size, T, Q>, CharT> : std::formatter<jutils::jstring, CharT>
    {
        template<typename FormatContext>
        auto format(const glm::vec<Size, T, Q>& value, FormatContext& ctx)
        {
            return std::formatter<jutils::jstring, CharT>::format(jutils::toString(value), ctx);
        }
    };
#endif
