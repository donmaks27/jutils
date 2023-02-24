// Copyright © 2021-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"
#include "jpool_pointer.h"

namespace jutils
{
    template<typename T, typename UIDType = uint32>
    class jpool
    {
    public:

        using type = T;
        using uid_type = UIDType;
        using pointer_type = jpool_pointer<uid_type>;
        static constexpr uint32 maxSegmentSize = sizeof(type) >= sizeof(type*)
            ? std::numeric_limits<uint32>::max()
            : static_cast<uint32>(std::numeric_limits<
				std::conditional_t<sizeof(type) >= 4, uint32, std::conditional_t<sizeof(type) >= 2, uint16, uint8>>
    		>::max()) - static_cast<uint32>(1);

        constexpr jpool() : jpool(64) {}
        constexpr explicit jpool(const uint32 segmentSize) : segmentSize(jutils::math::clamp(segmentSize, 1, maxSegmentSize)) {}
        explicit jpool(const uint32 segmentSize, const int32 defaultSegmentsCount = 0)
            : jpool(segmentSize)
        {
            if (defaultSegmentsCount > 0)
            {
                segments.reserve(defaultSegmentsCount);
                for (int32 index = 0; index < defaultSegmentsCount; index++)
                {
                    _createSegment();
                }
            }
        }
        jpool(const jpool&) = delete;
        jpool(jpool&& otherPool) noexcept
        {
            segmentSize = otherPool.segmentSize;
            segments = std::move(otherPool.segments);
        }
        ~jpool()
        {
            clear();
        }
        
        jpool& operator=(const jpool&) = delete;
        jpool& operator=(jpool&& otherPool) noexcept
        {
            clear();
            segmentSize = otherPool.segmentSize;
            segments = std::move(otherPool.segments);
            return *this;
        }
        
        bool isValid(const pointer_type& pointer) const;
        type* get(const pointer_type& pointer) const;

        template<typename... Args>
        pointer_type allocate(Args&&... args);
        void deallocate(const pointer_type& pointer);
        void clear();

    private:

        struct segment_type
        {
            type* nodes = nullptr;
            juid<uid_type>* nodeUIDs = nullptr;
            bool* nodeFlags = nullptr;

            type* firstEmptyNode = nullptr;
        };
        
        class empty_nodes_list_pointer
        {
        public:
            static void init(segment_type& segment, uint32 size);
            static type* pull(segment_type& segment);
            static void push(segment_type& segment, type* node);
        private:
            static void _setNextNode(type* node, const type* nextNode) { std::memcpy(node, &nextNode, sizeof(type*)); }
            static type* _getNextNode(const type* node)
            {
                type* nextNode = nullptr;
                std::memcpy(&nextNode, node, sizeof(type*));
                return nextNode;
            }
        };
        template<typename IndexType>
        class empty_nodes_list_index
        {
            using index_type = IndexType;
        public:
            static void init(segment_type& segment, uint32 size);
            static type* pull(segment_type& segment);
            static void push(segment_type& segment, type* node);
        private:
            static void _setNextNode(type* node, const index_type index) { std::memcpy(node, &index, sizeof(index_type)); }
            static index_type _getNextNode(const type* node)
            {
                index_type index = 0;
                std::memcpy(&index, node, sizeof(index_type));
                return index;
            }
        };
        using empty_nodes_list = std::conditional_t<sizeof(type) >= sizeof(type*),
            empty_nodes_list_pointer,
            empty_nodes_list_index<std::conditional_t<sizeof(type) >= 4,
                uint32,
                std::conditional_t<sizeof(type) >= 2,uint16, uint8>
            >>
        >;
        
        jarray<segment_type> segments;
        uint32 segmentSize = 0;


        int32 _createSegment();
        void _clearSegment(int32 segmentIndex);

        pointer_type _pullNode();
        type* _getNode(const pointer_type& pointer) const;
        void _pushNode(const pointer_type& pointer);
    };

    template<typename T, typename UIDType>
    void jpool<T, UIDType>::empty_nodes_list_pointer::init(segment_type& segment, const uint32 size)
    {
        for (uint32 index = 0; index < size - 1; index++)
        {
	        empty_nodes_list_pointer::_setNextNode(segment.nodes + index, segment.data + index + 1);
            segment.nodeFlags[index] = false;
        }
        empty_nodes_list_pointer::_setNextNode(segment.nodes + size - 1, nullptr);
        segment.nodeFlags[size - 1] = false;
        segment.firstEmptyNode = segment.nodes;
    }
    template <typename T, typename UIDType>
    typename jpool<T, UIDType>::type* jpool<T, UIDType>::empty_nodes_list_pointer::pull(segment_type& segment)
    {
        if (segment.firstEmptyNode == nullptr)
        {
	        return nullptr;
        }
        type* result = segment.firstEmptyNode;
        segment.firstEmptyNode = empty_nodes_list_pointer::_getNextNode(segment.firstEmptyNode);
        return result;
    }
    template <typename T, typename UIDType>
    void jpool<T, UIDType>::empty_nodes_list_pointer::push(segment_type& segment, type* node)
    {
        empty_nodes_list_pointer::_setNextNode(node, segment.firstEmptyNode);
        segment.firstEmptyNode = node;
    }

