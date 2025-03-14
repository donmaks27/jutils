﻿// Copyright © 2021-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jmemory.h"
#include "math/math.h"
#include <deque>

namespace jutils
{
    template<typename T, uint8 SegmentSize = 1>
    class jpool
    {
    public:

        using type = T;

        jpool() = default;
        jpool(const jpool&) = delete;
        jpool(jpool&& pool) noexcept = default;
        ~jpool() { clear(); }

        jpool& operator=(const jpool&) = delete;
        jpool& operator=(jpool&& pool) noexcept = default;

        template<typename... Args>
        [[nodiscard]] type* getObject(Args&&... args);
        void returnObject(type* object);
        void clear();
        
    private:

        static constexpr uint8 segment_size = jutils::math::max(SegmentSize, 1);
        struct alignas(type) internal_type
        {
            uint8 data[sizeof(type)];
        };
        struct segment_type
        {
            internal_type data[segment_size];
        };

        std::deque<segment_type> objectsPool;
        std::deque<type*> unusedObjects;

        
        template<typename... Args>
        void _initPoolObject(type* object, Args&&... args);
        void _clearPoolObject(type* object);
    };
    
    template<typename T, uint8 SegmentSize>
    template<typename... Args>
    typename jpool<T, SegmentSize>::type* jpool<T, SegmentSize>::getObject(Args&&... args)
    {
        type* object;
        if (!unusedObjects.empty())
        {
            object = unusedObjects.front();
            unusedObjects.pop_front();
        }
        else
        {
            auto& segment = objectsPool.emplace_back()->data;
            object = reinterpret_cast<type*>(segment[0].data);
            if constexpr (segment_size > 1)
            {
                for (uint8 index = 1; index < segment_size; index++)
                {
                    unusedObjects.push_back(reinterpret_cast<type*>(segment[index].data));
                }
            }
        }
        this->_initPoolObject(object, std::forward<Args>(args)...);
        return object;
    }
    template<typename T, uint8 SegmentSize>
    void jpool<T, SegmentSize>::returnObject(type* object)
    {
        if (object != nullptr)
        {
            this->_clearPoolObject(object);
            unusedObjects.push_back(object);
        }
    }
    template<typename T, uint8 SegmentSize>
    void jpool<T, SegmentSize>::clear()
    {
        for (auto& segment : objectsPool)
        {
            for (auto& wrapper : segment.data)
            {
                type* object = reinterpret_cast<type*>(wrapper.data);
                if (std::find(unusedObjects.begin(), unusedObjects.end(), object) == unusedObjects.end())
                {
                    this->_clearPoolObject(object);
                }
            }
        }
        unusedObjects.clear();
        objectsPool.clear();
    }

    template <typename T, uint8 SegmentSize>
    template<typename... Args>
    void jpool<T, SegmentSize>::_initPoolObject(type* object, Args&&... args)
    {
        jutils::memory::construct(object, std::forward<Args>(args)...);
    }
    template <typename T, uint8 SegmentSize>
    void jpool<T, SegmentSize>::_clearPoolObject(type* object)
    {
        jutils::memory::destruct(object);
    }
}
