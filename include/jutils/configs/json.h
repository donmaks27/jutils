// Copyright © 2022-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../jarray.h"
#include "../jmap.h"
#include "../jstringID.h"

#include <memory>

namespace jutils
{
    namespace json
    {
        enum class json_value_type : uint8 { null, boolean, number, string, array, object };

        class json_value_base;
        class json_value_base_null;
        class json_value_base_boolean;
        class json_value_base_number;
        class json_value_base_string;
        class json_value_base_array;
        class json_value_base_object;

        using json_value = std::shared_ptr<json_value_base>;
        using json_value_null = std::shared_ptr<json_value_base_null>;
        using json_value_boolean = std::shared_ptr<json_value_base_boolean>;
        using json_value_number = std::shared_ptr<json_value_base_number>;
        using json_value_string = std::shared_ptr<json_value_base_string>;
        using json_value_array = std::shared_ptr<json_value_base_array>;
        using json_value_object = std::shared_ptr<json_value_base_object>;

        template<json_value_type Type>
        struct json_value_info : std::false_type {};
        template<>
        struct json_value_info<json_value_type::null> : std::true_type { using json_type = json_value_null; };
        template<>
        struct json_value_info<json_value_type::boolean> : std::true_type { using json_type = json_value_boolean; };
        template<>
        struct json_value_info<json_value_type::number> : std::true_type { using json_type = json_value_number; };
        template<>
        struct json_value_info<json_value_type::string> : std::true_type { using json_type = json_value_string; };
        template<>
        struct json_value_info<json_value_type::array> : std::true_type { using json_type = json_value_array; };
        template<>
        struct json_value_info<json_value_type::object> : std::true_type { using json_type = json_value_object; };

        template<json_value_type Type, typename... Args>
        typename json_value_info<Type>::json_type createJsonValue(Args&&... args)
        {
            return std::make_shared<typename json_value_info<Type>::json_type::element_type>(std::forward<Args>(args)...);
        }
        template<json_value_type Type>
        typename json_value_info<Type>::json_type castJsonValue(const json_value& value)
        {
            return std::dynamic_pointer_cast<typename json_value_info<Type>::json_type::element_type>(value);
        }

        class json_value_base
        {
        protected:
            json_value_base() = default;
            json_value_base(const json_value_type type)
                : valueType(type)
            {}
        public:
            virtual ~json_value_base() = default;

            json_value_type getType() const { return valueType; }

            bool isNull() const { return getType() == json_value_type::null; }

            virtual bool asBool() const { return false; }
            virtual double asNumber() const { return 0.0; }
            virtual const jstring& asString() const
            {
                static jstring emptyString = {};
                return emptyString;
            }
            virtual const jarray<json_value>& asArray() const
            {
                static jarray<json_value> emptyArray = {};
                return emptyArray;
            }
            virtual const jmap<jstringID, json_value>& asObject() const
            {
                static jmap<jstringID, json_value> emptyObject = {};
                return emptyObject;
            }

            bool tryGetBool(bool& outValue) const
            {
                if (getType() != json_value_type::boolean)
                {
                    return false;
                }
                outValue = asBool();
                return true;
            }
            bool tryGetNumber(double& outValue) const
            {
                if (getType() != json_value_type::number)
                {
                    return false;
                }
                outValue = asNumber();
                return true;
            }
            bool tryGetNumber(int32& outValue) const
            {
                double value;
                if (!tryGetNumber(value))
                {
                    return false;
                }
                outValue = static_cast<int32>(value);
                return true;
            }
            bool tryGetNumber(uint32& outValue) const
            {
                double value;
                if (!tryGetNumber(value))
                {
                    return false;
                }
                outValue = static_cast<uint32>(value);
                return true;
            }
            bool tryGetString(jstring& outValue) const
            {
                if (getType() != json_value_type::string)
                {
                    return false;
                }
                outValue = asString();
                return true;
            }
            bool tryGetArray(const jarray<json_value>*& outValue) const
            {
                if (getType() != json_value_type::array)
                {
                    return false;
                }
                outValue = &asArray();
                return true;
            }
            bool tryGetObject(const jmap<jstringID, json_value>*& outValue) const
            {
                if (getType() != json_value_type::object)
                {
                    return false;
                }
                outValue = &asObject();
                return true;
            }

        private:

            json_value_type valueType = json_value_type::null;
        };

        class json_value_base_null final : public json_value_base
        {
        public:
            json_value_base_null()
                : json_value_base(json_value_type::null)
            {}
            virtual ~json_value_base_null() override = default;
        };

        class json_value_base_boolean final : public json_value_base
        {
        public:
            json_value_base_boolean()
                : json_value_base_boolean(false)
            {}
            json_value_base_boolean(const bool value)
                : json_value_base(json_value_type::boolean)
                , value(value)
            {}
            virtual ~json_value_base_boolean() override = default;

            virtual bool asBool() const override { return get(); }

            bool get() const { return value; }
            void set(const bool newValue) { value = newValue; }

        private:

            bool value = false;
        };

        class json_value_base_number final : public json_value_base
        {
        public:
            json_value_base_number()
                : json_value_base_number(0.0)
            {}
            json_value_base_number(const double value)
                : json_value_base(json_value_type::number)
                , value(value)
            {}
            json_value_base_number(const int32 value)
                : json_value_base_number(static_cast<double>(value))
            {}
            json_value_base_number(const uint32 value)
                : json_value_base_number(static_cast<double>(value))
            {}
            virtual ~json_value_base_number() override = default;

            virtual double asNumber() const override { return get(); }

            double get() const { return value; }
            void set(const double newValue) { value = newValue; }
            void set(const int32 newValue) { set(static_cast<double>(newValue)); }
            void set(const uint32 newValue) { set(static_cast<double>(newValue)); }

        private:

            double value = 0.0;
        };

        class json_value_base_string final : public json_value_base
        {
        public:
            json_value_base_string()
                : json_value_base(json_value_type::string)
            {}
            json_value_base_string(const jstring& value)
                : json_value_base(json_value_type::string)
                , value(value)
            {}
            json_value_base_string(jstring&& value) noexcept
                : json_value_base(json_value_type::string)
                , value(std::move(value))
            {}
            virtual ~json_value_base_string() override = default;

            virtual const jstring& asString() const override { return get(); }

            const jstring& get() const { return value; }
            void set(const jstring& newValue) { value = newValue; }
            void set(jstring&& newValue) { value = std::move(newValue); }

        private:

            jstring value;
        };

        class json_value_base_array final : public json_value_base
        {
        public:
            json_value_base_array()
                : json_value_base(json_value_type::array)
            {}
            virtual ~json_value_base_array() override = default;

            virtual const jarray<json_value>& asArray() const override { return get(); }

            jarray<json_value>& get() { return value; }
            const jarray<json_value>& get() const { return value; }

        private:

            jarray<json_value> value;
        };

        class json_value_base_object final : public json_value_base
        {
        public:
            json_value_base_object()
                : json_value_base(json_value_type::object)
            {}
            virtual ~json_value_base_object() override = default;

            virtual const jmap<jstringID, json_value>& asObject() const override { return get(); }

            jmap<jstringID, json_value>& get() { return value; }
            const jmap<jstringID, json_value>& get() const { return value; }

        private:

            jmap<jstringID, json_value> value;
        };
    }
}
