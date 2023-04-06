// Copyright © 2021-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"
#include "juid.h"

namespace jutils
{
    template<typename T>
    class jpool;

    class jpool_pointer
    {
        template<typename T>
        friend class jpool;

    public:

        using uid_type = uint32;

        constexpr jpool_pointer() = default;
        constexpr jpool_pointer(nullptr_t) : jpool_pointer() {}
        constexpr jpool_pointer(const jpool_pointer&) = default;
    private:
        constexpr jpool_pointer(const int32 segment, const uint32 node, const uid_type uid)
            : segmentIndex(segment), nodeIndex(node), UID(uid)
        {}
    public:

        constexpr jpool_pointer& operator=(nullptr_t);
        constexpr jpool_pointer& operator=(const jpool_pointer&) = default;

        constexpr bool operator==(nullptr_t) const { return (UID == juid<uid_type>::invalidUID) || (segmentIndex < 0); }
        constexpr bool operator!=(nullptr_t) const { return !operator==(nullptr); }

        constexpr bool operator==(const jpool_pointer& pointer) const;
        constexpr bool operator!=(const jpool_pointer& pointer) const { return !operator==(pointer); }

    private:

        int32 segmentIndex = -1;
	    uint32 nodeIndex = 0;
	    uid_type UID = juid<uid_type>::invalidUID;
    };

    template<typename T>
    class jpool
    {
    public:

        using type = T;
        using pointer_type = jpool_pointer;
        using uid_type = pointer_type::uid_type;
        static constexpr uint32 maxSegmentSize = sizeof(type) >= sizeof(type*)
            ? std::numeric_limits<uint32>::max()
            : static_cast<uint32>(std::numeric_limits<
				std::conditional_t<sizeof(type) >= 4, uint32, std::conditional_t<sizeof(type) >= 2, uint16, uint8>>
    		>::max()) - static_cast<uint32>(1);

        constexpr jpool() : jpool(1024) {}
        constexpr explicit jpool(const uint32 segmentSize)
            : segmentSize(jutils::math::clamp(segmentSize, 1, maxSegmentSize))
        {}
        explicit jpool(uint32 segmentSize, int32 defaultSegmentsCount = 0);
        jpool(const jpool&) = delete;
        jpool(jpool&& otherPool) noexcept
            : segments(std::move(otherPool.segments)), segmentSize(otherPool.segmentSize)
        {}
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
        pointer_type create(Args&&... args);
        void destroy(const pointer_type& pointer);
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

    constexpr jpool_pointer& jpool_pointer::operator=(nullptr_t)
    {
        segmentIndex = -1;
        nodeIndex = 0;
        UID = juid<uid_type>::invalidUID;
        return *this;
    }
    constexpr bool jpool_pointer::operator==(const jpool_pointer& pointer) const
    {
        if (*this == nullptr)
        {
            return pointer == nullptr;
        }
        return (segmentIndex == pointer.segmentIndex) && (nodeIndex == pointer.nodeIndex) && (UID == pointer.UID);
    }

    template<typename T>
    jpool<T>::jpool(const uint32 segmentSize, const int32 defaultSegmentsCount)
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

    template<typename T>
    void jpool<T>::empty_nodes_list_pointer::init(segment_type& segment, const uint32 size)
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
    template<typename T>
    typename jpool<T>::type* jpool<T>::empty_nodes_list_pointer::pull(segment_type& segment)
    {
        if (segment.firstEmptyNode == nullptr)
        {
	        return nullptr;
        }
        type* result = segment.firstEmptyNode;
        segment.firstEmptyNode = empty_nodes_list_pointer::_getNextNode(segment.firstEmptyNode);
        return result;
    }
    template<typename T>
    void jpool<T>::empty_nodes_list_pointer::push(segment_type& segment, type* node)
    {
        empty_nodes_list_pointer::_setNextNode(node, segment.firstEmptyNode);
        segment.firstEmptyNode = node;
    }

