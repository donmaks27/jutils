﻿// Copyright © 2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "vec_base.h"

#include <glm/vec3.hpp>

template<typename T, glm::qualifier Q>
struct jutils::string_formatter<glm::vec<3, T, Q>> : std::true_type
{
    [[nodiscard]] static std::string format(const glm::vec<3, T, Q>& v) noexcept
    {
        return jutils::format("{{ {}; {}; {} }}", v.x, v.y, v.z);
    }
};