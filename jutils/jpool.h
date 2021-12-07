// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "type_defines.h"

#include <limits>
#include <stdexcept>

namespace jutils
{
    template<typename T>
    class jpool
    {
    public:

        using type = T;

        jpool() = default;
        jpool(const uint64 segmentSize, const uint64 defaultSegmentsCount = 0)
            : segmentSize(jutils::math::max(segmentSize, 1))
        {
            for (uint64 segmentIndex = 0; segmentIndex < defaultSegmentsCount; segmentIndex++)
            {
                createSegment();
            }
        }
        jpool(const jpool&) = delete;
        jpool(jpool&&) = delete;
        ~jpool() { clear(); }

        jpool& operator=(const jpool&) = delete;
        jpool& operator=(jpool&&) = delete;

        void clear()
        {
            pool_segment* segment = firstSegment;
            while (segment != nullptr)
            {
                pool_segment* nextSegment = segment->nextSegment;
                deleteSegment(segment);
                segment = nextSegment;
            }
        }

        template<typename... Args>
        type* createObject(Args&&... args)
        {
            type* object = allocateObjectNode();
            if (object != nullptr)
            {
                ::new (static_cast<void*>(object)) type(std::forward<Args>(args)...);
            }
            return object;
        }
        void destroyObject(type* object)
        {
            if (object != nullptr)
            {
                object->~T();
                deallocateObjectNode(object);
            }
        }

    private:

        struct pool_segment;

        template<typename IndexType>
        struct jpool_empty_nodes_getter_index
        {
            static void init(pool_segment* segment, uint64 size);
            static type* getFreeNode(pool_segment* segment);
            static void returnNode(pool_segment* segment, type* node);

        private:

            static void setNextNode(type* node, const IndexType index) { memcpy(node, &index, sizeof(IndexType)); }
            static IndexType getNextNode(const type* node)
            {
                IndexType index = 0;
                memcpy(&index, node, sizeof(IndexType));
                return index;
            }
        };
        struct jpool_empty_nodes_getter_pointer
        {
            static void init(pool_segment* segment, uint64 size);
            static type* getFreeNode(pool_segment* segment);
            static void returnNode(pool_segment* segment, type* node);

        private:

            static void setNextNode(type* node, const type* nextNode) { memcpy(node, &nextNode, sizeof(type*)); }
            static type* getNextNode(const type* node)
            {
                type* nextNode = nullptr;
                memcpy(&nextNode, node, sizeof(type*));
                return nextNode;
            }
        };
        using jpool_empty_nodes_getter = std::conditional_t<sizeof(type) >= sizeof(type*), jpool_empty_nodes_getter_pointer,
            jpool_empty_nodes_getter_index<std::conditional_t<sizeof(type) == 1, uint8, std::conditional_t<sizeof(type) < 4, uint16, uint32>>>
        >;

        struct pool_segment
        {
            type* data = nullptr;
            type* firstEmptyNode = nullptr;
            pool_segment* nextSegment = nullptr;

            bool isFull() const { return firstEmptyNode == nullptr; }
        };

        uint64 segmentSize = 1;
        pool_segment* firstSegment;


        pool_segment* createSegment()
        {
            pool_segment* segment = new pool_segment();
            segment->data = static_cast<type*>(::operator new(sizeof(type) * segmentSize));
            jpool_empty_nodes_getter::init(segment, segmentSize);

            if (firstSegment == nullptr)
            {
                firstSegment = segment;
            }
            else
            {
                pool_segment* prevSegment = firstSegment;
                while (prevSegment->nextSegment != nullptr)
                {
                    prevSegment = prevSegment->nextSegment;
                }
                prevSegment->nextSegment = segment;
            }
            return segment;
        }
        void removeSegment(pool_segment* segment)
        {
            if ((segment == nullptr) || (firstSegment == nullptr))
            {
                return;
            }
            if (firstSegment != segment)
            {
                pool_segment* prevSegment = firstSegment;
                while ((prevSegment != nullptr) && (prevSegment->nextSegment != segment))
                {
                    prevSegment = prevSegment->nextSegment;
                }
                if (prevSegment == nullptr)
                {
                    return;
                }

                prevSegment->nextSegment = segment->nextSegment;
            }
            else
            {
                firstSegment = firstSegment->nextSegment;
            }
            deleteSegment(segment);
        }
        void deleteSegment(pool_segment* segment)
        {
            ::operator delete(segment->data, sizeof(T) * segmentSize);
            delete segment;
        }

        type* allocateObjectNode()
        {
            pool_segment* segment = firstSegment;
            while ((segment != nullptr) && (segment->firstEmptyNode == nullptr))
            {
                segment = segment->nextSegment;
            }
            if (segment == nullptr)
            {
                segment = createSegment();
            }
            return jpool_empty_nodes_getter::getFreeNode(segment);
        }
        void deallocateObjectNode(type* node)
        {
            pool_segment* segment = firstSegment;
            while (segment != nullptr)
            {
                if ((node >= segment->data) && (node < segment->data + segmentSize))
                {
                    jpool_empty_nodes_getter::returnNode(segment, node);
                    break;
                }
                segment = segment->nextSegment;
            }
        }
    };

    template<typename T>
    template<typename IndexType>
    void jpool<T>::jpool_empty_nodes_getter_index<IndexType>::init(pool_segment* segment, const uint64 size)
    {
        if (size > std::numeric_limits<IndexType>::max())
        {
            throw std::exception("Segment size can't be bigger then max index");
        }
        for (uint64 i = 0; i < size - 1; i++)
        {
            setNextNode(segment->data + i, i + 2);
        }
        setNextNode(segment->data + size - 1, 0);
        segment->firstEmptyNode = segment->data;
    }
    template<typename T>
    template<typename IndexType>
    T* jpool<T>::jpool_empty_nodes_getter_index<IndexType>::getFreeNode(pool_segment* segment)
    {
        if (segment->firstEmptyNode == nullptr)
        {
            return nullptr;
        }
        type* result = segment->firstEmptyNode;
        const IndexType nextNodeIndex = getNextNode(segment->firstEmptyNode);
        segment->firstEmptyNode = nextNodeIndex > 0 ? segment->data + nextNodeIndex - 1 : nullptr;
        return result;
    }
    template<typename T>
    template<typename IndexType>
    void jpool<T>::jpool_empty_nodes_getter_index<IndexType>::returnNode(pool_segment* segment, type* node)
    {
        setNextNode(node, segment->firstEmptyNode != nullptr ? segment->firstEmptyNode - segment->data + 1 : 0);
        segment->firstEmptyNode = node;
    }

    template<typename T>
    void jpool<T>::jpool_empty_nodes_getter_pointer::init(pool_segment* segment, const uint64 size)
    {
        for (uint64 i = 0; i < size - 1; i++)
        {
            setNextNode(segment->data + i, segment->data + i + 1);
        }
        setNextNode(segment->data + size - 1, nullptr);
        segment->firstEmptyNode = segment->data;
    }
    template<typename T>
    T* jpool<T>::jpool_empty_nodes_getter_pointer::getFreeNode(pool_segment* segment)
    {
        if (segment->firstEmptyNode == nullptr)
        {
            return nullptr;
        }
        type* result = segment->firstEmptyNode;
        segment->firstEmptyNode = getNextNode(segment->firstEmptyNode);
        return result;
    }
    template<typename T>
    void jpool<T>::jpool_empty_nodes_getter_pointer::returnNode(pool_segment* segment, type* node)
    {
        setNextNode(node, segment->firstEmptyNode);
        segment->firstEmptyNode = node;
    }
}
