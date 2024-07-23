// Copyright © 2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vec_base.h"

#include <glm/vec4.hpp>

template<typename T, glm::qualifier Q>
struct jutils::formatter<glm::vec<4, T, Q>> : std::true_type
{
    [[nodiscard]] static jutils::jstring format(const glm::vec<4, T, Q>& v) noexcept
    {
        return jutils::format("{{ {}; {}; {}; {} }}", v.x, v.y, v.z, v.w);
    }
};
