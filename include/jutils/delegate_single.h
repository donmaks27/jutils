// Copyright © 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#include "base_types.h"
#include <functional>
#include <utility>

namespace jutils
{
    template<typename... Args>
    class delegate_single
    {
    public:
        delegate_single() noexcept = default;
        delegate_single(std::nullptr_t) noexcept {}
        template<typename T>
        delegate_single(T* object, void (T::*function)(Args...))
        {
            bind(object, function);
        }
        delegate_single(const std::function<void(Args...)>& function)
        {
            bind(function);
        }
        delegate_single(std::function<void(Args...)>&& function)
        {
            bind(std::move(function));
        }
        delegate_single(const delegate_single& otherDelegate)
        {
            if (otherDelegate.isValid())
            {
                _delegate = otherDelegate._delegate->copy();
            }
        }
        delegate_single(delegate_single&& otherDelegate) noexcept
        {
            if (otherDelegate.isValid())
            {
                _delegate = otherDelegate._delegate;
                otherDelegate._delegate = nullptr;
            }
        }
        ~delegate_single() noexcept { clear(); }

        delegate_single& operator=(std::nullptr_t) noexcept
        {
            clear();
            return *this;
        }
        delegate_single& operator=(const std::function<void(Args...)>& function)
        {
            bind(function);
            return *this;
        }
        delegate_single& operator=(std::function<void(Args...)>&& function)
        {
            bind(std::move(function));
            return *this;
        }
        delegate_single& operator=(const delegate_single& otherDelegate)
        {
            if (this != &otherDelegate)
            {
                clear();
                if (otherDelegate.isValid())
                {
                    _delegate = otherDelegate._delegate->copy();
                }
            }
            return *this;
        }
        delegate_single& operator=(delegate_single&& otherDelegate) noexcept
        {
            clear();
            if (otherDelegate.isValid())
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

        [[nodiscard]] bool isValid() const noexcept { return _delegate != nullptr; }

        template<typename T>
        [[nodiscard]] bool isBinded(T* object, void (T::*function)(Args...)) const
        {
            if (isValid() && (object != nullptr) && (function != nullptr))
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
            if (isValid() && (function != nullptr))
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
            if (isValid())
            {
                if (_delegate->callCounter > 0)
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
            if (isValid())
            {
                container_interface* container = _delegate;
                container->callCounter++;
                container->call(std::forward<Args>(args)...);
                container->callCounter--;
                if (container->pendingDelete)
                {
                    delete container;
                }
            }
        }

    private:

        class container_interface
        {
        public:
            virtual ~container_interface() = default;

            [[nodiscard]] virtual container_interface* copy() = 0;

            virtual void call(Args...) = 0;

            uint32 callCounter = 0;
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

            [[nodiscard]] bool isBinded(class_type* checking_object, function_type checking_function) const
                { return (_object == checking_object) && (_function == checking_function); }

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

            [[nodiscard]] bool isBinded(const std::function<void(Args...)>& checking_function) const
                { return _function == checking_function; }

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
