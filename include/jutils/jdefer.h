// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include <functional>

namespace jutils
{
    class jdefer
    {
    public:
        jdefer() = delete;
        jdefer(const std::function<void()>& f)
            : _deferFunction(f)
        {}
        jdefer(std::function<void()>&& f)
            : _deferFunction(std::move(f))
        {}
        jdefer(const jdefer&) = delete;
        jdefer(jdefer&&) noexcept = delete;
        ~jdefer() { _deferFunction(); }

        jdefer& operator=(const jdefer&) = delete;
        jdefer& operator=(jdefer&&) noexcept = delete;

    private:

        std::function<void()> _deferFunction;
    };
}