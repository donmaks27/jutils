// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#include "jutils/configs/ini_parser.h"

#include <fstream>
#include <regex>
#include <sstream>

namespace jutils
{
	namespace ini
	{
		jstring serialize(const jmap<jstring, jmap<jstring, jstring>>& configData)
		{
			jstring data;
			for (const auto& sectionData : configData)
			{
				data += '[' + sectionData.key + "]\n";
				for (const auto& valueData : sectionData.value)
				{
					data += valueData.key + " = " + valueData.value + '\n';
				}
				data += '\n';
			}
			return data;
		}

		bool trimString(const std::string& str, int32 firstIndex, int32 lastIndex, jstring& outString)
		{
			if (lastIndex < firstIndex)
			{
				return false;
			}
			while ((static_cast<std::size_t>(firstIndex) < str.size()) && std::isspace(str[firstIndex]))
			{
				firstIndex++;
			}
			while ((lastIndex >= 0) && std::isspace(str[lastIndex]))
			{
				lastIndex--;
			}
			if (lastIndex < firstIndex)
			{
				return false;
			}
			outString = str.substr(firstIndex, lastIndex - firstIndex + 1);
			return true;
		}
		void parseLine(const std::string& line, jmap<jstring, jmap<jstring, jstring>>& configData, jstring& sectionName)
		{
			if (line.empty())
			{
				return;
			}

			const std::regex commentRegex("^\\s*((;|#).*){0,1}$");
			if (std::regex_search(line, commentRegex))
			{
				return;
			}

			const std::regex sectionRegex("^\\s*\\[[^\\[\\]]*\\].*$");
			if (std::regex_search(line, sectionRegex))
			{
				if (trimString(line, line.find('[') + 1, line.find(']') - 1, sectionName))
				{
					configData.getOrAdd(sectionName);
				}
				return;
			}
			if (sectionName.isEmpty())
			{
				return;
			}

			const std::regex keyValueRegex("^\\s*\\w+\\s*=.*$");
			if (std::regex_search(line, keyValueRegex))
			{
				const int32 index = line.find('=');
				jstring key;
				if (trimString(line, 0, index - 1, key))
				{
					jstring value;
					trimString(line, index + 1, line.size() - 1, value);
					configData[sectionName].add(key, value);
				}
			}
		}
		jmap<jstring, jmap<jstring, jstring>> parse(const jstring& data)
		{
			jmap<jstring, jmap<jstring, jstring>> configData;
			std::istringstream dataStream(data.getInternalData());
			std::string line;
			jstring sectionName;
			while (std::getline(dataStream, line))
			{
				parseLine(line, configData, sectionName);
			}
			return configData;
		}
		jmap<jstring, jmap<jstring, jstring>> parseFile(const jstring& filePath)
		{
			jmap<jstring, jmap<jstring, jstring>> configData;
			std::fstream file(*filePath);
			if (file.is_open())
			{
				std::string line;
				jstring sectionName;
				while (std::getline(file, line))
				{
					parseLine(line, configData, sectionName);
				}
				file.close();
			}
			return configData;
		}
	}
}
