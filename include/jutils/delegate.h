// Copyright © 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#include "delegate_single.h"
#include <algorithm>
#include <vector>

namespace jutils
{
    template<typename... Args>
    class delegate
    {
        using delegate_type = delegate_single<Args...>;

    protected:
        delegate() = default;
    public:
        delegate(const delegate&) = delete;
        delegate(delegate&&) = delete;
        ~delegate() = default;

        delegate& operator=(const delegate&) = delete;
        delegate& operator=(delegate&&) = delete;

        template<typename T>
        void bind(T* object, void (T::*function)(Args...)) const
        {
            if ((object != nullptr) && (function != nullptr) && !isBinded(object, function))
            {
                _delegates.emplace_back(object, function);
            }
        }
        void bind(const std::function<void(Args...)>& function) const
        {
            if ((function != nullptr) && !isBinded(function))
            {
                _delegates.emplace_back(function);
            }
        }
        void bind(std::function<void(Args...)>&& function) const
        {
            if ((function != nullptr) && !isBinded(function))
            {
                _delegates.emplace_back(std::move(function));
            }
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

        template<typename T>
        void unbind(T* object, void (T::*function)(Args...)) const
        {
            const std::size_t index = _findDelegate(object, function);
            if (index != invalid_index)
            {
                if (_callCounter == 0)
                {
                    _delegates.erase(std::next(_delegates.begin(), index));
                }
                else
                {
                    _delegates[index].clear();
                }
            }
        }
        void unbind(const std::function<void(Args...)>& function) const
        {
            const std::size_t index = _findDelegate(function);
            if (index != invalid_index)
            {
                if (_callCounter == 0)
                {
                    _delegates.erase(std::next(_delegates.begin(), index));
                }
                else
                {
                    _delegates[index].clear();
                }
            }
        }

        void clear() const
        {
            if (_callCounter > 0)
            {
                for (auto& delegate : _delegates)
                {
                    delegate.clear();
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
                    _delegates[index].call(std::forward<Args>(args)...);
                }
                _callCounter--;
                if (_callCounter == 0)
                {
                    _clearInvalidDelegates();
                }
            }
        }

    private:

        mutable std::vector<delegate_type> _delegates;
        mutable uint16 _callCounter = 0;


        template<typename T>
        [[nodiscard]] std::size_t _findDelegate(T* object, void (T::*function)(Args...)) const
        {
            if ((object != nullptr) && (function != nullptr))
            {
                for (std::size_t index = 0; index < _delegates.size(); index++)
                {
                    if (_delegates[index].isBinded(object, function))
                    {
                        return index;
                    }
                }
            }
            return invalid_index;
        }
        [[nodiscard]] std::size_t _findDelegate(const std::function<void(Args...)>& function) const
        {
            if (function != nullptr)
            {
                for (std::size_t index = 0; index < _delegates.size(); index++)
                {
                    if (_delegates[index].isBinded(function))
                    {
                        return index;
                    }
                }
            }
            return invalid_index;
        }
        void _clearInvalidDelegates() const
        {
#if JUTILS_STD_VERSION >= JUTILS_STD20
            std::erase_if(_delegates, [](const delegate_type& d){ return !d.isValid(); });
#else
            const auto iter = std::remove_if(_delegates.begin(), _delegates.end(), [](const delegate_type& d) { return !d.isValid(); });
            _delegates.erase(iter, _delegates.end());
#endif
        }
    };
}



#define JUTILS_DELEGATE_HELPER(DelegateName, ParamTypes, ParamNames, Params)    \
    class DelegateName : public jutils::delegate<ParamTypes>                    \
    {                                                                           \
        using base_class = jutils::delegate<ParamTypes>;                        \
    public:                                                                     \
        DelegateName() = default;                                               \
        DelegateName(const DelegateName&) = delete;                             \
        DelegateName(DelegateName&&) = delete;                                  \
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