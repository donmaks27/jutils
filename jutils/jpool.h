// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "type_defines.h"
#include "type_checks.h"

#include <limits>
#include <stdexcept>

namespace jutils
{
    template<typename T, TEMPLATE_ENABLE(is_not_same<T, void>)>
    class jpool
    {
    public:

        using type = T;

        jpool() = default;
        jpool(const int32 size) { init(size); }
        jpool(const jpool&) = delete;
        jpool(jpool&&) = delete;
        ~jpool() { clear(); }

        jpool& operator=(const jpool&) = delete;
        jpool& operator=(jpool&&) = delete;

        bool isValid() const { return poolSize > 0; }

        bool init(const int32 size)
        {
            if (isValid() || (size <= 0))
            {
                return false;
            }
            initializePoolData(size);
            return true;
        }
        void clear()
        {
            if (isValid())
            {
                emptyNodesList = jpool_allocator_empty_nodes_list();
                ::operator delete(poolData, sizeof(T) * poolSize);
                poolData = nullptr;
                poolSize = 0;
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

        template<typename IndexType>
        class jpool_allocator_empty_nodes_list_index
        {
        public:

            type* firstPoolNode = nullptr;
            type* firstEmptyNode = nullptr;

            void init(int32 size, type* data);
            type* getFreeNode();
            void returnNode(type* node);

            static void setNextNode(type* node, const IndexType index) { memcpy(node, &index, sizeof(IndexType)); }
            static IndexType getNextNode(const type* node);
        };
        class jpool_allocator_empty_nodes_list_pointer
        {
        public:

            type* firstEmptyNode = nullptr;

            void init(int32 size, type* data);
            type* getFreeNode();
            void returnNode(type* node);

            static void setNextNode(type* node, const type* nextNode) { memcpy(node, &nextNode, sizeof(type*)); }
            static type* getNextNode(const type* node);
        };
        class jpool_allocator_empty_nodes_list : public std::conditional_t<sizeof(type) >= sizeof(type*), jpool_allocator_empty_nodes_list_pointer,
            jpool_allocator_empty_nodes_list_index<std::conditional_t<sizeof(type) == 1, uint8, std::conditional_t<sizeof(type) < 4, uint16, uint32>>>
        > {};


        int32 poolSize = 0;
        type* poolData = nullptr;
        jpool_allocator_empty_nodes_list emptyNodesList;


        void initializePoolData(const int32 size)
        {
            poolSize = size;
            poolData = static_cast<type*>(::operator new(sizeof(T) * size));
            emptyNodesList.init(poolSize, poolData);
        }
        type* allocateObjectNode()
        {
            type* result = emptyNodesList.getFreeNode();
            if (result != nullptr)
            {
                poolSize++;
            }
            return result;
        }
        void deallocateObjectNode(type* node)
        {
            if (node != nullptr)
            {
                emptyNodesList.returnNode(node);
                poolSize--;
            }
        }
    };

    template<typename T, TEMPLATE_ENABLE_IMPL(is_not_same<T, void>) Condition>
    template<typename IndexType>
    void jpool<T, Condition>::jpool_allocator_empty_nodes_list_index<IndexType>::init(const int32 size, type* data)
    {
        if (size > std::numeric_limits<IndexType>::max())
        {
            throw std::length_error("Size to big for index type!");
        }

        for (int32 i = 0; i < size - 1; i++)
        {
            setNextNode(data + i, i + 2);
        }
        setNextNode(data + size - 1, 0);

        firstPoolNode = data;
        firstEmptyNode = firstPoolNode;
    }
    template<typename T, TEMPLATE_ENABLE_IMPL(is_not_same<T, void>) Condition>
    template<typename IndexType>
    T* jpool<T, Condition>::jpool_allocator_empty_nodes_list_index<IndexType>::getFreeNode()
    {
        if (firstEmptyNode == nullptr)
        {
            return nullptr;
        }

        type* result = firstEmptyNode;
        const IndexType nextNodeIndex = getNextNode(firstEmptyNode);
        if (nextNodeIndex <= 0)
        {
            firstEmptyNode = nullptr;
        }
        else
        {
            firstEmptyNode = firstPoolNode + nextNodeIndex - 1;
        }
        return result;
    }
    template<typename T, TEMPLATE_ENABLE_IMPL(is_not_same<T, void>) Condition>
    template<typename IndexType>
    void jpool<T, Condition>::jpool_allocator_empty_nodes_list_index<IndexType>::returnNode(type* node)
    {
        setNextNode(node, firstEmptyNode != nullptr ? firstEmptyNode - firstPoolNode + 1 : 0);
        firstEmptyNode = node;
    }
    template<typename T, TEMPLATE_ENABLE_IMPL(is_not_same<T, void>) Condition>
    template <typename IndexType>
    IndexType jpool<T, Condition>::jpool_allocator_empty_nodes_list_index<IndexType>::getNextNode(const type* node)
    {
        IndexType index = 0;
        memcpy(&index, node, sizeof(IndexType));
        return index;
    }

    template<typename T, TEMPLATE_ENABLE_IMPL(is_not_same<T, void>) Condition>
    void jpool<T, Condition>::jpool_allocator_empty_nodes_list_pointer::init(const int32 size, type* data)
    {
        for (int32 i = 0; i < size - 1; i++)
        {
            setNextNode(data + i, data + i + 1);
        }
        setNextNode(data + size - 1, nullptr);

        firstEmptyNode = data;
    }
    template<typename T, TEMPLATE_ENABLE_IMPL(is_not_same<T, void>) Condition>
    T* jpool<T, Condition>::jpool_allocator_empty_nodes_list_pointer::getFreeNode()
    {
        if (firstEmptyNode == nullptr)
        {
            return nullptr;
        }

        type* result = firstEmptyNode;
        firstEmptyNode = getNextNode(firstEmptyNode);
        return result;
    }
    template<typename T, TEMPLATE_ENABLE_IMPL(is_not_same<T, void>) Condition>
    void jpool<T, Condition>::jpool_allocator_empty_nodes_list_pointer::returnNode(type* node)
    {
        setNextNode(node, firstEmptyNode);
        firstEmptyNode = node;
    }
    template<typename T, TEMPLATE_ENABLE_IMPL(is_not_same<T, void>) Condition>
    T* jpool<T, Condition>::jpool_allocator_empty_nodes_list_pointer::getNextNode(const type* node)
    {
        type* nextNode = nullptr;
        memcpy(&nextNode, node, sizeof(type*));
        return nextNode;
    }
}
