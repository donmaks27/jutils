// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "ini.h"

namespace jutils
{
	namespace ini
	{
		jstring serialize(const ini_value& configData);

		ini_value parse(const jstring& data);
		ini_value parseFile(const jstring& filePath);
	}
}
