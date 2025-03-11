// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include <functional>

namespace jutils
{
    class defer
    {
    public:
        defer() = delete;
        explicit defer(const std::function<void()>& function) : _function(function) {}
        explicit defer(std::function<void()>&& function) : _function(std::move(function)) {}
        defer(const defer&) = delete;
        defer(defer&&) noexcept = delete;
        ~defer()
        {
            if (_function != nullptr)
            {
                _function();
            }
        }

        defer& operator=(const defer&) = delete;
        defer& operator=(defer&&) noexcept = delete;

    private:

        std::function<void()> _function;
    };
}

#define JUTILS_DEFER_ID_HELPER(id, ...) jutils::defer defer_##id([&]() { __VA_ARGS__ })
#define JUTILS_DEFER_ID(id, ...) JUTILS_DEFER_ID_HELPER(id, __VA_ARGS__)
#define JUTILS_DEFER(...) JUTILS_DEFER_ID(__LINE__, __VA_ARGS__)