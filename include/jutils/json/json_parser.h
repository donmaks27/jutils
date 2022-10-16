﻿// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "json.h"

namespace jutils
{
    namespace json
    {
        jstring serialize(const json_value& data);
        json_value parse(const jstring& data);
    }
}
