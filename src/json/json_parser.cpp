// Copyright 2022 Leonov Maksim. All Rights Reserved.

#include "../../include/jutils/json/json_parser.h"

#include <cctype>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace jutils
{
    namespace json
    {
        json_value parse(const jstring& data, int32 startIndex, int32& outEndIndex);
        int32 skipSpaces(const jstring& data, int32 startIndex);

        jstring serialize(const json_value& data)
        {
            if (data != nullptr)
            {
                switch (data->getType())
                {
                case json_value_type::boolean: return data->asBool() ? "true" : "false";
                case json_value_type::number:
                    {
                        const double value = data->asNumber();
                        const int64 valueInt = math::roundDown<int64>(value);
                        return math::isEqual(value - math::roundDown(value), 0.0) ? TO_JSTR(valueInt) : TO_JSTR(value);
                    }
                case json_value_type::string:
                    {
                        std::string value;
                        std::stringstream stream;
                        stream << std::quoted(data->asString().getInternalData());
                        stream >> value;
                        return jstring(value);
                    }
                case json_value_type::array:
                    {
                        jstring strData;
                        for (const auto& dataValue : data->asArray())
                        {
                            strData += strData.isEmpty() ? '[' : ',';
                            strData += serialize(dataValue);
                        }
                        if (strData.isEmpty())
                        {
                            return "[]";
                        }
                        strData += ']';
                        return strData;
                    }
                case json_value_type::object:
                    {
                        jstring strData;
                        for (const auto& dataValue : data->asObject())
                        {
                            strData += (strData.isEmpty() ? '{' : ',');

                            std::string key;
                            std::stringstream stream;
                            stream << std::quoted(dataValue.key.toString().getInternalData());
                            stream >> key;
                            strData += jstring(key) + ':' + serialize(dataValue.value);
                        }
                        if (strData.isEmpty())
                        {
                            return "{}";
                        }
                        strData += '}';
                        return strData;
                    }
                default: ;
                }
            }
            return "null";
        }

        json_value parse(const jstring& data)
        {
            int32 index;
            json_value value = parse(data, 0, index);
            if (value == nullptr)
            {
                return nullptr;
            }
            index = skipSpaces(data, index);
            if (data.getSize() > index)
            {
                return nullptr;
            }
            return value;
        }
        json_value parseFile(const jstring& filePath)
        {
            std::fstream file(*filePath);
            if (file.is_open())
            {
                jstring data;
                std::string line;
                while (std::getline(file, line))
                {
                    data += line.c_str();
                    data += '\n';
                }
                file.close();
                return parse(data);
            }
            return nullptr;
        }

        int32 skipSpaces(const jstring& data, const int32 startIndex)
        {
            int32 index = startIndex;
            while ((data.getSize() > index) && std::isspace(data[index]))
            {
                index++;
            }
            return index;
        }
        json_value_number parseNumber(const jstring& data, const int32 startIndex, int32& outEndIndex)
        {
            outEndIndex = startIndex + 1;
            while (data.getSize() > outEndIndex)
            {
                if ((data[outEndIndex] >= '0') && (data[outEndIndex] <= '9'))
                {
                    outEndIndex++;
                }
                else if (data[outEndIndex] == '.')
                {
                    if ((data.getSize() <= outEndIndex + 1) || (data[outEndIndex + 1] < '0') || (data[outEndIndex + 1] > '9'))
                    {
                        return nullptr;
                    }
                    outEndIndex += 2;
                }
                else
                {
                    break;
                }
            }
            const jstring value = data.substr(startIndex, outEndIndex - startIndex);
            return createJsonValue<json_value_type::number>(std::atof(*value));
        }
        bool parseString(const jstring& data, const int32 startIndex, int32& outEndIndex, jstring& outValue)
        {
            outValue = jstring();
            outEndIndex = startIndex + 1;
            while (data.getSize() > outEndIndex)
            {
                switch (data[outEndIndex])
                {
                case '"':
                    {
                        outEndIndex++;
                        return true;
                    }
                case '\\':
                    {
                        if (data.getSize() <= outEndIndex + 1)
                        {
                            return false;
                        }
                        switch (data[outEndIndex + 1])
                        {
                        case '\\': outValue += '\\'; break;
                        case '"':  outValue +=  '"'; break;
                        case 'n':  outValue += '\n'; break;
                        case 't':  outValue += '\t'; break;
                        default: return false;
                        }
                        outEndIndex += 2;
                    }
                    break;
                default:
                    outValue += data[outEndIndex];
                    outEndIndex++;
                    break;
                }
            }
            return false;
        }
        json_value_string parseString(const jstring& data, const int32 startIndex, int32& outEndIndex)
        {
            jstring value;
            if (!parseString(data, startIndex, outEndIndex, value))
            {
                return nullptr;
            }
            return createJsonValue<json_value_type::string>(value);
        }
        json_value_array parseArray(const jstring& data, const int32 startIndex, int32& outEndIndex)
        {
            int32 index = skipSpaces(data, startIndex + 1);
            if (data.getSize() <= index)
            {
                return nullptr;
            }
            if (data[index] == ']')
            {
                outEndIndex = index + 1;
                return createJsonValue<json_value_type::array>();
            }

            json_value_array valueArray = createJsonValue<json_value_type::array>();
            while (true)
            {
                int32 endIndex;
                const json_value& value = valueArray->get().add(parse(data, index, endIndex));
                if (value == nullptr)
                {
                    return nullptr;
                }
                index = skipSpaces(data, endIndex);
                if (data.getSize() <= index)
                {
                    return nullptr;
                }

                switch (data[index])
                {
                case ',': index++; break;
                case ']': outEndIndex = index + 1; return valueArray;
                default: return nullptr;
                }
            }
        }
        json_value_object parseObject(const jstring& data, const int32 startIndex, int32& outEndIndex)
        {
            int32 index = skipSpaces(data, startIndex + 1);
            if (data.getSize() <= index)
            {
                return nullptr;
            }
            if (data[index] == '}')
            {
                outEndIndex = index + 1;
                return createJsonValue<json_value_type::object>();
            }

            json_value_object valueObject = createJsonValue<json_value_type::object>();
            while (true)
            {
                index = skipSpaces(data, index);
                if ((data.getSize() <= index) || (data[index] != '\"'))
                {
                    return nullptr;
                }

                jstring key;
                if (!parseString(data, index, index, key))
                {
                    return nullptr;
                }

                index = skipSpaces(data, index);
                if ((data.getSize() <= index) || (data[index] != ':'))
                {
                    return nullptr;
                }
                const json_value& value = valueObject->get().add(key, parse(data, index + 1, index));
                if (value == nullptr)
                {
                    return nullptr;
                }

                index = skipSpaces(data, index);
                if (data.getSize() <= index)
                {
                    return nullptr;
                }
                switch (data[index])
                {
                case ',': index++; continue;
                case '}': outEndIndex = index + 1; return valueObject;
                default: ;
                }

                return nullptr;
            }
        }
        json_value parse(const jstring& data, const int32 startIndex, int32& outEndIndex)
        {
            const int32 index = skipSpaces(data, startIndex);
            if (data.getSize() <= index)
            {
                return nullptr;
            }

            switch (data[index])
            {
            case 't': case 'T':
            case 'n': case 'N':
                if (data.getSize() > index + 3)
                {
                    const jstring v = data.substr(index, 4);
                    if ((v == JSTR("true")) || (v == JSTR("TRUE")))
                    {
                        outEndIndex = index + 4;
                        return createJsonValue<json_value_type::boolean>(true);
                    }
                    if ((v == JSTR("null")) || (v == JSTR("NULL")))
                    {
                        outEndIndex = index + 4;
                        return createJsonValue<json_value_type::null>();
                    }
                }
                return nullptr;
            case 'f': case 'F':
                if (data.getSize() > index + 4)
                {
                    const jstring v = data.substr(index, 5);
                    if ((v == JSTR("false")) || (v == JSTR("FALSE")))
                    {
                        outEndIndex = index + 5;
                        return createJsonValue<json_value_type::boolean>(false);
                    }
                }
                return nullptr;

            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '-':
                return parseNumber(data, index, outEndIndex);
            case '"': return parseString(data, index, outEndIndex);
            case '[': return parseArray(data, index, outEndIndex);
            case '{': return parseObject(data, index, outEndIndex);

            default: ;
            }
            return nullptr;
        }
    }
}
