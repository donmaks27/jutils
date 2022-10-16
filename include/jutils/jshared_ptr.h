// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include <memory>
#include "type_checks.h"

namespace jutils
{
    template<typename T>
    class jshared_ptr : public std::shared_ptr<T>
    {
        using base_class = std::shared_ptr<T>;

    public:
        constexpr jshared_ptr() noexcept
            : base_class()
        {}
        constexpr jshared_ptr(nullptr_t) noexcept
            : base_class(nullptr)
        {}
        jshared_ptr(T* ptr)
            : base_class(ptr)
        {}
        template<typename U, TEMPLATE_ENABLE(std::_SP_pointer_compatible<U, T>::value)>
        jshared_ptr(const std::shared_ptr<U>& ptr) noexcept
            : base_class(ptr)
        {}
        template<typename U, TEMPLATE_ENABLE(std::_SP_pointer_compatible<U, T>::value)>
        jshared_ptr(std::shared_ptr<U>&& ptr) noexcept
            : base_class(std::move(ptr))
        {}

        T* operator->() { return base_class::get(); }
        const T* operator->() const { return base_class::get(); }
    };

    template<typename T>
    class jweak_ptr : public std::weak_ptr<T>
    {
        using base_class = std::weak_ptr<T>;

    public:
        constexpr jweak_ptr() noexcept
            : base_class()
        {}
        jweak_ptr(const jweak_ptr& ptr) = default;
        template<typename U, std::enable_if_t<std::_SP_pointer_compatible<U, T>::value, int> = 0>
        jweak_ptr(const std::shared_ptr<U>& ptr) noexcept
            : base_class(ptr)
        {}
        template<typename U, std::enable_if_t<std::_SP_pointer_compatible<U, T>::value, int> = 0>
        jweak_ptr(const std::weak_ptr<U>& ptr) noexcept
            : base_class(ptr)
        {}
    };

    template<typename T, typename... Args>
    jshared_ptr<T> make_jshared_ptr(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }

    template<typename To, typename From>
    jshared_ptr<To> jshared_dynamic_cast(const jshared_ptr<From>& ptr) { return std::dynamic_pointer_cast<To>(ptr); }
}
