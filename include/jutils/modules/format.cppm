// Copyright © 2025 Leonov Maksim. All Rights Reserved.

module;

#define JUTILS_MODULE

#include "../core.h"
#include <type_traits>
#include <string>
#ifdef JUTILS_USE_FMT
    #include <fmt/format.h>
#else
    #include <format>
#endif

export module jutils.format;

export import jutils.string;

#include "../format.h"