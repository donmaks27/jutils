// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "math/hash.h"
#include "jlist_allocator.h"
#include "jarray.h"

namespace jutils
{
    template<typename T, TEMPLATE_ENABLE(math::hash::hash_info<T>::has_hash)>
    class jhash_table_chain
    {
    public:

        using type = T;
        using hash_type = typename math::hash::hash_info<type>::hash_type;
        using index_type = int32;

    private:

        struct chain_node
        {
            type object;
            hash_type cachedHash = 0;
            chain_node* nextNode = nullptr;

            void markUnused() {}
        };

    public:

        friend class iterator;
        friend class const_iterator;
        class const_iterator
        {
            friend jhash_table_chain;

        public:
            const_iterator() = default;
            const_iterator(const const_iterator&) = default;
        protected:
            const_iterator(const jhash_table_chain* table, const index_type index, chain_node* chainNode)
                : tablePtr(table), tableIndex(index), node(chainNode)
            {}
            const_iterator(const jhash_table_chain* table, const index_type index)
                : tablePtr(table), tableIndex(index)
            {
                _findNextNode();
            }

        public:

            const type& operator*() const { return node->object; }
            const type* operator->() const { return _isValid() ? &node->object : nullptr; }
            
            const_iterator& operator++() { _findNextNode(); return *this; }
            const_iterator operator++(int) { const_iterator temp = *this; ++*this; return temp; }
            
            const_iterator& operator+=(const index_type offset) { _offset(offset); return *this; }
            const_iterator operator+(const index_type offset) const { return const_iterator(*this) += offset; }

            bool operator==(const const_iterator& iter) const { return _isValid() ? (tablePtr == iter.tablePtr) && (tableIndex == iter.tableIndex) && (node == iter.node) : !iter._isValid(); }
            bool operator!=(const const_iterator& iter) const { return !this->operator==(iter); }

        protected:

            const jhash_table_chain* tablePtr = nullptr;
            index_type tableIndex = -1;
            chain_node* node = nullptr;


            bool _isValid() const { return (tablePtr != nullptr) && (tableIndex >= 0) && (tableIndex < tablePtr->size) && (node != nullptr); }
            void _findNextNode()
            {
                if ((tablePtr == nullptr) || (tableIndex < 0))
                {
                    return;
                }

                while (tableIndex < tablePtr->size)
                {
                    node = (node == nullptr) ? tablePtr->data[tableIndex].firstNode : node->nextNode;
                    if (node != nullptr)
                    {
                        return;
                    }
                    tableIndex++;
                }
            }
            void _offset(index_type offset)
            {
                while ((offset > 0) && _isValid())
                {
                    _findNextNode();
                    offset--;
                }
            }
        };
        class iterator : public const_iterator
        {
            friend jhash_table_chain;

        public:
            iterator() = default;
            iterator(const iterator&) = default;
        protected:
            iterator(const jhash_table_chain* table, const index_type index, chain_node* chainNode)
                : const_iterator(table, index, chainNode)
            {}
            iterator(const jhash_table_chain* table, const index_type index)
                : const_iterator(table, index)
            {}

        public:

            type& operator*() const { return this->node->object; }
            type* operator->() const { return this->_isValid() ? &this->node->object : nullptr; }
            
            iterator& operator++() { this->_findNextNode(); return *this; }
            iterator operator++(int) { iterator temp = *this; ++*this; return temp; }
            
            iterator& operator+=(const index_type offset) { this->_offset(offset); return *this; }
            iterator operator+(const index_type offset) const { return iterator(*this) += offset; }
        };

        jhash_table_chain()
            : jhash_table_chain(min_table_size)
        {}
        explicit jhash_table_chain(const index_type tableSize)
        {
            resizeTable(tableSize);
        }
        jhash_table_chain(std::initializer_list<type> list)
        {
            resizeTable(min_table_size);
            append(list);
        }
        jhash_table_chain(const jhash_table_chain& table)
        {
            resizeTable(table.size);
            append(table);
        }
        jhash_table_chain(jhash_table_chain&& table) noexcept
        {
            chainAllocator = std::move(table.chainAllocator);

            firstUnusedChainNode = table.firstUnusedChainNode;
            data = table.data;
            size = table.size;
            objectCount = table.objectCount;

            table.firstUnusedChainNode = nullptr;
            table.data = nullptr;
            table.size = 0;
            table.objectCount = 0;
        }
        ~jhash_table_chain()
        {
            _clearAllData();
        }

        jhash_table_chain& operator=(std::initializer_list<type> list)
        {
            clear();
            append(list);
            return *this;
        }
        jhash_table_chain& operator=(const jhash_table_chain& table)
        {
            if (this != &table)
            {
                clear();
                resizeTable(table.size);
                append(table);
            }
            return *this;
        }
        jhash_table_chain& operator=(jhash_table_chain&& table) noexcept
        {
            _clearAllData();

            chainAllocator = std::move(table.chainAllocator);

            firstUnusedChainNode = table.firstUnusedChainNode;
            data = table.data;
            size = table.size;
            objectCount = table.objectCount;

            table.firstUnusedChainNode = nullptr;
            table.data = nullptr;
            table.size = 0;
            table.objectCount = 0;

            return *this;
        }