    template <typename T, typename UIDType>
    template <typename IndexType>
    void jpool<T, UIDType>::empty_nodes_list_index<IndexType>::init(segment_type& segment, const uint32 size)
    {
        for (uint32 index = 0; index < size - 1; index++)
        {
            // Store (index + 1) of next node, 0 is invalid index
	        empty_nodes_list_index::_setNextNode(segment.nodes + index, index + 2);
            segment.nodeFlags[index] = false;
        }
        empty_nodes_list_index::_setNextNode(segment.nodes + size - 1, 0);
        segment.nodeFlags[size - 1] = false;
        segment.firstEmptyNode = segment.nodes;
    }
    template <typename T, typename UIDType>
    template <typename IndexType>
    typename jpool<T, UIDType>::type* jpool<T, UIDType>::empty_nodes_list_index<IndexType>::pull(segment_type& segment)
    {
        if (segment.firstEmptyNode == nullptr)
        {
	        return nullptr;
        }
        type* result = segment.firstEmptyNode;
        const index_type nextNodeIndex = empty_nodes_list_index::_getNextNode(segment.firstEmptyNode);
        segment.firstEmptyNode = nextNodeIndex > 0 ? segment.nodes + nextNodeIndex - 1 : nullptr;
        return result;
    }
    template <typename T, typename UIDType>
    template <typename IndexType>
    void jpool<T, UIDType>::empty_nodes_list_index<IndexType>::push(segment_type& segment, type* node)
    {
        const index_type nextNodeIndex = segment.firstEmptyNode != nullptr ? segment.firstEmptyNode - segment.nodes + 1 : 0;
        empty_nodes_list_index::_setNextNode(node, nextNodeIndex);
        segment.firstEmptyNode = node;
    }

    template <typename T, typename UIDType>
    int32 jpool<T, UIDType>::_createSegment()
    {
        segment_type& segment = segments.addDefault();
        segment.nodes = jutils::memory::allocate<type>(segmentSize);
        segment.nodeUIDs = new juid<uid_type>[segmentSize];
        segment.nodeFlags = new bool[segmentSize];
        empty_nodes_list::init(segment, segmentSize);
        return segments.getSize() - 1;
    }
    template <typename T, typename UIDType>
    void jpool<T, UIDType>::_clearSegment(const int32 segmentIndex)
    {
        if (!segments.isValidIndex(segmentIndex))
        {
	        return;
        }

        segment_type& segment = segments[segmentIndex];
        for (int32 index = 0; index < segmentSize; index++)
        {
	        if (segment.nodeFlags[index])
	        {
		        jutils::memory::destruct(segment.nodes[index]);
	        }
        }
        jutils::memory::deallocate(segment.nodes, segmentSize);
        delete[] segment.nodeUIDs;
        delete[] segment.nodeFlags;
    }

    template <typename T, typename UIDType>
    typename jpool<T, UIDType>::pointer_type jpool<T, UIDType>::_pullNode()
    {
        int32 segmentIndex = -1;
        for (int32 index = 0; index < segments.getSize(); index++)
        {
	        if (segments[index].firstEmptyNode != nullptr)
	        {
		        segmentIndex = index;
                break;
	        }
        }
        if (segmentIndex == -1)
        {
	        segmentIndex = this->_createSegment();
        }

        segment_type& segment = segments[segmentIndex];
        type* node = empty_nodes_list::pull(segment);
        const uint32 nodeIndex = static_cast<uint32>(node - segment.nodes);
        const uid_type UID = segment.nodeUIDs[nodeIndex].getUID();
        segment.nodeFlags[nodeIndex] = true;
        return { segmentIndex, nodeIndex, UID };
    }
    template <typename T, typename UIDType>
    typename jpool<T, UIDType>::type* jpool<T, UIDType>::_getNode(const pointer_type& pointer) const
    {
	    return segments[pointer.segmentIndex].nodes[pointer.nodeIndex];
    }
    template <typename T, typename UIDType>
    void jpool<T, UIDType>::_pushNode(const pointer_type& pointer)
    {
        segment_type& segment = segments[pointer.segmentIndex];
        if (segment.nodeUIDs[pointer.nodeIndex].generateUID() != juid<uid_type>::invalidUID)
        {
	        empty_nodes_list::push(segment, segment.nodes + pointer.nodeIndex);
        }
        segment.nodeFlags[pointer.nodeIndex] = false;
    }

    template <typename T, typename UIDType>
    bool jpool<T, UIDType>::isValid(const pointer_type& pointer) const
    {
        return segments.isValidIndex(pointer.segmentIndex) && (pointer.nodeIndex < segmentSize) &&
            segments[pointer.segmentIndex].nodeFlags[pointer.nodeIndex] &&
            (pointer.UID == segments[pointer.segmentIndex].nodeUIDs[pointer.nodeIndex].getUID());
    }
    template <typename T, typename UIDType>
    typename jpool<T, UIDType>::type* jpool<T, UIDType>::get(const pointer_type& pointer) const
    {
        return this->isValid(pointer) ? this->_getNode(pointer) : nullptr;
    }

    template <typename T, typename UIDType>
    template <typename ... Args>
    typename jpool<T, UIDType>::pointer_type jpool<T, UIDType>::allocate(Args&&... args)
    {
        const pointer_type pointer = this->_pullNode();
        if (pointer.isValid())
        {
	        jutils::memory::construct(this->_getNode(pointer), std::forward<Args>(args)...);
        }
        return pointer;
    }
    template <typename T, typename UIDType>
    void jpool<T, UIDType>::deallocate(const pointer_type& pointer)
    {
        if (this->isValid(pointer))
        {
	        jutils::memory::destruct(this->_getNode(pointer));
            this->_pushNode(pointer);
        }
    }
    template <typename T, typename UIDType>
    void jpool<T, UIDType>::clear()
    {
	    for (int32 index = 0; index < segments.getSize(); index++)
	    {
		    _clearSegment(index);
	    }
	    segments.clear();
    }
}
