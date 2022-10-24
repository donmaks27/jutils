// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jdelegate.h"
#include "jarray.h"

namespace jutils
{
    template<typename... ArgTypes>
    class jdelegate_multicast
    {
    public:

        jdelegate_multicast() = default;
        jdelegate_multicast(const jdelegate_multicast& value)
            : delegates_container(value.delegates_container)
        {}
        jdelegate_multicast(jdelegate_multicast&& value) noexcept
            : delegates_container(std::move(value.delegates_container))
        {}

        jdelegate_multicast& operator=(const jdelegate_multicast& value)
        {
            if (this != &value)
            {
                delegates_container = value.delegates_container;
            }
            return *this;
        }
        jdelegate_multicast& operator=(jdelegate_multicast&& value) noexcept
        {
            delegates_container = std::move(value.delegates_container);
            return *this;
        }

        template<typename T>
        void bind(T* object, void (T::*function)(ArgTypes...))
        {
            if ((object != nullptr) && !isBinded(object, function))
            {
                delegates_container.addDefault().bind(object, function);
            }
        }

        template<typename T>
        bool isBinded(T* object, void (T::*function)(ArgTypes...)) const
        {
            if (object != nullptr)
            {
                for (const auto& delegate_entry : delegates_container)
                {
                    if (delegate_entry.isBinded(object, function))
                    {
                        return true;
                    }
                }
            }
            return false;
        }
        template<typename T>
        bool isBinded(T* object) const
        {
            if (object != nullptr)
            {
                for (const auto& delegate : delegates_container)
                {
                    if (delegate.isBinded(object))
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        template<typename T>
        void unbind(T* object)
        {
            if (object != nullptr)
            {
                int32 index = 0;
                while (index < delegates_container.getSize())
                {
                    if (delegates_container[index].isBinded(object))
                    {
                        delegates_container.removeAt(index);
                    }
                    else
                    {
                        index++;
                    }
                }
            }
        }
        template<typename T>
        void unbind(T* object, void (T::*callback)(ArgTypes...))
        {
            if (object != nullptr)
            {
                for (int32 index = 0; index < delegates_container.getSize(); index++)
                {
                    if (delegates_container[index].isBinded(object, callback))
                    {
                        delegates_container.removeAt(index);
                        break;
                    }
                }
            }
        }

        void clear() { delegates_container.clear(); }

        void _call_internal(ArgTypes... args) const
        {
            auto delegatesCopy = delegates_container;
            for (auto& delegate : delegatesCopy)
            {
                delegate._call_internal(args...);
            }
        }

    private:

        mutable jarray<jdelegate<ArgTypes...>> delegates_container;
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
    void call(Params) const { _call_internal(ParamsNames); }                                        \
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
