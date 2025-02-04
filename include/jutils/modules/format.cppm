// Copyright © 2025 Leonov Maksim. All Rights Reserved.

module;

#include "../core.h"

#include <string>
#ifdef JUTILS_USE_FMT
    #include <fmt/format.h>
#else
    #include <format>
#endif

export module jutils.format;

export import jutils.string;

#define JUTILS_USE_MODULES
#include "../format.h"