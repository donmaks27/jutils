// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"
#include "juid.h"

namespace jutils
{
    class jpool_pointer
    {
        template<typename T>
        friend class jpool;

    public:
        constexpr jpool_pointer() = default;
        constexpr jpool_pointer(const jpool_pointer& value) = default;
    private:
        constexpr jpool_pointer(const int32 segment, const uint32 node, const uint32 uid)
            : segmentIndex(segment), nodeIndex(node), UID(uid)
        {}

    public:

        constexpr bool isValid() const { return (UID != juid<uint32>::invalidUID) && (segmentIndex >= 0); }

        constexpr bool operator==(const jpool_pointer& pointer) const
        {
            if (isValid())
            {
                return (segmentIndex == pointer.segmentIndex) && (nodeIndex == pointer.nodeIndex) && (UID == pointer.UID);
            }
            return !pointer.isValid();
        }
        constexpr bool operator!=(const jpool_pointer& pointer) const { return !operator==(pointer); }

        constexpr bool operator==(nullptr_t) const { return !isValid(); }
        constexpr bool operator!=(nullptr_t) const { return isValid(); }

    private:

        int32 segmentIndex = -1;
        uint32 nodeIndex = 0;
        uint32 UID = juid<uint32>::invalidUID;
    };
    constexpr jpool_pointer jpool_pointer_INVALID = {};

    template<typename T>
    class jpool
    {
    public:

        using type = T;
        using segment_size_type = uint32;
        using uid_type = uint32;
        using uid_generator_type = juid<uid_type>;
        using pointer = jpool_pointer;

        jpool()
            : jpool(1)
        {}
        explicit jpool(const segment_size_type segmentSize, const int32 defaultSegmentsCount = 0)
            : segmentSize(jutils::math::max(segmentSize, 1))
        {
            if (defaultSegmentsCount > 0)
            {
                segments.reserve(defaultSegmentsCount);
                for (int32 segmentIndex = 0; segmentIndex < defaultSegmentsCount; segmentIndex++)
                {
                    createSegment();
                }
            }
        }
        jpool(const jpool&) = delete;
        jpool(jpool&& value) noexcept
        {
            segmentSize = value.segmentSize;
            segments = std::move(value.segments);
        }
        ~jpool() { clear(); }

        jpool& operator=(const jpool&) = delete;
        jpool& operator=(jpool&&) = delete;
        
        template<typename... Args>
        pointer createObject(Args&&... args)
        {
            const pointer nodePointer = allocateObjectNode();
            jutils::memory::construct(getObjectNode(nodePointer), std::forward<Args>(args)...);
            return nodePointer;
        }
        bool isObjectValid(const pointer& pointer) const
        {
            return segments.isValidIndex(pointer.segmentIndex) && 
                (pointer.nodeIndex < segmentSize) &&
                (pointer.UID != uid_generator_type::invalidUID) &&
                (pointer.UID == segments.getData()[pointer.segmentIndex]->nodeUIDs[pointer.nodeIndex]);
        }
        type* getObject(const pointer& nodePointer) const { return isObjectValid(nodePointer) ? getObjectNode(nodePointer) : nullptr; }
        void destroyObject(const pointer& pointer)
        {
            if (isObjectValid(pointer))
            {
                jutils::memory::destruct(getObjectNode(pointer));
                deallocateObjectNode(pointer);
            }
        }

        void clear(const bool deconstructObjects = true)
        {
            for (const auto& segment : segments)
            {
                deleteSegment(segment, deconstructObjects);
            }
            segments.clear();
        }

    private:

        struct pool_segment
        {
            type* data = nullptr;
            type* firstEmptyNode = nullptr;
            uid_type* nodeUIDs = nullptr;
        };

        template<typename IndexType>
        struct jpool_empty_nodes_getter_index
        {
            static void init(pool_segment* segment, segment_size_type size);
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
            static void init(pool_segment* segment, segment_size_type size);
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

        jarray<pool_segment*> segments;
        uid_generator_type nodeUIDs;
        segment_size_type segmentSize = 1;


        pool_segment* createSegment()
        {
            pool_segment* segment = segments.add(new pool_segment());

            segment->data = jutils::memory::allocate<type>(segmentSize);
            jpool_empty_nodes_getter::init(segment, segmentSize);

            segment->nodeUIDs = new uid_type[segmentSize];
            for (segment_size_type i = 0; i < segmentSize; i++)
            {
                segment->nodeUIDs[i] = uid_generator_type::invalidUID;
            }

            return segment;
        }
        void deleteSegment(pool_segment* segment, const bool deconstructObjects)
        {
            if (deconstructObjects)
            {
                for (segment_size_type nodeIndex = 0; nodeIndex < segmentSize; nodeIndex++)
                {
                    if (segment->nodeUIDs[nodeIndex] != uid_generator_type::invalidUID)
                    {
                        jutils::memory::destruct(&segment->data[nodeIndex]);
                    }
                }
            }
            jutils::memory::deallocate(segment->data, segmentSize);
            delete[] segment->nodeUIDs;
            delete segment;
        }

        pointer allocateObjectNode()
        {
            int32 segmentIndex = -1;
            for (int32 i = 0; i < segments.getSize(); i++)
            {
                if (segments.getData()[i]->firstEmptyNode != nullptr)
                {
                    segmentIndex = i;
                    break;
                }
            }
            if (segmentIndex == -1)
            {
                createSegment();
                segmentIndex = segments.getSize() - 1;
            }

            pool_segment* segment = segments.getData()[segmentIndex];
            const segment_size_type nodeIndex = static_cast<segment_size_type>(jpool_empty_nodes_getter::getFreeNode(segment) - segment->data);
            const uid_type nodeUID = segment->nodeUIDs[nodeIndex] = nodeUIDs.getUID();
            return { segmentIndex, nodeIndex, nodeUID };
        }
        type* getObjectNode(const pointer& nodePointer) const { return segments.getData()[nodePointer.segmentIndex]->data + nodePointer.nodeIndex; }
        void deallocateObjectNode(const pointer& nodePointer)
        {
            pool_segment* segment = segments.getData()[nodePointer.segmentIndex];
            jpool_empty_nodes_getter::returnNode(segment, segment->data + nodePointer.nodeIndex);
            segment->nodeUIDs[nodePointer.nodeIndex] = uid_generator_type::invalidUID;
        }
    };

    template<typename T>
    template<typename IndexType>
    void jpool<T>::jpool_empty_nodes_getter_index<IndexType>::init(pool_segment* segment, const segment_size_type size)
    {
        if (size > std::numeric_limits<IndexType>::max())
        {
            throw std::exception("Segment size can't be bigger then max index");
        }
        for (segment_size_type i = 0; i < size - 1; i++)
        {
            setNextNode(segment->data + i, i + 2);
        }
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
    void jpool<T>::jpool_empty_nodes_getter_pointer::init(pool_segment* segment, const segment_size_type size)
    {
        for (segment_size_type i = 0; i < size - 1; i++)
        {
            setNextNode(segment->data + i, segment->data + i + 1);
        }
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
        segment->firstEmptyNode = segment->firstEmptyNode != nullptr ? getNextNode(segment->firstEmptyNode) : nullptr;
        return result;
    }
    template<typename T>
    void jpool<T>::jpool_empty_nodes_getter_pointer::returnNode(pool_segment* segment, type* node)
    {
        setNextNode(node, segment->firstEmptyNode);
        segment->firstEmptyNode = node;
    }
}
