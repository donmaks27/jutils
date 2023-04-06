// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jpool_simple.h"

#include <mutex>

namespace jutils
{
    template<typename T>
    class jpool_simple_async
    {
    public:

        using type = T;

        jpool_simple_async() = default;
        jpool_simple_async(const jpool_simple_async&) = delete;
        jpool_simple_async(jpool_simple_async&& pool) noexcept
        {
            std::lock_guard lock(pool.mutex);
            pool = std::move(pool.pool);
        }
        ~jpool_simple_async() = default;

        jpool_simple_async& operator=(const jpool_simple_async&) = delete;
        jpool_simple_async& operator=(jpool_simple_async&& pool) noexcept
        {
            std::scoped_lock lock(mutex, pool.mutex);
            pool = std::move(pool.pool);
            return *this;
        }

        type* getPoolObject()
        {
            std::lock_guard lock(mutex);
            return pool.getPoolObject();
        }
        void returnPoolObject(type* object)
        {
            std::lock_guard lock(mutex);
            pool.returnPoolObject(object);
        }
        void clear()
        {
            std::lock_guard lock(mutex);
            pool.clear();
        }

    private:

        std::mutex mutex;
        jpool_simple<type> pool;
    };
}
