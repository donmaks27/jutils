// Copyright © 2025 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../core.h"

#define JUTILS_IF_HAS_ARGS_SUPPORTED JUTILS_VA_OPT_SUPPORTED

#if JUTILS_IF_HAS_ARGS_SUPPORTED

    #define JUTILS_IF_HAS_ARGS_HELPER_BRANCH0(IfHasArgs, IfNoArgs) IfHasArgs
    #define JUTILS_IF_HAS_ARGS_HELPER_BRANCH1(IfHasArgs, IfNoArgs) IfNoArgs
    #define JUTILS_IF_HAS_ARGS_HELPER(IfHasArgs, IfNoArgs, Count, ...) JUTILS_IF_HAS_ARGS_HELPER_BRANCH ## Count (IfHasArgs, IfNoArgs)
    #define JUTILS_IF_HAS_ARGS(IfHasArgs, IfNoArgs, ...) JUTILS_IF_HAS_ARGS_HELPER( IfHasArgs, IfNoArgs, __VA_OPT__(0, ) 1 )

#endif
