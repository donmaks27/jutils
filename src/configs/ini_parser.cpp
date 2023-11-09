// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#include "jutils/configs/ini_parser.h"

#include <fstream>
#include <regex>
#include <sstream>

#include <cstring>
#define INI_IMPLEMENTATION
#define INI_STRNICMP( s1, s2, cnt ) ( std::strncmp( s1, s2, cnt ) )
#include <mattiasgustavsson/ini.h>

namespace jutils
{
	namespace ini
	{
		jstring serialize(const ini_value& configData)
		{
			ini_t* config = ini_create(nullptr);
			for (const auto& sectionData : configData)
			{
			    const int32 sectionIndex = ini_section_add(config, *sectionData.first, 0);
				for (const auto& propertyData : sectionData.second)
				{
				    ini_property_add(config, sectionIndex, *propertyData.first, 0, *propertyData.second, 0);
				}
			}

			const int32 configSize = ini_save(config, nullptr, 0);
			jstring str(configSize, ' ');
			ini_save(config, *str, configSize);
			ini_destroy(config);
			return str;
		}
		
		ini_value parse(const jstring& data)
		{
			ini_value configData;
			ini_t* config = ini_load(*data, nullptr);
			const int32 sectionsCount = ini_section_count(config);
			for (int32 sectionIndex = 1; sectionIndex < sectionsCount; sectionIndex++)
			{
				ini_section_value& sectionData = configData[ini_section_name(config, sectionIndex)];
			    const int32 propertiesCount = ini_property_count(config, sectionIndex);
				for (int32 propertyIndex = 0; propertyIndex < propertiesCount; propertyIndex++)
				{
					std::string key = ini_property_name(config, sectionIndex, propertyIndex);
					key.erase(std::find_if(key.rbegin(), key.rend(), [](const std::string::value_type ch)
					{
					    return !std::isspace(ch);
					}).base(), key.end());
					sectionData.add(
						jstring(key), 
						ini_property_value(config, sectionIndex, propertyIndex)
					);
				}
			}
			ini_destroy(config);
			return configData;
		}
		ini_value parseFile(const jstring& filePath)
		{
			std::ifstream file(*filePath);
			if (!file.is_open())
			{
			    return {};
			}
			jstring config;
			std::string line;
			while (std::getline(file, line))
			{
			    config += '\n' + jstring(line);
			}
			file.close();
			return parse(config);
		}
	}
}
