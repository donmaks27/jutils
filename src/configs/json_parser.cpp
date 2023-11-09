// Copyright © 2022-2023 Leonov Maksim. All Rights Reserved.

#include "jutils/configs/json_parser.h"

#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

namespace jutils::json
{
    json::value parse(const nlohmann::json& config);
    nlohmann::json toNlohmannJSON(const json::value& data);

    jstring serialize(const json::value& data, const bool prettyPrint)
    {
        return toNlohmannJSON(data).dump(prettyPrint ? 4 : -1);
    }
    nlohmann::json toNlohmannJSON(const json::value& data)
    {
        if (data != nullptr)
        {
            switch (data->getType())
            {
            case type::boolean: return data->asBool();
            case type::number: return data->asNumber();
            case type::string: return data->asString().toBase();
            case type::array:
                {
                    nlohmann::json config;
                    for (const auto& jsonValue : data->asArray())
                    {
                        config.push_back(toNlohmannJSON(jsonValue));
                    }
                    return config;
                }
            case type::object:
                {
                    nlohmann::json config;
                    for (const auto& jsonPair : data->asObject())
                    {
                        config[jsonPair.first.toString().toBase()] = toNlohmannJSON(jsonPair.second);
                    }
                    return config;
                }
            default: ;
            }
        }
        return nullptr;
    }

    json::value parse(const jstring& data)
    {
        return parse(nlohmann::json::parse(std::istringstream(data.toBase())));
    }
    json::value parseFile(const jstring& filePath)
    {
        std::ifstream file(*filePath);
        if (!file.is_open())
        {
            return nullptr;
        }
        const nlohmann::json config = nlohmann::json::parse(file);
        file.close();
        return parse(config);
    }
    json::value parse(const nlohmann::json& config)
    {
        if (config.is_null())
        {
            return createValue<type::null>();
        }
        if (config.is_boolean())
        {
            return createValue<type::boolean>(config.get<bool>());
        }
        if (config.is_number())
        {
            return createValue<type::number>(config.get<double>());
        }
        if (config.is_string())
        {
            return createValue<type::string>(jstring(config.get<std::string>()));
        }
        if (config.is_array())
        {
            value_array jsonArray = createValue<type::array>();
            for (const auto& arrayValue : config)
            {
                jsonArray->get().add(parse(arrayValue));
            }
            return jsonArray;
        }
        value_object jsonObject = createValue<type::object>();
        for (const auto& [key, value] : config.items())
        {
            jsonObject->get().add(jstring(key), parse(value));
        }
        return jsonObject;
    }
}
