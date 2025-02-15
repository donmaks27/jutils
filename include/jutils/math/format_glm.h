// Copyright © 2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../format.h"

#ifdef JUTILS_USE_GLM

    #include <glm/detail/qualifier.hpp>

    namespace jutils
    {
        template<glm::length_t Size, typename T, glm::qualifier Q>
        struct string_formatter<glm::vec<Size, T, Q>> : std::true_type
        {
            [[nodiscard]] static std::string format(const glm::vec<Size, T, Q>& v) noexcept
            {
                if constexpr (Size == 2)
                {
                    return jutils::format("{{ {}; {} }}", v.x, v.y);
                }
                else if constexpr (Size == 3)
                {
                    return jutils::format("{{ {}; {}; {} }}", v.x, v.y, v.z);
                }
                else if constexpr (Size == 4)
                {
                    return jutils::format("{{ {}; {}; {}; {} }}", v.x, v.y, v.z, v.w);
                }
                else
                {
                    return {};
                }
            }
        };

        template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
        struct string_formatter<glm::mat<C, R, T, Q>> : std::true_type
        {
            [[nodiscard]] static std::string format(const glm::mat<C, R, T, Q>& v) noexcept
            {
                if constexpr (C == 2)
                {
                    return jutils::format("{{ {}; {} }}", v[0], v[1]);
                }
                else if constexpr (C == 3)
                {
                    return jutils::format("{{ {}; {}; {} }}", v[0], v[1], v[2]);
                }
                else if constexpr (C == 4)
                {
                    return jutils::format("{{ {}; {}; {}; {} }}", v[0], v[1], v[2], v[3]);
                }
                else
                {
                    return {};
                }
            }
        };
    }

    namespace JUTILS_FORMAT_NAMESPACE
    {
        template<glm::length_t Size, typename T, glm::qualifier Q>
        struct formatter<glm::vec<Size, T, Q>> : formatter<std::string>
        {
            template<typename FormatContext>
            auto format(const glm::vec<Size, T, Q>& value, FormatContext& ctx)
            {
                return formatter<std::string>::format(jutils::toString(value), ctx);
            }
        };

        template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
        struct formatter<glm::mat<C, R, T, Q>> : formatter<std::string>
        {
            template<typename FormatContext>
            auto format(const glm::mat<C, R, T, Q>& value, FormatContext& ctx)
            {
                return formatter<std::string>::format(jutils::toString(value), ctx);
            }
        };
    }

#endif
