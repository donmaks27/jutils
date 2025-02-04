// Copyright © 2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "mat_base.h"
#include "vec4.h"

#include <glm/mat3x4.hpp>

template<typename T, glm::qualifier Q>
struct jutils::formatter<glm::mat<3, 4, T, Q>> : std::true_type
{
    [[nodiscard]] static jutils::string format(const glm::mat<3, 4, T, Q>& v) noexcept
    {
        return jutils::format("{{ {}; {}; {} }}", v[0], v[1], v[2]);
    }
};
