// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "type_defines.h"
#include "jmemory.h"

namespace jutils
{
    template<typename T>
    class jlist_allocator
    {
    public:

        using type = T;
        using index_type = int32;

        jlist_allocator() = default;
        jlist_allocator(const jlist_allocator&) = delete;
        jlist_allocator(jlist_allocator&& allocator) noexcept
        {
            lastSegment = allocator.lastSegment;
            allocator.lastSegment = nullptr;
        }
        ~jlist_allocator()
        {
            clear();
        }

        jlist_allocator& operator=(const jlist_allocator&) = delete;
        jlist_allocator& operator=(jlist_allocator&& allocator) noexcept
        {
            clear();
            lastSegment = allocator.lastSegment;
            allocator.lastSegment = nullptr;
            return *this;
        }

        index_type getSize() const { return size; }

        void allocateSegment(type*& outData, index_type& outSize)
        {
            _allocateNewSegment();
            outData = lastSegment->data;
            outSize = lastSegment->size;
        }
        void clear() { _clearAllSegments(); }

    private:

        static constexpr index_type default_segment_size = 4;

        struct segment_description
        {
            segment_description* prevSegment = nullptr;
            type* data = nullptr;
            index_type size = 0;
        };

        segment_description* lastSegment = nullptr;
        index_type size = 0;


        static type* _allocateData(const index_type size) { return jutils::memory::allocate<type>(size); }
        static void _deallocateData(type* data, const index_type size) { jutils::memory::deallocate(data, size); }

        static segment_description* _allocateSegment(const index_type size)
        {
            segment_description* segment = new segment_description();
            segment->size = size;
            segment->data = _allocateData(size);
            return segment;
        }
        static segment_description* _deallocateSegment(segment_description* segment)
        {
            segment_description* prevSegment = segment->prevSegment;
            _deallocateData(segment->data, segment->size);
            delete segment;
            return prevSegment;
        }

        void _allocateNewSegment()
        {
            const index_type segmentSize = lastSegment != nullptr ? size : default_segment_size;
            segment_description* segment = _allocateSegment(segmentSize);
            segment->prevSegment = lastSegment;

            lastSegment = segment;
            size += segmentSize;
        }
        void _clearAllSegments()
        {
            while (lastSegment != nullptr)
            {
                lastSegment = _deallocateSegment(lastSegment);
            }
            size = 0;
        }
    };
}