    template<typename T>
    template<typename IndexType>
    void jpool<T>::empty_nodes_list_index<IndexType>::init(segment_type& segment, const uint32 size)
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
    template<typename T>
    template<typename IndexType>
    typename jpool<T>::type* jpool<T>::empty_nodes_list_index<IndexType>::pull(segment_type& segment)
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
    template<typename T>
    template<typename IndexType>
    void jpool<T>::empty_nodes_list_index<IndexType>::push(segment_type& segment, type* node)
    {
        const index_type nextNodeIndex = segment.firstEmptyNode != nullptr ? segment.firstEmptyNode - segment.nodes + 1 : 0;
        empty_nodes_list_index::_setNextNode(node, nextNodeIndex);
        segment.firstEmptyNode = node;
    }

    template<typename T>
    int32 jpool<T>::_createSegment()
    {
        segment_type& segment = segments.addDefault();
        segment.nodes = jutils::memory::allocate<type>(segmentSize);
        segment.nodeUIDs = new juid<uid_type>[segmentSize];
        segment.nodeFlags = new bool[segmentSize];
        empty_nodes_list::init(segment, segmentSize);
        return segments.getSize() - 1;
    }
    template<typename T>
    void jpool<T>::_clearSegment(const int32 segmentIndex)
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

    template<typename T>
    typename jpool<T>::pointer_type jpool<T>::_pullNode()
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
	        segmentIndex = _createSegment();
        }

        segment_type& segment = segments[segmentIndex];
        type* node = empty_nodes_list::pull(segment);
        const uint32 nodeIndex = static_cast<uint32>(node - segment.nodes);
        const uid_type UID = segment.nodeUIDs[nodeIndex].getUID();
        segment.nodeFlags[nodeIndex] = true;
        return { segmentIndex, nodeIndex, UID };
    }
    template<typename T>
    typename jpool<T>::type* jpool<T>::_getNode(const pointer_type& pointer) const
    {
	    return segments[pointer.segmentIndex].nodes[pointer.nodeIndex];
    }
    template<typename T>
    void jpool<T>::_pushNode(const pointer_type& pointer)
    {
        segment_type& segment = segments[pointer.segmentIndex];
        if (segment.nodeUIDs[pointer.nodeIndex].generateUID() != juid<uid_type>::invalidUID)
        {
	        empty_nodes_list::push(segment, segment.nodes + pointer.nodeIndex);
        }
        segment.nodeFlags[pointer.nodeIndex] = false;
    }

    template<typename T>
    bool jpool<T>::isValid(const pointer_type& pointer) const
    {
        return segments.isValidIndex(pointer.segmentIndex) && (pointer.nodeIndex < segmentSize) &&
            segments[pointer.segmentIndex].nodeFlags[pointer.nodeIndex] &&
            (pointer.UID == segments[pointer.segmentIndex].nodeUIDs[pointer.nodeIndex].getUID());
    }
    template<typename T>
    typename jpool<T>::type* jpool<T>::get(const pointer_type& pointer) const
    {
        return isValid(pointer) ? _getNode(pointer) : nullptr;
    }

    template<typename T>
    template<typename ... Args>
    typename jpool<T>::pointer_type jpool<T>::create(Args&&... args)
    {
        const pointer_type pointer = _pullNode();
        if (pointer != nullptr)
        {
	        jutils::memory::construct(_getNode(pointer), std::forward<Args>(args)...);
        }
        return pointer;
    }
    template<typename T>
    void jpool<T>::destroy(const pointer_type& pointer)
    {
        if (isValid(pointer))
        {
	        jutils::memory::destruct(_getNode(pointer));
            _pushNode(pointer);
        }
    }
    template<typename T>
    void jpool<T>::clear()
    {
	    for (int32 index = 0; index < segments.getSize(); index++)
	    {
		    _clearSegment(index);
	    }
	    segments.clear();
    }
}
