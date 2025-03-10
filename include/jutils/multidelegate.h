// Copyright © 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#include "delegate.h"
#include "uid.h"
#include <algorithm>
#include <iterator>
#include <vector>

namespace jutils
{
    using delegate_id = uint32;
    constexpr delegate_id invalid_delegate_id = uid<delegate_id>::invalidUID;

    template<typename... Args>
    class multidelegate
    {
        using delegate_type = delegate<Args...>;
        template<typename T>
        using method_type = typename delegate_type::template method_type<T>;
        using callable_type = typename delegate_type::callable_type;

    protected:
        multidelegate() = default;
        multidelegate(const multidelegate& other)
        {
            _copyDelegates(other);
        }
        multidelegate(multidelegate&& other) noexcept
        {
            if (other._callCounter == 0)
            {
                _delegates = std::move(other._delegates);
                _idGenerator = std::move(other._idGenerator);
            }
            else
            {
                _copyDelegates(other);
                other.clear();
            }
        }
        multidelegate& operator=(const multidelegate& other)
        {
            if (this != &other)
            {
                clear();
                _appendDelegates(other);
            }
            return *this;
        }
        multidelegate& operator=(multidelegate&& other) noexcept
        {
            clear();
            if (_callCounter == 0)
            {
                if (other._callCounter == 0)
                {
                    _delegates = std::move(other._delegates);
                    _idGenerator = std::move(other._idGenerator);
                }
                else
                {
                    _copyDelegates(other);
                    other.clear();
                }
            }
            else
            {
                _appendDelegates(std::move(other));
                other.clear();
            }
            return *this;
        }
    public:
        ~multidelegate() = default;

        [[nodiscard]] bool isValid() const
        {
            return !_delegates.empty() && std::find_if(_delegates.begin(), _delegates.end(), [](const delegate_entry& entry) {
                return entry.delegate.isValid();
            }) != _delegates.end();
        }

        JUTILS_TEMPLATE_CONDITION((std::is_function_v<std::remove_pointer_t<Func>>), typename Func)
        [[nodiscard]] bool isBinded(const Func function) const { return _findDelegate(function) != invalid_index; }
        template<typename T>
        [[nodiscard]] bool isBinded(const T* object, const method_type<T> function) const { return _findDelegate(object, function) != invalid_index; }
        [[nodiscard]] bool isBinded(const delegate_id id) const { return _findDelegate(id) != invalid_index; }

        JUTILS_TEMPLATE_CONDITION((std::is_function_v<std::remove_pointer_t<Func>>), typename Func)
        [[nodiscard]] delegate_id find(const Func function) const
        {
            const std::size_t index = _findDelegate(function);
            return index != invalid_index ? _delegates[index].id : invalid_delegate_id;
        }
        template<typename T>
        [[nodiscard]] delegate_id find(const T* object, const method_type<T> function) const
        {
            const std::size_t index = _findDelegate(object, function);
            return index != invalid_index ? _delegates[index].id : invalid_delegate_id;
        }
        
        JUTILS_TEMPLATE_CONDITION((std::is_function_v<std::remove_pointer_t<Func>>), typename Func)
        delegate_id bind(Func function) const
        {
            if (function == nullptr)
            {
                return invalid_delegate_id;
            }
            const std::size_t index = _findDelegate(function);
            if (index != invalid_index)
            {
                return _delegates[index].id;
            }
            const delegate_id id = _idGenerator.generateUID();
            _delegates.emplace_back(delegate_type{ function }, id);
            return id;
        }
        template<typename T>
        delegate_id bind(T* object, const method_type<T> function) const
        {
            if ((object == nullptr) || (function == nullptr))
            {
                return invalid_delegate_id;
            }
            const std::size_t index = _findDelegate(object, function);
            if (index != invalid_index)
            {
                return _delegates[index].id;
            }
            const delegate_id id = _idGenerator.generateUID();
            _delegates.emplace_back(delegate_type{ object, function }, id);
            return id;
        }
        delegate_id bind(callable_type&& function) const
        {
            if (function != nullptr)
            {
                const delegate_id id = _idGenerator.generateUID();
                _delegates.emplace_back(delegate_type{ std::forward<callable_type>(function) }, id);
                return id;
            }
            return invalid_delegate_id;
        }
        
        JUTILS_TEMPLATE_CONDITION((std::is_function_v<std::remove_pointer_t<Func>>), typename Func)
        void unbind(const Func function) const { _unbind(_findDelegate(function)); }
        template<typename T>
        void unbind(const T* object, const method_type<T> function) const { _unbind(_findDelegate(object, function)); }
        void unbind(const delegate_id id) const { _unbind(_findDelegate(id)); }

