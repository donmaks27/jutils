// Copyright © 2022-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "json.h"

namespace jutils::json
{
    jstring serialize(const json::value& data, bool prettyPrint = false);

    json::value parse(const jstring& data);
    json::value parseFile(const jstring& filePath);
}
