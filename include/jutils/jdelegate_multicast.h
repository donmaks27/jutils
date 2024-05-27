// Copyright © 2021-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jdelegate.h"
#include "jarray.h"

namespace jutils
{
    template<typename... Args>
    class jdelegate_multicast
    {
    public:
        jdelegate_multicast() = default;
        jdelegate_multicast(const jdelegate_multicast& value)
            : _delegates(value._delegates)
        {
            _clearPendingDeleteEntries();
        }
        jdelegate_multicast(jdelegate_multicast&& value) noexcept
            : _delegates(std::move(value._delegates))
            , _callCounter(value._callCounter)
        {
            value._callCounter = 0;
        }

        jdelegate_multicast& operator=(const jdelegate_multicast& value)
        {
            if (this != &value)
            {
                _delegates = value._delegates;
                _clearPendingDeleteEntries();
            }
            return *this;
        }
        jdelegate_multicast& operator=(jdelegate_multicast&& value) noexcept
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
            jdelegate<Args...> delegate;
            bool pendingDelete = false;
        };

        mutable jarray<delegate_entry> _delegates;
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

#define JUTILS_CREATE_MULTICAST_DELEGATE_INTERNAL(DelegateName, ParamsTypes, ParamsNames, Params)   \
class DelegateName : public jutils::jdelegate_multicast<ParamsTypes>                                \
{                                                                                                   \
    using base_class = jutils::jdelegate_multicast<ParamsTypes>;                                    \
public:                                                                                             \
    DelegateName() : base_class() {}                                                                \
    DelegateName(const base_class& value) : base_class(value) {}                                    \
    DelegateName(base_class&& value) noexcept : base_class(std::move(value)) {}                     \
    DelegateName& operator=(const base_class& value)                                                \
        { base_class::operator=(value); return *this; }                                             \
    DelegateName& operator=(base_class&& value) noexcept                                            \
        { base_class::operator=(std::move(value)); return *this; }                                  \
    void call(Params) const { base_class::call(ParamsNames); }                                      \
}

#define JUTILS_CREATE_MULTICAST_DELEGATE(DelegateName) JUTILS_CREATE_MULTICAST_DELEGATE_INTERNAL(DelegateName, , , )
#define JUTILS_CREATE_MULTICAST_DELEGATE1(DelegateName, ParamType1, ParamName1) JUTILS_CREATE_MULTICAST_DELEGATE_INTERNAL(DelegateName, \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1), JUTILS_DELEGATE_CONCAT_HELPER(ParamName1), \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1 ParamName1))
#define JUTILS_CREATE_MULTICAST_DELEGATE2(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2) JUTILS_CREATE_MULTICAST_DELEGATE_INTERNAL(DelegateName, \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1, ParamType2), JUTILS_DELEGATE_CONCAT_HELPER(ParamName1, ParamName2), \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1 ParamName1, ParamType2 ParamName2))
#define JUTILS_CREATE_MULTICAST_DELEGATE3(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3) JUTILS_CREATE_MULTICAST_DELEGATE_INTERNAL(DelegateName, \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1, ParamType2, ParamType3), JUTILS_DELEGATE_CONCAT_HELPER(ParamName1, ParamName2, ParamName3), \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3))
#define JUTILS_CREATE_MULTICAST_DELEGATE4(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4) JUTILS_CREATE_MULTICAST_DELEGATE_INTERNAL(DelegateName, \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1, ParamType2, ParamType3, ParamType4), JUTILS_DELEGATE_CONCAT_HELPER(ParamName1, ParamName2, ParamName3, ParamName4), \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4))
#define JUTILS_CREATE_MULTICAST_DELEGATE5(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5) JUTILS_CREATE_MULTICAST_DELEGATE_INTERNAL(DelegateName, \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5), JUTILS_DELEGATE_CONCAT_HELPER(ParamName1, ParamName2, ParamName3, ParamName4, ParamName5), \
    JUTILS_DELEGATE_CONCAT_HELPER(ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5))
