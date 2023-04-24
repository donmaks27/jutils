// Copyright © 2022-2023 Leonov Maksim. All Rights Reserved.

#include "jutils/configs/json_parser.h"

#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

namespace jutils
{
    namespace json
    {
        json_value parse(const nlohmann::json& config);
        nlohmann::json toNlohmannJSON(const json_value& data);

        jstring serialize(const json_value& data)
        {
            return jstring(toNlohmannJSON(data).dump());
        }
        nlohmann::json toNlohmannJSON(const json_value& data)
        {
            if (data != nullptr)
            {
                switch (data->getType())
                {
                case json_value_type::boolean: return data->asBool();
                case json_value_type::number: return data->asNumber();
                case json_value_type::string: return data->asString().toBase();
                case json_value_type::array:
                    {
                        nlohmann::json config;
                        for (const auto& jsonValue : data->asArray())
                        {
                            config.push_back(toNlohmannJSON(jsonValue));
                        }
                        return config;
                    }
                case json_value_type::object:
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

        json_value parse(const jstring& data)
        {
            return parse(nlohmann::json::parse(std::istringstream(data.toBase())));
        }
        json_value parseFile(const jstring& filePath)
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
        json_value parse(const nlohmann::json& config)
        {
            if (config.is_null())
            {
                return createJsonValue<json_value_type::null>();
            }
            if (config.is_boolean())
            {
                return createJsonValue<json_value_type::boolean>(config.get<bool>());
            }
            if (config.is_number())
            {
                return createJsonValue<json_value_type::number>(config.get<double>());
            }
            if (config.is_string())
            {
                return createJsonValue<json_value_type::string>(jstring(config.get<std::string>()));
            }
            if (config.is_array())
            {
                json_value_array jsonArray = createJsonValue<json_value_type::array>();
                for (const auto& arrayValue : config)
                {
                    jsonArray->get().add(parse(arrayValue));
                }
                return jsonArray;
            }
            json_value_object jsonObject = createJsonValue<json_value_type::object>();
            for (const auto& [key, value] : config.items())
            {
                jsonObject->get().add(jstring(key), parse(value));
            }
            return jsonObject;
        }
    }
}
