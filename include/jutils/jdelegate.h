// Copyright © 2021-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types.h"

#include <functional>
#include <utility>

namespace jutils
{
    template<typename... Args>
    class jdelegate
    {
    public:
        jdelegate() = default;
        template<typename T>
        jdelegate(T* object, void (T::*function)(Args...))
        {
            bind(object, function);
        }
        jdelegate(const std::function<void(Args...)>& function)
        {
            bind(function);
        }
        jdelegate(std::function<void(Args...)>&& function) noexcept
        {
            bind(std::move(function));
        }
        jdelegate(const jdelegate& otherDelegate)
        {
            if (otherDelegate.delegate_container != nullptr)
            {
                delegate_container = otherDelegate.delegate_container->copy();
            }
        }
        jdelegate(jdelegate&& otherDelegate) noexcept
            : delegate_container(otherDelegate.delegate_container)
        {
            otherDelegate.delegate_container = nullptr;
        }
        ~jdelegate() { clear(); }

        jdelegate& operator=(const std::function<void(Args...)>& function)
        {
            bind(function);
            return *this;
        }
        jdelegate& operator=(std::function<void(Args...)>&& function) noexcept
        {
            bind(std::move(function));
            return *this;
        }
        jdelegate& operator=(const jdelegate& otherDelegate)
        {
            if (this != &otherDelegate)
            {
                clear();
                if (otherDelegate.delegate_container != nullptr)
                {
                    delegate_container = otherDelegate.delegate_container->copy();
                }
            }
            return *this;
        }
        jdelegate& operator=(jdelegate&& otherDelegate) noexcept
        {
            clear();
            delegate_container = otherDelegate.delegate_container;
            otherDelegate.delegate_container = nullptr;
            return *this;
        }

        template<typename T>
        void bind(T* object, void (T::*function)(Args...))
        {
            clear();
            if ((object != nullptr) && (function != nullptr))
            {
                delegate_container = new container_class<T>(object, function);
            }
        }
        void bind(const std::function<void(Args...)>& function)
        {
            clear();
            if (function != nullptr)
            {
                delegate_container = new container_function(function);
            }
        }
        void bind(std::function<void(Args...)>&& function)
        {
            clear();
            if (function != nullptr)
            {
                delegate_container = new container_function(std::move(function));
            }
        }

        template<typename T>
        [[nodiscard]] bool isBinded(T* object, void (T::*function)(Args...)) const
        {
            if ((object != nullptr) && (function != nullptr) && (delegate_container != nullptr))
            {
                const auto* container = dynamic_cast<const container_class<T>*>(delegate_container);
                if (container != nullptr)
                {
                    return container->isBinded(object, function);
                }
            }
            return false;
        }
        [[nodiscard]] bool isBinded(const std::function<void(Args...)>& function) const
        {
            if ((function != nullptr) && (delegate_container != nullptr))
            {
                const auto* container = dynamic_cast<const container_function*>(delegate_container);
                if (container != nullptr)
                {
                    return container->isBinded(function);
                }
            }
            return false;
        }

        void clear()
        {
            if (delegate_container != nullptr)
            {
                if (_callCounter > 0)
                {
                    delegate_container->pendingDelete = true;
                }
                else
                {
                    delete delegate_container;
                }
                delegate_container = nullptr;
            }
        }

        void call(Args... args) const
        {
            if (delegate_container != nullptr)
            {
                _callCounter++;
                container_interface* container = delegate_container;
                container->call(std::forward<Args>(args)...);
                if (container->pendingDelete)
                {
                    delete container;
                }
                _callCounter--;
            }
        }

    private:

        class container_interface
        {
        public:
            virtual ~container_interface() = default;

            [[nodiscard]] virtual container_interface* copy() = 0;

            virtual void call(Args...) = 0;

            bool pendingDelete = false;
        };
        template<typename T>
        class container_class : public container_interface
        {
        public:

            using class_type = T;
            using function_type = void (class_type::*)(Args...);

            container_class(class_type* object, function_type func)
                : _object(object)
                , _function(func)
            {}
            virtual ~container_class() override = default;

