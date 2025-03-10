// Copyright © 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#include "base_types.h"
#include "type_traits.h"
#include <functional>
#include <utility>

namespace jutils
{
    template<typename... Args>
    class delegate final
    {
    public:

        template<typename T>
        using method_type = void (T::*)(Args...);
        using callable_type = std::function<void(Args...)>;

        delegate() noexcept = default;
        delegate(std::nullptr_t) noexcept {}
        JUTILS_TEMPLATE_CONDITION((std::is_invocable_v<Func, Args...>), typename Func)
        delegate(Func&& function){ _bind(std::forward<Func>(function)); }
        template<typename T>
        delegate(T* object, method_type<T> function) { _bind(object, function); }
        delegate(const delegate& otherDelegate)
        {
            _container = otherDelegate.isValid() ? otherDelegate._container->copy() : nullptr;
        }
        delegate(delegate&& otherDelegate) noexcept
        {
            _container = otherDelegate._container;
            otherDelegate._container = nullptr;
        }
        ~delegate() { clear(); }

        delegate& operator=(std::nullptr_t)
        {
            clear();
            return *this;
        }
        JUTILS_TEMPLATE_CONDITION((std::is_invocable_v<Func, Args...>), typename Func)
        delegate& operator=(Func&& function)
        {
            bind(std::forward<Func>(function));
            return *this;
        }
        delegate& operator=(const delegate& otherDelegate)
        {
            if (this != &otherDelegate)
            {
                clear();
                _container = otherDelegate.isValid() ? otherDelegate._container->copy() : nullptr;
            }
            return *this;
        }
        delegate& operator=(delegate&& otherDelegate) noexcept
        {
            clear();
            _container = otherDelegate._container;
            otherDelegate._container = nullptr;
            return *this;
        }

        [[nodiscard]] bool isValid() const noexcept { return _container != nullptr; }

        JUTILS_TEMPLATE_CONDITION((std::is_function_v<std::remove_pointer_t<Func>>), typename Func)
        [[nodiscard]] bool isBinded(const Func function) const
        {
            if (isValid() && (function != nullptr) && (_container->type == containter_type::Function))
            {
                const auto* container = reinterpret_cast<const container_function*>(_container);
                return container->function == function;
            }
            return false;
        }
        template<typename T>
        [[nodiscard]] bool isBinded(const T* object, const method_type<T> function) const
        {
            if (isValid() && (object != nullptr) && (function != nullptr) && (_container->type == containter_type::Method))
            {
                const auto* container = dynamic_cast<const container_method<T>*>(_container);
                return (container != nullptr) && (container->object == object) && (container->function == function);
            }
            return false;
        }

        [[nodiscard]] bool operator==(const delegate& other) const
        {
            if (!isValid() || !other.isValid())
            {
                return !isValid() && !other.isValid();
            }
            if (_container->type == other._container->type)
            {
                switch (_container->type)
                {
                case containter_type::Function:
                    {
                        const auto* function = reinterpret_cast<const container_function*>(_container)->function;
                        const auto* otherFunction = reinterpret_cast<const container_function*>(other._container)->function;
                        return function != otherFunction;
                    }
                case containter_type::Method:
                    {
                        const auto* container = reinterpret_cast<const container_method_base*>(_container);
                        const auto* otherContainer = reinterpret_cast<const container_method_base*>(other._container);
                        return container->isEquals(otherContainer);
                    }
                default: 
                    return _container == other._container;
                }
            }
            return false;
        }
        [[nodiscard]] bool operator!=(const delegate& other) const { return !operator==(other); }

        JUTILS_TEMPLATE_CONDITION((std::is_function_v<std::remove_pointer_t<Func>>), typename Func)
        void bind(Func function)
        {
            if (function == nullptr)
            {
                clear();
                return;
            }
            if (isValid() && (_container->type == containter_type::Function))
            {
                auto* container = reinterpret_cast<container_function*>(_container);
                if (container->function == function)
                {
                    return;
                }
                if (_container->callCounter == 0)
                {
                    container->function = function;
                    return;
                }
            }
            clear();
            _bind(function);
        }
        template<typename T>
        void bind(T* object, method_type<T> function)
        {
            if ((object == nullptr) || (function == nullptr))
            {
                clear();
                return;
            }
            if (isValid() && (_container->type == containter_type::Method))
            {
                auto* container = dynamic_cast<container_method<T>*>(_container);
                if (container != nullptr)
                {
                    if ((container->object == object) && (container->function == function))
                    {
                        return;
                    }
                    if (_container->callCounter == 0)
                    {
                        container->object = object;
                        container->function = function;
                        return;
                    }
                }
            }
            clear();
            _bind(object, function);
        }
        void bind(callable_type&& function)
        {
            if (function == nullptr)
            {
                clear();
            }
            else if (isValid() && (_container->type == containter_type::Callable) && (_container->callCounter == 0))
            {
                reinterpret_cast<container_callable*>(_container)->function = std::forward<callable_type>(function);
            }
            else
            {
                clear();
                _bind(std::forward<callable_type>(function));
            }
        }

