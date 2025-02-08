// Copyright © 2025 Leonov Maksim. All Rights Reserved.

#pragma once

#include "../core.h"

#define JUTILS_DELEGATE_HELPER_CONCAT(...) __VA_ARGS__
#define JUTILS_DELEGATE_HELPER(DelegateName, ParamsTypes, ParamsNames, Params)                  \
class DelegateName : public jutils::delegate<ParamsTypes>                                       \
{                                                                                               \
    using base_class = jutils::delegate<ParamsTypes>;                                           \
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

#define JUTILS_DELEGATE(DelegateName) JUTILS_DELEGATE_HELPER(DelegateName, , , )
#define JUTILS_DELEGATE1(DelegateName, ParamType1, ParamName1) JUTILS_DELEGATE_HELPER(DelegateName, \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1), JUTILS_DELEGATE_HELPER_CONCAT(ParamName1), \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1 ParamName1))
#define JUTILS_DELEGATE2(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2) JUTILS_DELEGATE_HELPER(DelegateName, \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1, ParamType2), JUTILS_DELEGATE_HELPER_CONCAT(ParamName1, ParamName2), \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1 ParamName1, ParamType2 ParamName2))
#define JUTILS_DELEGATE3(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3) JUTILS_DELEGATE_HELPER(DelegateName, \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1, ParamType2, ParamType3), JUTILS_DELEGATE_HELPER_CONCAT(ParamName1, ParamName2, ParamName3), \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3))
#define JUTILS_DELEGATE4(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4) JUTILS_DELEGATE_HELPER(DelegateName, \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1, ParamType2, ParamType3, ParamType4), JUTILS_DELEGATE_HELPER_CONCAT(ParamName1, ParamName2, ParamName3, ParamName4), \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4))
#define JUTILS_DELEGATE5(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5) JUTILS_DELEGATE_HELPER(DelegateName, \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5), JUTILS_DELEGATE_HELPER_CONCAT(ParamName1, ParamName2, ParamName3, ParamName4, ParamName5), \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5))


#define JUTILS_MULTICAST_DELEGATE_HELPER(DelegateName, ParamsTypes, ParamsNames, Params)\
class DelegateName : public jutils::delegate_multicast<ParamsTypes>                     \
{                                                                                       \
    using base_class = jutils::delegate_multicast<ParamsTypes>;                         \
public:                                                                                 \
    DelegateName() : base_class() {}                                                    \
    DelegateName(const base_class& value) : base_class(value) {}                        \
    DelegateName(base_class&& value) noexcept : base_class(std::move(value)) {}         \
    DelegateName& operator=(const base_class& value)                                    \
        { base_class::operator=(value); return *this; }                                 \
    DelegateName& operator=(base_class&& value) noexcept                                \
        { base_class::operator=(std::move(value)); return *this; }                      \
    void call(Params) const { base_class::call(ParamsNames); }                          \
}

#define JUTILS_MULTICAST_DELEGATE(DelegateName) JUTILS_MULTICAST_DELEGATE_HELPER(DelegateName, , , )
#define JUTILS_MULTICAST_DELEGATE1(DelegateName, ParamType1, ParamName1) JUTILS_MULTICAST_DELEGATE_HELPER(DelegateName, \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1), JUTILS_DELEGATE_HELPER_CONCAT(ParamName1), \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1 ParamName1))
#define JUTILS_MULTICAST_DELEGATE2(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2) JUTILS_MULTICAST_DELEGATE_HELPER(DelegateName, \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1, ParamType2), JUTILS_DELEGATE_HELPER_CONCAT(ParamName1, ParamName2), \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1 ParamName1, ParamType2 ParamName2))
#define JUTILS_MULTICAST_DELEGATE3(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3) JUTILS_MULTICAST_DELEGATE_HELPER(DelegateName, \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1, ParamType2, ParamType3), JUTILS_DELEGATE_HELPER_CONCAT(ParamName1, ParamName2, ParamName3), \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3))
#define JUTILS_MULTICAST_DELEGATE4(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4) JUTILS_MULTICAST_DELEGATE_HELPER(DelegateName, \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1, ParamType2, ParamType3, ParamType4), JUTILS_DELEGATE_HELPER_CONCAT(ParamName1, ParamName2, ParamName3, ParamName4), \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4))
#define JUTILS_MULTICAST_DELEGATE5(DelegateName, ParamType1, ParamName1, ParamType2, ParamName2, ParamType3, ParamName3, ParamType4, ParamName4, ParamType5, ParamName5) JUTILS_MULTICAST_DELEGATE_HELPER(DelegateName, \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5), JUTILS_DELEGATE_HELPER_CONCAT(ParamName1, ParamName2, ParamName3, ParamName4, ParamName5), \
    JUTILS_DELEGATE_HELPER_CONCAT(ParamType1 ParamName1, ParamType2 ParamName2, ParamType3 ParamName3, ParamType4 ParamName4, ParamType5 ParamName5))