        index_type getSize() const { return objectCount; }
        bool isEmpty() const { return getSize() == 0; }

        iterator begin() noexcept { return iterator(this, 0); }
        iterator end() noexcept { return iterator(); }

        const_iterator begin() const noexcept { return const_iterator(this, 0); }
        const_iterator end() const noexcept { return const_iterator(); }
        
        template<typename KeyType>
        type* find(const KeyType& key)
        {
            chain_node* node = _findNode(key);
            return node != nullptr ? &node->object : nullptr;
        }
        template<typename KeyType>
        const type* find(const KeyType& key) const
        {
            const chain_node* node = _findNode(key);
            return node != nullptr ? &node->object : nullptr;
        }
        template<typename KeyType>
        bool contains(const KeyType& key) const { return _findNode(key) != nullptr; }

        void reserve(const index_type capacity) { _reserveChainNodes(capacity); }
        void resizeTable(const index_type tableSize)
        {
            if (tableSize >= min_table_size)
            {
                _changeTableDataSize(tableSize);
            }
        }
        
        template<typename KeyType, typename... Args>
        type& put(const KeyType& key, Args&&... args) { return _putValue(false, key, std::forward<Args>(args)...); }
        template<typename KeyType, typename... Args>
        type& set(const KeyType& key, Args&&... args) { return _putValue(true, key, std::forward<Args>(args)...); }
        
        type& add(const type& value, const bool shouldOverride = true) { return shouldOverride ? set(value, value) : put(value, value); }
        type& add(type&& value, const bool shouldOverride = true) { return shouldOverride ? set(value, std::move(value)) : put(value, std::move(value)); }

        void append(std::initializer_list<type> list, const bool shouldOverride = true)
        {
            reserve(list.size());
            for (const auto& value : list)
            {
                add(value, shouldOverride);
            }
        }
        void append(const jhash_table_chain& table, const bool shouldOverride = true)
        {
            if ((this != &table) && !table.isEmpty())
            {
                reserve(table.getSize());
                for (const auto& value : table)
                {
                    add(value, shouldOverride);
                }
            }
        }

        template<typename KeyType>
        void remove(const KeyType& key) { _removeKey(key); }
        void clear()
        {
            for (index_type index = 0; index < size; index++)
            {
                chain_node* node = data[index].firstNode;
                while (node != nullptr)
                {
                    chain_node* nextNode = node->nextNode;
                    _destroyObject(node);
                    _returnChainNode(node);
                    node = nextNode;
                }
                data[index].firstNode = nullptr;
            }
            objectCount = 0;
        }
        
        jhash_table_chain& operator+=(const type& value)
        {
            add(value);
            return *this;
        }
        jhash_table_chain& operator+=(type&& value)
        {
            add(std::move(value));
            return *this;
        }
        jhash_table_chain& operator+=(std::initializer_list<type> list)
        {
            append(list);
            return *this;
        }
        jhash_table_chain& operator+=(const jhash_table_chain& tree)
        {
            append(tree);
            return *this;
        }

    private:

        static constexpr index_type min_table_size = 8;

        struct table_node
        {
            chain_node* firstNode = nullptr;
        };

        using chain_allocator_type = jlist_allocator<chain_node>;

        chain_allocator_type chainAllocator = chain_allocator_type();

        chain_node* firstUnusedChainNode = nullptr;

        table_node* data = nullptr;
        index_type size = 0;
        index_type objectCount = 0;


        static table_node* _allocateTableNodes(const index_type size)
        {
            table_node* data = static_cast<table_node*>(::operator new(sizeof(table_node) * size, static_cast<std::align_val_t>(alignof(table_node))));
            ::memset(data, 0, sizeof(table_node) * size);
            return data;
        }
        static void _deallocateTableNodes(table_node* data, const index_type size) { ::operator delete(data, sizeof(table_node) * size, static_cast<std::align_val_t>(alignof(table_node))); }

        template<typename... Args>
        static void _constructObject(chain_node* node, Args&&... args) { ::new (&node->object) type(std::forward<Args>(args)...); }
        static void _destroyObject(chain_node* node) { node->object.~type(); }

        template<typename KeyType>
        static bool _compareObjects(const type& object, const KeyType& key) { return object == key; }
        template<typename KeyType, TEMPLATE_ENABLE(math::hash::hash_info<KeyType>::has_hash)>
        static constexpr hash_type _getObjectHash(const KeyType& key) { return math::hash::getHash(key); }