        void clear() const
        {
            if (_callCounter > 0)
            {
                for (auto& entry : _delegates)
                {
                    entry.delegate.clear();
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
            if (isValid())
            {
                _callCounter++;
                const std::size_t lastIndex = _delegates.size() - 1;
                for (std::size_t index = 0; index <= lastIndex; index++)
                {
                    _delegates[index].delegate.call(std::forward<Args>(args)...);
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


        void _copyDelegates(const multidelegate& other)
        {
            _delegates.reserve(other._delegates.size());
            std::copy_if(other._delegates.begin(), other._delegates.end(), std::back_inserter(_delegates), [](const delegate_entry& entry) {
                return entry.delegate.isValid();
            });
            _idGenerator = other._idGenerator;
        }
        void _appendDelegates(const multidelegate& other)
        {
            const std::size_t prevSize = _delegates.size();
            _delegates.reserve(prevSize + other._delegates.size());
            std::copy_if(other._delegates.begin(), other._delegates.end(), std::back_inserter(_delegates), [this, prevSize](const delegate_entry& entry) {
                if (entry.delegate.isValid())
                {
                    const auto prevEnd = std::next(_delegates.begin(), prevSize);
                    return std::find_if(_delegates.begin(), prevEnd, [&entry](const delegate_entry& prevEntry) {
                        return prevEntry.delegate == entry.delegate;
                    }) == prevEnd;
                }
                return false;
            });
            std::for_each(std::next(_delegates.begin(), prevSize), _delegates.end(), [this](delegate_entry& entry) {
                entry.id = _idGenerator.generateUID();
            });
        }
        void _appendDelegates(multidelegate&& other)
        {
            const std::size_t prevSize = _delegates.size();
            _delegates.reserve(_delegates.size() + other._delegates.size());
            std::for_each(other._delegates.begin(), other._delegates.end(), [this, move = (other._callCounter == 0), prevSize](delegate_entry& entry) {
                if (entry.delegate.isValid())
                {
                    const auto prevEnd = std::next(_delegates.begin(), prevSize);
                    const auto iter = std::find_if(_delegates.begin(), prevEnd, [&entry](const delegate_entry& prevEntry) {
                        return prevEntry.delegate == entry.delegate;
                    });
                    if (iter == prevEnd)
                    {
                        if (move)
                        {
                            _delegates.emplace_back(std::move(entry.delegate), _idGenerator.generateUID());
                        }
                        else
                        {
                            _delegates.emplace_back(entry.delegate, _idGenerator.generateUID());
                        }
                    }
                }
            });
        }

        JUTILS_TEMPLATE_CONDITION((std::is_function_v<std::remove_pointer_t<Func>>), typename Func)
        [[nodiscard]] std::size_t _findDelegate(const Func function) const
        {
            for (std::size_t index = 0; index < _delegates.size(); index++)
            {
                if (_delegates[index].delegate.isBinded(function))
                {
                    return index;
                }
            }
            return invalid_index;
        }
        template<typename T>
        [[nodiscard]] std::size_t _findDelegate(const T* object, const method_type<T> function) const
        {
            for (std::size_t index = 0; index < _delegates.size(); index++)
            {
                if (_delegates[index].delegate.isBinded(object, function))
                {
                    return index;
                }
            }
            return invalid_index;
        }
        [[nodiscard]] std::size_t _findDelegate(const delegate_id id) const
        {
            for (std::size_t index = 0; index < _delegates.size(); index++)
            {
                if (_delegates[index].id == id)
                {
                    return index;
                }
            }
            return invalid_index;
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
                    _delegates[index].delegate.clear();
                }
            }
        }
        void _clearInvalidDelegates() const
        {
#if JUTILS_STD_VERSION >= JUTILS_STD20
            std::erase_if(_delegates, [](const delegate_entry& entry) {
                return !entry.delegate.isValid();
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
    class DelegateName final : public jutils::multidelegate<ParamTypes>         \
    {                                                                           \
        using base_class = jutils::multidelegate<ParamTypes>;                   \
    public:                                                                     \
        DelegateName() = default;                                               \
        DelegateName(const DelegateName&) = default;                            \
        DelegateName(DelegateName&&) noexcept = default;                        \
        ~DelegateName() = default;                                              \
        DelegateName& operator=(std::nullptr_t) { clear(); return *this; }      \
        DelegateName& operator=(const DelegateName&) = default;                 \
        DelegateName& operator=(DelegateName&&) noexcept = default;             \
        void call(Params) const { _call(ParamNames); }                          \
        void operator()(Params) const { _call(ParamNames); }                    \
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