        void clear()
        {
            if (isValid())
            {
                if (_container->callCounter > 0)
                {
                    _container->pendingDelete = true;
                }
                else
                {
                    delete _container;
                }
                _container = nullptr;
            }
        }

        void call(Args... args) const
        {
            if (isValid())
            {
                container* container_copy = _container;
                ++container_copy->callCounter;
                container_copy->call(std::forward<Args>(args)...);
                --container_copy->callCounter;
                if (container_copy->pendingDelete && (container_copy->callCounter == 0))
                {
                    delete container_copy;
                }
            }
        }
        void operator()(Args&&... args) const { call(std::forward<Args>(args)...); }

    private:

        enum class containter_type : uint8 { Function, Method, Callable };

        class container
        {
        protected:
            container(const containter_type t) : type(t) {}
        public:
            virtual ~container() = default;
            
            [[nodiscard]] virtual container* copy() = 0;
            virtual void call(Args...) = 0;

            containter_type type = containter_type::Function;
            bool pendingDelete = false;
            uint16 callCounter = 0;
        };
        class container_function : public container
        {
        public:
            using function_type = void(*)(Args...);

            explicit container_function(const function_type func)
                : container(containter_type::Function), function(func)
            {}
            virtual ~container_function() override = default;
            
            [[nodiscard]] virtual container* copy() override { return new container_function(function); }
            virtual void call(Args... args) override
            {
                if (function != nullptr)
                {
                    function(std::forward<Args>(args)...);
                }
            }

            function_type function = nullptr;
        };
        class container_method_base : public container
        {
        protected:
            container_method_base() : container(containter_type::Method) {}
        public:
            virtual ~container_method_base() override = default;

            [[nodiscard]] virtual bool isEquals(const container_method_base* other) const = 0;
        };
        template<typename T>
        class container_method : public container_method_base
        {
        public:
            container_method(T* obj, method_type<T> func)
                : container_method_base(), object(obj), function(func)
            {}
            virtual ~container_method() override = default;

            [[nodiscard]] virtual bool isEquals(const container_method_base* other) const override
            {
                const container_method* otherCasted = dynamic_cast<const container_method*>(other);
                return (otherCasted != nullptr) && (otherCasted->object == object) && (otherCasted->function == function);
            }
            [[nodiscard]] virtual container* copy() override { return new container_method(object, function); }
            virtual void call(Args... args) override
            {
                if (object != nullptr)
                {
                    (object->*function)(std::forward<Args>(args)...);
                }
            }

            T* object = nullptr;
            method_type<T> function = nullptr;
        };
        class container_callable : public container
        {
        public:
            explicit container_callable(const callable_type& func)
                : container(containter_type::Callable), function(func)
            {}
            explicit container_callable(callable_type&& func)
                : container(containter_type::Callable), function(std::move(func))
            {}
            virtual ~container_callable() override = default;

            [[nodiscard]] virtual container* copy() override { return new container_callable(function); }
            virtual void call(Args... args) override
            {
                if (function != nullptr)
                {
                    function(std::forward<Args>(args)...);
                }
            }

            callable_type function = nullptr;
        };

        container* _container = nullptr;


        JUTILS_TEMPLATE_CONDITION((std::is_function_v<std::remove_pointer_t<Func>>), typename Func)
        void _bind(Func function)
        {
            if (function != nullptr)
            {
                _container = new container_function(function);
            }
        }
        template<typename T>
        void _bind(T* object, method_type<T> function)
        {
            if ((object != nullptr) && (function != nullptr))
            {
                _container = new container_method<T>(object, function);
            }
        }
        void _bind(callable_type&& function)
        {
            if (function != nullptr)
            {
                _container = new container_callable(std::forward<callable_type>(function));
            }
        }
    };
}
