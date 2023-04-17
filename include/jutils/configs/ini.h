// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../jmap.h"
#include "../jstring.h"

namespace jutils
{
    namespace ini
    {
        using ini_section_value = jmap<jstring, jstring>;
        using ini_value = jmap<jstring, ini_section_value>;
    }
}