        void _allocateMoreChainNodes()
        {
            chain_node* segmentData = nullptr;
            index_type segmentSize = 0;
            chainAllocator.allocateSegment(segmentData, segmentSize);

            for (index_type index = 0; index < segmentSize - 1; index++)
            {
                segmentData[index].markUnused();
                segmentData[index].nextNode = segmentData + index + 1;
            }
            segmentData[segmentSize - 1].markUnused();
            segmentData[segmentSize - 1].nextNode = firstUnusedChainNode;
            firstUnusedChainNode = segmentData;
        }
        void _returnChainNode(chain_node* node)
        {
            if (node != nullptr)
            {
                node->markUnused();
                node->nextNode = firstUnusedChainNode;
                firstUnusedChainNode = node;
            }
        }
        chain_node* _getChainNode()
        {
            if (firstUnusedChainNode == nullptr)
            {
                _allocateMoreChainNodes();
            }

            chain_node* node = firstUnusedChainNode;
            firstUnusedChainNode = node->nextNode;
            return node;
        }
        void _reserveChainNodes(const index_type count)
        {
            while (chainAllocator.getSize() < count)
            {
                _allocateMoreChainNodes();
            }
        }

        void _changeTableDataSize(const index_type newSize)
        {
            if ((newSize == size) || (newSize <= 0))
            {
                return;
            }

            table_node* newData = _allocateTableNodes(newSize);
            if (size > 0)
            {
                for (index_type index = 0; index < size; index++)
                {
                    table_node& node = data[index];
                    while (node.firstNode != nullptr)
                    {
                        chain_node* chainNode = node.firstNode;
                        node.firstNode = chainNode->nextNode;

                        const index_type newIndex = chainNode->cachedHash % newSize;
                        chainNode->nextNode = newData[newIndex].firstNode;
                        newData[newIndex].firstNode = chainNode;
                    }
                }
                _deallocateTableNodes(data, size);
            }

            data = newData;
            size = newSize;
        }

        template<typename KeyType>
        chain_node* _findNode(const KeyType& key) const
        {
            if (!isEmpty())
            {
                const hash_type hash = _getObjectHash(key);
                const index_type index = hash % size;

                chain_node* node = data[index].firstNode;
                while (node != nullptr)
                {
                    if ((hash == node->cachedHash) && _compareObjects(node->object, key))
                    {
                        return node;
                    }
                    node = node->nextNode;
                }
            }
            return nullptr;
        }

        // Return false if key was exist before
        template<typename KeyType>
        bool _putKey(const KeyType& key, chain_node*& outChainNode)
        {
            const hash_type hash = _getObjectHash(key);
            const index_type index = hash % size;

            table_node& tableNode = data[index];
            chain_node* chainNode = tableNode.firstNode;
            while (chainNode != nullptr)
            {
                if ((chainNode->cachedHash == hash) && _compareObjects(chainNode->object, key))
                {
                    outChainNode = chainNode;
                    return false;
                }
                chainNode = chainNode->nextNode;
            }

            outChainNode = _getChainNode();
            outChainNode->cachedHash = hash;
            outChainNode->nextNode = tableNode.firstNode;
            tableNode.firstNode = outChainNode;
            objectCount++;
            return true;
        }
        template<typename KeyType, typename... Args>
        type& _putValue(bool overrideValue, const KeyType& key, Args&&... args)
        {
            index_type index = -1;
            const bool keyWasCreated = _putKey(key, index);
            if (keyWasCreated)
            {
                _constructObject(data + index, std::forward<Args>(args)...);
            }
            else if (overrideValue)
            {
                data[index].object = type(std::forward<Args>(args)...);
            }
            return data[index].object;
        }

        template<typename KeyType>
        void _removeKey(const KeyType& key)
        {
            const hash_type hash = _getObjectHash(key);
            const index_type index = hash % size;

            chain_node* prevChainNode = nullptr;
            chain_node* chainNode = data[index].firstNode;
            while (chainNode != nullptr)
            {
                if ((chainNode->cachedHash == hash) && _compareObjects(chainNode->object, key))
                {
                    _destroyObject(chainNode);
                    if (prevChainNode != nullptr)
                    {
                        prevChainNode->nextNode = chainNode->nextNode;
                    }
                    else
                    {
                        data[index].firstNode = chainNode->nextNode;
                    }
                    _returnChainNode(chainNode);
                    objectCount--;
                    return;
                }
                prevChainNode = chainNode;
                chainNode = chainNode->nextNode;
            }
        }

        void _clearAllData()
        {
            if (size > 0)
            {
                for (index_type index = 0; index < size; index++)
                {
                    chain_node* node = data[index].firstNode;
                    while (node != nullptr)
                    {
                        _destroyObject(node);
                        node = node->nextNode;
                    }
                }

                _deallocateTableNodes(data, size);
            }

            chainAllocator.clear();

            firstUnusedChainNode = nullptr;
            data = nullptr;
            size = 0;
            objectCount = 0;
        }
    };
}
