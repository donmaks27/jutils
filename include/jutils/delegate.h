// Copyright © 2021-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#ifndef JUTILS_MODULE
    #include "base_types.h"
    #include "macro/delegate.h"
    #include <functional>
    #include <utility>
#endif

JUTILS_MODULE_EXPORT namespace jutils
{
    template<typename... Args>
    class delegate
    {
    public:
        delegate() = default;
        template<typename T>
        delegate(T* object, void (T::*function)(Args...))
        {
            bind(object, function);
        }
        delegate(const std::function<void(Args...)>& function)
        {
            bind(function);
        }
        delegate(std::function<void(Args...)>&& function) noexcept
        {
            bind(std::move(function));
        }
        delegate(const delegate& otherDelegate)
        {
            if ((otherDelegate._delegate != nullptr) && !otherDelegate._delegate->pendingDelete)
            {
                _delegate = otherDelegate._delegate->copy();
            }
        }
        delegate(delegate&& otherDelegate) noexcept
        {
            if ((otherDelegate._delegate != nullptr) && !otherDelegate._delegate->pendingDelete)
            {
                _delegate = otherDelegate._delegate;
                otherDelegate._delegate = nullptr;
            }
        }
        ~delegate() { clear(); }

        delegate& operator=(const std::function<void(Args...)>& function)
        {
            bind(function);
            return *this;
        }
        delegate& operator=(std::function<void(Args...)>&& function) noexcept
        {
            bind(std::move(function));
            return *this;
        }
        delegate& operator=(const delegate& otherDelegate)
        {
            if (this != &otherDelegate)
            {
                clear();
                if ((otherDelegate._delegate != nullptr) && !otherDelegate._delegate->pendingDelete)
                {
                    _delegate = otherDelegate._delegate->copy();
                }
            }
            return *this;
        }
        delegate& operator=(delegate&& otherDelegate) noexcept
        {
            clear();
            if ((otherDelegate._delegate != nullptr) && !otherDelegate._delegate->pendingDelete)
            {
                _delegate = otherDelegate._delegate;
                otherDelegate._delegate = nullptr;
            }
            return *this;
        }

        template<typename T>
        void bind(T* object, void (T::*function)(Args...))
        {
            clear();
            if ((object != nullptr) && (function != nullptr))
            {
                _delegate = new container_class<T>(object, function);
            }
        }
        void bind(const std::function<void(Args...)>& function)
        {
            clear();
            if (function != nullptr)
            {
                _delegate = new container_function(function);
            }
        }
        void bind(std::function<void(Args...)>&& function)
        {
            clear();
            if (function != nullptr)
            {
                _delegate = new container_function(std::move(function));
            }
        }

        template<typename T>
        [[nodiscard]] bool isBinded(T* object, void (T::*function)(Args...)) const
        {
            if ((object != nullptr) && (function != nullptr) && (_delegate != nullptr))
            {
                const auto* container = dynamic_cast<const container_class<T>*>(_delegate);
                if (container != nullptr)
                {
                    return container->isBinded(object, function);
                }
            }
            return false;
        }
        [[nodiscard]] bool isBinded(const std::function<void(Args...)>& function) const
        {
            if ((function != nullptr) && (_delegate != nullptr))
            {
                const auto* container = dynamic_cast<const container_function*>(_delegate);
                if (container != nullptr)
                {
                    return container->isBinded(function);
                }
            }
            return false;
        }

        void clear()
        {
            if (_delegate != nullptr)
            {
                if (_callCounter > 0)
                {
                    _delegate->pendingDelete = true;
                }
                else
                {
                    delete _delegate;
                }
                _delegate = nullptr;
            }
        }

        void call(Args... args) const
        {
            if (_delegate != nullptr)
            {
                _callCounter++;
                container_interface* container = _delegate;
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

        mutable container_interface* _delegate = nullptr;
        mutable uint32 _callCounter = 0;
    };
}
