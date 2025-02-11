// Copyright © 2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../format.h"

#include <glm/detail/qualifier.hpp>

template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
struct JUTILS_FORMAT_NAMESPACE::formatter<glm::mat<C, R, T, Q>> : JUTILS_FORMAT_NAMESPACE::formatter<std::string>
{
    template<typename FormatContext>
    auto format(const glm::mat<C, R, T, Q>& value, FormatContext& ctx)
    {
        return JUTILS_FORMAT_NAMESPACE::formatter<std::string>::format(jutils::toString(value), ctx);
    }
};