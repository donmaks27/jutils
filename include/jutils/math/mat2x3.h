// Copyright © 2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "mat_base.h"
#include "vec3.h"

#include <glm/mat2x3.hpp>

template<typename T, glm::qualifier Q>
struct jutils::string_formatter<glm::mat<2, 3, T, Q>> : std::true_type
{
    [[nodiscard]] static jutils::string format(const glm::mat<2, 3, T, Q>& v) noexcept
    {
        return jutils::format("{{ {}; {} }}", v[0], v[1]);
    }
};