            [[nodiscard]] virtual container_interface* copy() override { return new container_class(_object, _function); }

            [[nodiscard]] bool isBinded(class_type* checking_object, function_type checking_function) const { return (_object == checking_object) && (_function == checking_function); }

            virtual void call(Args... args) override
            {
                if (_object != nullptr)
                {
                    (_object->*_function)(std::forward<Args>(args)...);
                }
            }

        private:

            T* _object = nullptr;
            function_type _function = nullptr;
        };
        class container_function : public container_interface
        {
        public:
            explicit container_function(const std::function<void(Args...)>& func)
                : _function(func)
            {}
            explicit container_function(std::function<void(Args...)>&& func)
                : _function(std::move(func))
            {}
            virtual ~container_function() override = default;

            [[nodiscard]] virtual container_interface* copy() override { return new container_function(_function); }

            [[nodiscard]] bool isBinded(const std::function<void(Args...)>& checking_function) const { return _function == checking_function; }

            virtual void call(Args... args) override
            {
                if (_function != nullptr)
                {
                    _function(std::forward<Args>(args)...);
                }
            }

        private:

            std::function<void(Args...)> _function = nullptr;
        };

        mutable container_interface* delegate_container = nullptr;
        mutable uint32 _callCounter = 0;
    };
}

#define JUTILS_DELEGATE_CONCAT_HELPER(...) __VA_ARGS__

#define JUTILS_CREATE_DELEGATE_INTERNAL(DelegateName, ParamsTypes, ParamsNames, Params)         \
class DelegateName : public jutils::jdelegate<ParamsTypes>                                      \
{                                                                                               \
    using base_class = jutils::jdelegate<ParamsTypes>;                                          \
public:                                                                                         \
    DelegateName() : base_class() {}                                                            \
    template<typename T>                                                                        \
    DelegateName(T* object, void (T::*function)(ParamsTypes)) : base_class(object, function) {} \
    DelegateName(void (*function)(ParamsTypes)) : base_class(function) {}                       \
    DelegateName(const base_class& value) : base_class(value) {}                                \
    DelegateName(base_class&& value) noexcept : base_class(std::move(value)) {}                 \
    DelegateName& operator=(const base_class& value)                                            \
        { base_class::operator=(value); return *this; }                                         \
    DelegateName& operator=(base_class&& value) noexcept                                        \
        { base_class::operator=(std::move(value)); return *this; }                              \
    void call(Params) const { base_class::call(ParamsNames); }                                  \
}

#define JUTILS_CREATE_DELEGATE(DelegateName) JUTILS_CREATE_DELEGATE_INTERNAL(DelegateName, , , )
#define JUTILS_CREATE_DELEGATE1(DelegateName, ParamType1, ParamName1) JUTILS_CREATE_DELEGATE_INTERNAL(DelegateName, \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1), JUTILS_DELEGATE_CONCAT_HELPER(ParamName1), \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1 ParamName1))
#define JUTILS_CREATE_DELEGATE2(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2) JUTILS_CREATE_DELEGATE_INTERNAL(DelegateName, \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1, ParamType2), JUTILS_DELEGATE_CONCAT_HELPER(ParamName1, ParamName2), \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1 ParamName1, ParamType2 ParamName2))
#define JUTILS_CREATE_DELEGATE3(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3) JUTILS_CREATE_DELEGATE_INTERNAL(DelegateName, \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1, ParamType2, ParamType3), JUTILS_DELEGATE_CONCAT_HELPER(ParamName1, ParamName2, ParamName3), \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3))
#define JUTILS_CREATE_DELEGATE4(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4) JUTILS_CREATE_DELEGATE_INTERNAL(DelegateName, \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1, ParamType2, ParamType3, ParamType4), JUTILS_DELEGATE_CONCAT_HELPER(ParamName1, ParamName2, ParamName3, ParamName4), \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4))
#define JUTILS_CREATE_DELEGATE5(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5) JUTILS_CREATE_DELEGATE_INTERNAL(DelegateName, \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5), JUTILS_DELEGATE_CONCAT_HELPER(ParamName1, ParamName2, ParamName3, ParamName4, ParamName5), \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5))
