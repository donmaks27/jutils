// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../jmap.h"
#include "../jstring.h"

namespace jutils
{
	namespace ini
	{
		jstring serialize(const jmap<jstring, jmap<jstring, jstring>>& configData);

		jmap<jstring, jmap<jstring, jstring>> parse(const jstring& data);
		jmap<jstring, jmap<jstring, jstring>> parseFile(const jstring& filePath);
	}
}
