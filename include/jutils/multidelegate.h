// Copyright © 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#include "delegate.h"
#include "uid.h"
#include <algorithm>
#include <vector>

namespace jutils
{
    using delegate_id = uint32;
    constexpr delegate_id invalid_delegate_id = uid<delegate_id>::invalidUID;

    template<typename... Args>
    class multidelegate
    {
        using delegate_type = delegate<Args...>;

    protected:
        multidelegate() = default;
    public:
        multidelegate(const multidelegate&) = delete;
        multidelegate(multidelegate&&) = delete;
        ~multidelegate() = default;

        multidelegate& operator=(const multidelegate&) = delete;
        multidelegate& operator=(multidelegate&&) = delete;

        template<typename T>
        delegate_id bind(T* object, void (T::*function)(Args...)) const
        {
            if ((object != nullptr) && (function != nullptr) && !isBinded(object, function))
            {
                const delegate_id id = _idGenerator.generateUID();
                _delegates.emplace_back(delegate_type{ object, function }, id);
                return id;
            }
            return invalid_delegate_id;
        }
        delegate_id bind(const std::function<void(Args...)>& function) const
        {
            if ((function != nullptr) && !isBinded(function))
            {
                const delegate_id id = _idGenerator.generateUID();
                _delegates.emplace_back(delegate_type{ function }, id);
                return id;
            }
            return invalid_delegate_id;
        }
        delegate_id bind(std::function<void(Args...)>&& function) const
        {
            if ((function != nullptr) && !isBinded(function))
            {
                const delegate_id id = _idGenerator.generateUID();
                _delegates.emplace_back(delegate_type{ std::move(function) }, id);
                return id;
            }
            return invalid_delegate_id;
        }

        template<typename T>
        [[nodiscard]] bool isBinded(T* object, void (T::*function)(Args...)) const
        {
            return _findDelegate(object, function) != invalid_index;
        }
        [[nodiscard]] bool isBinded(const std::function<void(Args...)>& function) const
        {
            return _findDelegate(function) != invalid_index;
        }
        [[nodiscard]] bool isBinded(const delegate_id id) const
        {
            return _findDelegate(id) != invalid_index;
        }

        template<typename T>
        void unbind(T* object, void (T::*function)(Args...)) const { _unbind(_findDelegate(object, function)); }
        void unbind(const std::function<void(Args...)>& function) const { _unbind(_findDelegate(function)); }
        void unbind(const delegate_id id) const { _unbind(_findDelegate(id)); }

        void clear() const
        {
            if (_callCounter > 0)
            {
                for (auto& entry : _delegates)
                {
                    entry.multidelegate.clear();
                }
            }
            else
            {
                _delegates.clear();
            }
        }

    protected:

        void _call(Args... args) const
        {
            if (!_delegates.empty())
            {
                _callCounter++;
                const std::size_t lastIndex = _delegates.size() - 1;
                for (std::size_t index = 0; index < lastIndex; index++)
                {
                    _delegates[index].multidelegate.call(std::forward<Args>(args)...);
                }
                _callCounter--;
                if (_callCounter == 0)
                {
                    _clearInvalidDelegates();
                }
            }
        }

    private:

        struct delegate_entry
        {
            delegate_type delegate;
            delegate_id id = invalid_delegate_id;
        };

        mutable std::vector<delegate_entry> _delegates;
        mutable uid<delegate_id> _idGenerator;
        mutable uint16 _callCounter = 0;


        template<typename T>
        [[nodiscard]] std::size_t _findDelegate(T* object, void (T::*function)(Args...)) const
        {
            const auto iter = (object != nullptr) && (function != nullptr) ? std::find_if(_delegates.begin(), _delegates.end(), [object, function](const delegate_entry& entry) {
                return entry.multidelegate.isBinded(object, function);
            }) : _delegates.end();
            return iter != _delegates.end() ? std::distance(_delegates.begin(), iter) : invalid_index;
        }
        [[nodiscard]] std::size_t _findDelegate(const std::function<void(Args...)>& function) const
        {
            const auto iter = (function != nullptr) ? std::find_if(_delegates.begin(), _delegates.end(), [&function](const delegate_entry& entry) {
                return entry.multidelegate.isBinded(function);
            }) : _delegates.end();
            return iter != _delegates.end() ? std::distance(_delegates.begin(), iter) : invalid_index;
        }
        [[nodiscard]] std::size_t _findDelegate(const delegate_id id) const
        {
            const auto iter = (id != invalid_delegate_id) ? std::find_if(_delegates.begin(), _delegates.end(), [id](const delegate_entry& entry) {
                return entry.id == id;
            }) : _delegates.end();
            return iter != _delegates.end() ? std::distance(_delegates.begin(), iter) : invalid_index;
        }

