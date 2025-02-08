// Copyright © 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#ifndef JUTILS_MODULE
    #include "delegate.h"
    #include "vector.h"
#endif

JUTILS_MODULE_EXPORT namespace jutils
{
    template<typename... Args>
    class delegate_multicast
    {
    public:
        delegate_multicast() = default;
        delegate_multicast(const delegate_multicast& value)
            : _delegates(value._delegates)
        {
            _clearPendingDeleteEntries();
        }
        delegate_multicast(delegate_multicast&& value) noexcept
            : _delegates(std::move(value._delegates))
            , _callCounter(value._callCounter)
        {
            value._callCounter = 0;
        }

        delegate_multicast& operator=(const delegate_multicast& value)
        {
            if (this != &value)
            {
                _delegates = value._delegates;
                _clearPendingDeleteEntries();
            }
            return *this;
        }
        delegate_multicast& operator=(delegate_multicast&& value) noexcept
        {
            _delegates = std::move(value._delegates);
            _callCounter = value._callCounter;
            value._callCounter = 0;
            return *this;
        }

        template<typename T>
        void bind(T* object, void (T::*function)(Args...))
        {
            if ((object != nullptr) && (function != nullptr))
            {
                auto* entry = _findDelegateEntry(object, function);
                if (entry == nullptr)
                {
                    _delegates.addDefault().delegate.bind(object, function);
                }
                else if (entry->pendingDelete)
                {
                    entry->pendingDelete = false;
                }
            }
        }
        void bind(const std::function<void(Args...)>& function)
        {
            if (function != nullptr)
            {
                auto* entry = _findDelegateEntry(function);
                if (entry == nullptr)
                {
                    _delegates.addDefault().delegate.bind(function);
                }
                else if (entry->pendingDelete)
                {
                    entry->pendingDelete = false;
                }
            }
        }
        void bind(std::function<void(Args...)>&& function)
        {
            if (function != nullptr)
            {
                auto* entry = _findDelegateEntry(function);
                if (entry == nullptr)
                {
                    _delegates.addDefault().delegate.bind(std::move(function));
                }
                else if (entry->pendingDelete)
                {
                    entry->pendingDelete = false;
                }
            }
        }

        template<typename T>
        [[nodiscard]] bool isBinded(T* object, void (T::*function)(Args...)) const
        {
            if ((object == nullptr) || (function == nullptr))
            {
                return false;
            }
            const auto* entry = _findDelegateEntry(object, function);
            return (entry != nullptr) && !entry->pendingDelete;
        }
        [[nodiscard]] bool isBinded(const std::function<void(Args...)>& function) const
        {
            if (function == nullptr)
            {
                return false;
            }
            const auto* entry = _findDelegateEntry(function);
            return (entry != nullptr) && !entry->pendingDelete;
        }

        template<typename T>
        void unbind(T* object, void (T::*callback)(Args...))
        {
            if (object != nullptr)
            {
                for (index_type index = 0; index < _delegates.getSize(); index++)
                {
                    if (_delegates[index].delegate.isBinded(object, callback))
                    {
                        if (_callCounter == 0)
                        {
                            _delegates.removeAt(index);
                        }
                        else
                        {
                            _delegates[index].pendingDelete = true;
                        }
                        break;
                    }
                }
            }
        }
        void unbind(const std::function<void(Args...)>& function)
        {
            if (function != nullptr)
            {
                for (index_type index = 0; index < _delegates.getSize(); index++)
                {
                    if (_delegates[index].delegate.isBinded(function))
                    {
                        if (_callCounter == 0)
                        {
                            _delegates.removeAt(index);
                        }
                        else
                        {
                            _delegates[index].pendingDelete = true;
                        }
                        break;
                    }
                }
            }
        }

        void clear() { _delegates.clear(); }

        void call(Args... args) const
        {
            _callCounter++;
            for (const auto& entry : _delegates)
            {
                if (!entry.pendingDelete)
                {
                    entry.delegate.call(std::forward<Args>(args)...);
                }
            }
            _callCounter--;
            if (_callCounter == 0)
            {
                _clearPendingDeleteEntries();
            }
        }

    private:

        struct delegate_entry
        {
            delegate<Args...> delegate;
            bool pendingDelete = false;
        };

        mutable vector<delegate_entry> _delegates;
        mutable uint32 _callCounter = 0;


        template<typename T>
        [[nodiscard]] delegate_entry* _findDelegateEntry(T* object, void (T::*function)(Args...)) const
        {
            for (delegate_entry& entry : _delegates)
            {
                if (entry.delegate.isBinded(object, function))
                {
                    return &entry;
                }
            }
            return nullptr;
        }
        [[nodiscard]] delegate_entry* _findDelegateEntry(const std::function<void(Args...)>& function) const
        {
            for (delegate_entry& entry : _delegates)
            {
                if (entry.delegate.isBinded(function))
                {
                    return &entry;
                }
            }
            return nullptr;
        }
        void _clearPendingDeleteEntries() const { _delegates.remove([](const delegate_entry& entry){ return entry.pendingDelete; }); }
    };
}