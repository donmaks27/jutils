// Copyright © 2022-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "json.h"

namespace jutils
{
    namespace json
    {
        jstring serialize(const json_value& data);

        json_value parse(const jstring& data);
        json_value parseFile(const jstring& filePath);
    }
}
