// Copyright © 2025 Leonov Maksim. All Rights Reserved.

module;

#define JUTILS_MODULE

#include <cmath>
#include <initializer_list>
#include <numbers>
#include <string>
#ifdef JUTILS_USE_GLM
    #include <glm/detail/qualifier.hpp>
#endif

export module jutils.math;

export import jutils.base_types;
export import jutils.type_traits;

#include "../math/math.h"
#include "../math/hash.h"
