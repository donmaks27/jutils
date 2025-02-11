// Copyright © 2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "mat_base.h"
#include "vec2.h"

#include <glm/mat3x2.hpp>

template<typename T, glm::qualifier Q>
struct jutils::string_formatter<glm::mat<3, 2, T, Q>> : std::true_type
{
    [[nodiscard]] static std::string format(const glm::mat<3, 2, T, Q>& v) noexcept
    {
        return jutils::format("{{ {}; {}; {} }}", v[0], v[1], v[2]);
    }
};
