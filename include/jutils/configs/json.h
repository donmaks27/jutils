// Copyright © 2022-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../jarray.h"
#include "../jmap.h"
#include "../jstringID.h"

#include <memory>

namespace jutils::json
{
    enum class type : uint8 { null, boolean, number, string, array, object };

    class value_base;
    class value_base_null;
    class value_base_boolean;
    class value_base_number;
    class value_base_string;
    class value_base_array;
    class value_base_object;

    using value         = std::shared_ptr<json::value_base>;
    using value_null    = std::shared_ptr<json::value_base_null>;
    using value_boolean = std::shared_ptr<json::value_base_boolean>;
    using value_number  = std::shared_ptr<json::value_base_number>;
    using value_string  = std::shared_ptr<json::value_base_string>;
    using value_array   = std::shared_ptr<json::value_base_array>;
    using value_object  = std::shared_ptr<json::value_base_object>;

    template<type Type>
    struct value_info : std::false_type {};
    template<> struct value_info<json::type::null>    : std::true_type { using json_type = json::value_null; };
    template<> struct value_info<json::type::boolean> : std::true_type { using json_type = json::value_boolean; };
    template<> struct value_info<json::type::number>  : std::true_type { using json_type = json::value_number; };
    template<> struct value_info<json::type::string>  : std::true_type { using json_type = json::value_string; };
    template<> struct value_info<json::type::array>   : std::true_type { using json_type = json::value_array; };
    template<> struct value_info<json::type::object>  : std::true_type { using json_type = json::value_object; };

    template<json::type Type, typename... Args>
    typename json::value_info<Type>::json_type createValue(Args&&... args)
    {
        return std::make_shared<typename json::value_info<Type>::json_type::element_type>(std::forward<Args>(args)...);
    }
    template<json::type Type>
    typename json::value_info<Type>::json_type castValue(const json::value& value)
    {
        return std::dynamic_pointer_cast<typename json::value_info<Type>::json_type::element_type>(value);
    }

    class value_base
    {
    protected:
        value_base() = default;
        value_base(const json::type type)
            : _type(type)
        {}
    public:
        virtual ~value_base() = default;

        json::type getType() const { return _type; }
        bool isNull() const { return getType() == json::type::null; }

        virtual bool asBool() const { return false; }
        virtual double asNumber() const { return 0.0; }
        virtual const jstring& asString() const
        {
            static jstring emptyString = {};
            return emptyString;
        }
        virtual const jarray<json::value>& asArray() const
        {
            static jarray<json::value> emptyArray = {};
            return emptyArray;
        }
        virtual const jmap<jstringID, json::value>& asObject() const
        {
            static jmap<jstringID, json::value> emptyObject = {};
            return emptyObject;
        }

        bool tryGetBool(bool& outValue) const
        {
            if (getType() != json::type::boolean)
            {
                return false;
            }
            outValue = asBool();
            return true;
        }
        bool tryGetNumber(double& outValue) const
        {
            if (getType() != json::type::number)
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
            if (getType() != json::type::string)
            {
                return false;
            }
            outValue = asString();
            return true;
        }
        bool tryGetArray(const jarray<json::value>*& outValue) const
        {
            if (getType() != json::type::array)
            {
                return false;
            }
            outValue = &asArray();
            return true;
        }
        bool tryGetObject(const jmap<jstringID, json::value>*& outValue) const
        {
            if (getType() != json::type::object)
            {
                return false;
            }
            outValue = &asObject();
            return true;
        }

    private:

        json::type _type = json::type::null;
    };

    class value_base_null final : public json::value_base
    {
    public:
        value_base_null()
            : json::value_base(json::type::null)
        {}
        virtual ~value_base_null() override = default;
    };

    class value_base_boolean final : public json::value_base
    {
    public:
        value_base_boolean()
            : value_base_boolean(false)
        {}
        value_base_boolean(const bool value)
            : json::value_base(json::type::boolean)
            , _value(value)
        {}
        virtual ~value_base_boolean() override = default;

        virtual bool asBool() const override { return get(); }

        bool get() const { return _value; }
        void set(const bool newValue) { _value = newValue; }

    private:

        bool _value = false;
    };

    class value_base_number final : public json::value_base
    {
    public:
        value_base_number()
            : value_base_number(0.0)
        {}
        value_base_number(const double value)
            : json::value_base(json::type::number)
            , _value(value)
        {}
        value_base_number(const int32 value)
            : value_base_number(static_cast<double>(value))
        {}
        value_base_number(const uint32 value)
            : value_base_number(static_cast<double>(value))
        {}
        virtual ~value_base_number() override = default;

        virtual double asNumber() const override { return get(); }

        double get() const { return _value; }
        void set(const double newValue) { _value = newValue; }
        void set(const int32 newValue) { set(static_cast<double>(newValue)); }
        void set(const uint32 newValue) { set(static_cast<double>(newValue)); }

    private:

        double _value = 0.0;
    };

    class value_base_string final : public json::value_base
    {
    public:
        value_base_string()
            : json::value_base(json::type::string)
        {}
        value_base_string(const jstring& value)
            : json::value_base(json::type::string)
            , _value(value)
        {}
        value_base_string(jstring&& value) noexcept
            : json::value_base(json::type::string)
            , _value(std::move(value))
        {}
        virtual ~value_base_string() override = default;

        virtual const jstring& asString() const override { return get(); }

        const jstring& get() const { return _value; }
        void set(const jstring& newValue) { _value = newValue; }
        void set(jstring&& newValue) { _value = std::move(newValue); }

    private:

        jstring _value;
    };

    class value_base_array final : public json::value_base
    {
    public:
        value_base_array()
            : json::value_base(json::type::array)
        {}
        virtual ~value_base_array() override = default;

        virtual const jarray<json::value>& asArray() const override { return get(); }

        jarray<json::value>& get() { return _value; }
        const jarray<json::value>& get() const { return _value; }

    private:

        jarray<json::value> _value;
    };

    class value_base_object final : public json::value_base
    {
    public:
        value_base_object()
            : json::value_base(json::type::object)
        {}
        virtual ~value_base_object() override = default;

        virtual const jmap<jstringID, json::value>& asObject() const override { return get(); }

        jmap<jstringID, json::value>& get() { return _value; }
        const jmap<jstringID, json::value>& get() const { return _value; }

    private:

        jmap<jstringID, json::value> _value;
    };
}
