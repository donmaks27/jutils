﻿// Copyright © 2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../format.h"

#include <glm/detail/qualifier.hpp>

template<glm::length_t Size, typename T, glm::qualifier Q>
struct JUTILS_FORMAT_NAMESPACE::formatter<glm::vec<Size, T, Q>> : JUTILS_FORMAT_NAMESPACE::formatter<jutils::string>
{
    template<typename FormatContext>
    auto format(const glm::vec<Size, T, Q>& value, FormatContext& ctx)
    {
        return JUTILS_FORMAT_NAMESPACE::formatter<jutils::string>::format(jutils::toString(value), ctx);
    }
};