        void _unbind(const std::size_t index) const
        {
            if (index != invalid_index)
            {
                if (_callCounter == 0)
                {
                    _delegates.erase(std::next(_delegates.begin(), index));
                }
                else
                {
                    _delegates[index].multidelegate.clear();
                }
            }
        }
        void _clearInvalidDelegates() const
        {
#if JUTILS_STD_VERSION >= JUTILS_STD20
            std::erase_if(_delegates, [](const delegate_entry& entry) {
                return !entry.multidelegate.isValid();
            });
#else
            const auto iter = std::remove_if(_delegates.begin(), _delegates.end(), [](const delegate_entry& entry) {
                return !entry.delegate.isValid();
            });
            _delegates.erase(iter, _delegates.end());
#endif
        }
    };
}



#define JUTILS_DELEGATE_HELPER(DelegateName, ParamTypes, ParamNames, Params)    \
    class DelegateName : public jutils::multidelegate<ParamTypes>               \
    {                                                                           \
        using base_class = jutils::multidelegate<ParamTypes>;                   \
    public:                                                                     \
        DelegateName() = default;                                               \
        DelegateName(const DelegateName&) = delete;                             \
        DelegateName(DelegateName&&) = delete;                                  \
        ~DelegateName() = default;                                              \
        DelegateName& operator=(const DelegateName&) = delete;                  \
        DelegateName& operator=(DelegateName&&) = delete;                       \
        void call(Params) const { _call(ParamNames); }                          \
    }



#define JUTILS_DELEGATE(DelegateName) JUTILS_DELEGATE_HELPER(DelegateName, , , )

#define JUTILS_DELEGATE1(DelegateName, ArgType1, ArgName1)  \
    JUTILS_DELEGATE_HELPER(DelegateName,                    \
        JUTILS_HELPER_CONCAT(ArgType1),                     \
        JUTILS_HELPER_CONCAT(ArgName1),                     \
        JUTILS_HELPER_CONCAT(ArgType1 ArgName1)             \
    )

#define JUTILS_DELEGATE2(DelegateName, ArgType1, ArgName1, ArgType2, ArgName2)  \
    JUTILS_DELEGATE_HELPER(DelegateName,                                        \
        JUTILS_HELPER_CONCAT(ArgType1, ArgType2),                               \
        JUTILS_HELPER_CONCAT(ArgName1, ArgName2),                               \
        JUTILS_HELPER_CONCAT(ArgType1 ArgName1, ArgType2 ArgName2)              \
    )

#define JUTILS_DELEGATE3(DelegateName, ArgType1, ArgName1, ArgType2, ArgName2, ArgType3, ArgName3)  \
    JUTILS_DELEGATE_HELPER(DelegateName,                                                            \
        JUTILS_HELPER_CONCAT(ArgType1, ArgType2, ArgType3),                                         \
        JUTILS_HELPER_CONCAT(ArgName1, ArgName2, ArgName3),                                         \
        JUTILS_HELPER_CONCAT(ArgType1 ArgName1, ArgType2 ArgName2, ArgType3 ArgName3)               \
    )

#define JUTILS_DELEGATE4(DelegateName, ArgType1, ArgName1, ArgType2, ArgName2, ArgType3, ArgName3, ArgType4, ArgName4)  \
    JUTILS_DELEGATE_HELPER(DelegateName,                                                                                \
        JUTILS_HELPER_CONCAT(ArgType1, ArgType2, ArgType3, ArgType4),                                                   \
        JUTILS_HELPER_CONCAT(ArgName1, ArgName2, ArgName3, ArgName4),                                                   \
        JUTILS_HELPER_CONCAT(ArgType1 ArgName1, ArgType2 ArgName2, ArgType3 ArgName3, ArgType4 ArgName4)                \
    )

#define JUTILS_DELEGATE5(DelegateName, ArgType1, ArgName1, ArgType2, ArgName2, ArgType3, ArgName3, ArgType4, ArgName4, ArgType5, ArgName5)  \
    JUTILS_DELEGATE_HELPER(DelegateName,                                                                                                    \
        JUTILS_HELPER_CONCAT(ArgType1, ArgType2, ArgType3, ArgType4, ArgType5),                                                             \
        JUTILS_HELPER_CONCAT(ArgName1, ArgName2, ArgName3, ArgName4, ArgName5),                                                             \
        JUTILS_HELPER_CONCAT(ArgType1 ArgName1, ArgType2 ArgName2, ArgType3 ArgName3, ArgType4 ArgName4, ArgType5 ArgName5)                 \
    )