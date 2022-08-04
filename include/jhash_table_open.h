// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "math/hash.h"
#include "jmemory.h"

namespace jutils
{
    template<typename T, TEMPLATE_ENABLE(math::hash::hash_info<T>::has_hash)>
    class jhash_table_open
    {
    public:

        using type = T;
        using hash_type = typename math::hash::hash_info<type>::hash_type;
        using index_type = int32;

        friend class iterator;
        friend class const_iterator;
        class const_iterator
        {
            friend jhash_table_open;

        public:
            const_iterator() = default;
            const_iterator(const const_iterator&) = default;
        protected:
            const_iterator(const jhash_table_open* tablePtr, const index_type index)
                : tablePtr(tablePtr), index(index)
            {}

        public:

            const type& operator*() const { return tablePtr->data[index].object; }
            const type* operator->() const { return _isValid() ? &tablePtr->data[index].object : nullptr; }
            
            const_iterator& operator++() { index++; return *this; }
            const_iterator operator++(int) { const_iterator temp = *this; ++*this; return temp; }

            const_iterator& operator--() { index--; return *this; }
            const_iterator operator--(int) { const_iterator temp = *this; --*this; return temp; }
            
            const_iterator& operator+=(const index_type offset) { index += offset; return *this; }
            const_iterator& operator-=(const index_type offset) { index -= offset; return *this; }

            const_iterator operator+(const index_type offset) const { return { tablePtr, index + offset }; }
            const_iterator operator-(const index_type offset) const { return { tablePtr, index - offset }; }

            bool operator==(const const_iterator& iter) const { return _isValid() ? (tablePtr == iter.tablePtr) && (index == iter.index) : !iter._isValid(); }
            bool operator!=(const const_iterator& iter) const { return !this->operator==(iter); }

        protected:

            const jhash_table_open* tablePtr = nullptr;
            index_type index = -1;


            bool _isValid() const { return (tablePtr != nullptr) && math::isWithin(index, 0, tablePtr->size - 1) && tablePtr->data[index].isValid(); }
        };
        class iterator : public const_iterator
        {
            friend jhash_table_open;

        public:
            iterator() = default;
            iterator(const iterator&) = default;
        protected:
            iterator(const jhash_table_open* tablePtr, const index_type index)
                : const_iterator(tablePtr, index)
            {}

        public:

            type& operator*() const { return this->tablePtr->data[this->index].object; }
            type* operator->() const { return this->_isValid() ? &this->tablePtr->data[this->index].object : nullptr; }
            
            iterator& operator++() { this->index++; return *this; }
            iterator operator++(int) { iterator temp = *this; ++*this; return temp; }

            iterator& operator--() { this->index--; return *this; }
            iterator operator--(int) { iterator temp = *this; --*this; return temp; }
            
            iterator& operator+=(const index_type offset) { this->index += offset; return *this; }
            iterator& operator-=(const index_type offset) { this->index -= offset; return *this; }

            iterator operator+(const index_type offset) const { return { this->tablePtr, this->index + offset }; }
            iterator operator-(const index_type offset) const { return { this->tablePtr, this->index - offset }; }
        };

        jhash_table_open()
            : jhash_table_open(min_table_size)
        {}
        explicit jhash_table_open(const index_type capacity)
        {
            _updateDataSize(math::max(capacity, min_table_size));
        }
        jhash_table_open(std::initializer_list<type> list)
        {
            if (list.size() > 0)
            {
                append(list);
            }
            else
            {
                _updateDataSize(min_table_size);
            }
        }
        jhash_table_open(const jhash_table_open& table)
        {
            append(table);
        }
        jhash_table_open(jhash_table_open&& table) noexcept
        {
            data = table.data;
            size = table.size;
            objectCount = table.objectCount;

            table.data = nullptr;
            table.size = 0;
            table.objectCount = 0;
        }
        ~jhash_table_open()
        {
            _clearAllData();
        }

        jhash_table_open& operator=(std::initializer_list<type> list)
        {
            clear();
            append(list);
            return *this;
        }
        jhash_table_open& operator=(const jhash_table_open& table)
        {
            if (this != &table)
            {
                clear();
                append(table);
            }
            return *this;
        }
        jhash_table_open& operator=(jhash_table_open&& table) noexcept
        {
            _clearAllData();

            data = table.data;
            size = table.size;
            objectCount = table.objectCount;

            table.data = nullptr;
            table.size = 0;
            table.objectCount = 0;

            return *this;
        }

        index_type getSize() const { return objectCount; }
        bool isEmpty() const { return getSize() == 0; }

        iterator begin() noexcept { return iterator(this, 0); }
        iterator end() noexcept { return iterator(this, size); }

        const_iterator begin() const noexcept { return const_iterator(this, 0); }
        const_iterator end() const noexcept { return const_iterator(this, size); }

        template<typename KeyType>
        type* find(const KeyType& key)
        {
            const index_type index = _findIndex(key);
            return index >= 0 ? &data[index].object : nullptr;
        }
        template<typename KeyType>
        const type* find(const KeyType& key) const
        {
            const index_type index = _findIndex(key);
            return index >= 0 ? &data[index].object : nullptr;
        }
        template<typename KeyType>
        bool contains(const KeyType& key) const { return _findIndex(key) >= 0; }

        void reserve(const index_type capacity) { _updateDataSize(capacity); }

        template<typename KeyType, typename... Args>
        type& put(const KeyType& key, Args&&... args) { return _putValue(false, key, std::forward<Args>(args)...); }
        template<typename KeyType, typename... Args>
        type& set(const KeyType& key, Args&&... args) { return _putValue(true, key, std::forward<Args>(args)...); }
        
        type& add(const type& value, const bool shouldOverride = true) { return shouldOverride ? set(value, value) : put(value, value); }
        type& add(type&& value, const bool shouldOverride = true) { return shouldOverride ? set(value, std::move(value)) : put(value, std::move(value)); }

        void append(std::initializer_list<type> list, const bool shouldOverride = true)
        {
            const index_type listSize = static_cast<index_type>(list.size());
            index_type newSize = size;
            while (newSize < (objectCount + listSize))
            {
                newSize *= 2;
            }

            reserve(newSize);
            for (const auto& value : list)
            {
                add(value, shouldOverride);
            }
        }
        void append(const jhash_table_open& table, const bool shouldOverride = true)
        {
            if ((this != &table) && !table.isEmpty())
            {
                index_type newSize = size;
                while (newSize < (objectCount + table.size))
                {
                    newSize *= 2;
                }

                reserve(newSize);
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
                node& node = data[index];
                if (node.isValid())
                {
                    _destroyObject(data + index);
                }
                node.markAsEmpty();
            }
            objectCount = 0;
        }

        jhash_table_open& operator+=(const type& value)
        {
            add(value);
            return *this;
        }
        jhash_table_open& operator+=(type&& value)
        {
            add(std::move(value));
            return *this;
        }
        jhash_table_open& operator+=(std::initializer_list<type> list)
        {
            append(list);
            return *this;
        }
        jhash_table_open& operator+=(const jhash_table_open& tree)
        {
            append(tree);
            return *this;
        }

    private:

        static constexpr index_type min_table_size = 8;

        struct node
        {
            type object;
            hash_type cachedHash = 0;
            // 1 - object exists, 2 - object deleted, other - no object
            uint8 flags = 0;

            bool isValid() const { return flags == 1; }
            bool isDeleted() const { return flags == 2; }
            bool isEmpty() const { return !isValid() && !isDeleted(); }

            void markAsEmpty() { flags = 0; }
            void markAsValid() { flags = 1; }
            void markAsDeleted() { flags = 2; }
        };

        node* data = nullptr;
        index_type size = 0;
        index_type objectCount = 0;


        static node* _allocateMemory(const index_type size) { return jutils::memory::allocate<node>(size); }
        static void _deallocateMemory(node* data, const index_type size) { jutils::memory::deallocate(data, size); }

        template<typename... Args>
        static void _constructObject(node* node, Args&&... args) { jutils::memory::construct(&node->object, std::forward<Args>(args)...); }
        static void _destroyObject(node* node) { jutils::memory::destruct(&node->object); }

        static void _copyObject(node* srcNode, node* dstNode, const bool dstObjectExists = false)
        {
            if (std::is_trivially_copyable_v<type>)
            {
                ::memmove(&dstNode->object, &srcNode->object, sizeof(type));
            }
            else if (dstObjectExists)
            {
                dstNode->object = std::move(srcNode->object);
            }
            else
            {
                _constructObject(dstNode, std::move(srcNode->object));
            }
        }
        template<typename KeyType>
        static bool _compareObjects(const type& object, const KeyType& key) { return object == key; }
        template<typename KeyType, TEMPLATE_ENABLE(math::hash::hash_info<KeyType>::has_hash)>
        static constexpr hash_type _getObjectHash(const KeyType& key) { return math::hash::getHash(key); }
        
        template<typename KeyType>
        index_type _findIndex(const KeyType& key) const;
        template<typename KeyType>
        static index_type _findIndexForInsert(const node* data, index_type size, hash_type hash, const KeyType& key);

        void _updateDataSize(index_type newSize);

        // Return false if key was exist before
        template<typename KeyType>
        bool _putKey(const KeyType& key, index_type& outIndex);
        template<typename KeyType, typename... Args>
        type& _putValue(bool overrideValue, const KeyType& key, Args&&... args);

        template<typename KeyType>
        void _removeKey(const KeyType& key);

        void _clearAllData()
        {
            if (data != nullptr)
            {
                clear();
                _deallocateMemory(data, size);

                data = nullptr;
                size = 0;
            }
        }
    };

    template<typename T, TEMPLATE_ENABLE_IMPL(math::hash::hash_info<T>::has_hash) Condition>
    template<typename KeyType>
    typename jhash_table_open<T, Condition>::index_type jhash_table_open<T, Condition>::_findIndex(const KeyType& key) const
    {
        if (size > 0)
        {
            const hash_type hash = _getObjectHash(key);
            const index_type initialIndex = hash % size;

            index_type index = initialIndex;
            do
            {
                const node& node = data[index];
                if (node.isEmpty())
                {
                    return -1;
                }
                if (node.isValid() && (node.cachedHash == hash) && _compareObjects(node.object, key))
                {
                    return index;
                }
                index = (index + 1) % size;
            }
            while (index != initialIndex);
        }
        return -1;
    }
    template<typename T, TEMPLATE_ENABLE_IMPL(math::hash::hash_info<T>::has_hash) Condition>
    template<typename KeyType>
    typename jhash_table_open<T, Condition>::index_type jhash_table_open<T, Condition>::_findIndexForInsert(const node* data, const index_type size, 
        const hash_type hash, const KeyType& key)
    {
        if (size > 0)
        {
            const index_type initialIndex = hash % size;

            index_type index = initialIndex;
            do
            {
                const node& node = data[index];
                if (node.isEmpty() || node.isDeleted())
                {
                    return index;
                }
                if ((node.cachedHash == hash) && _compareObjects(node.object, key))
                {
                    return index;
                }
                index = (index + 1) % size;
            }
            while (index != initialIndex);
        }
        return -1;
    }

    template<typename T, TEMPLATE_ENABLE_IMPL(math::hash::hash_info<T>::has_hash) Condition>
    void jhash_table_open<T, Condition>::_updateDataSize(const index_type newSize)
    {
        if (newSize <= size)
        {
            return;
        }

        node* newData = _allocateMemory(newSize);
        for (index_type index = 0; index < newSize; index++)
        {
            newData[index].markAsEmpty();
        }

        if (size > 0)
        {
            for (index_type index = 0; index < size; index++)
            {
                const node& srcNode = data[index];
                if (srcNode.isValid())
                {
                    const index_type newIndex = _findIndexForInsert(newData, newSize, srcNode.cachedHash, srcNode.object);
                    node& dstNode = newData[newIndex];
                    dstNode.markAsValid();
                    dstNode.cachedHash = srcNode.cachedHash;
                    _copyObject(data + index, newData + newIndex);

                    _destroyObject(data + index);
                }
            }

            _deallocateMemory(data, size);
        }

        data = newData;
        size = newSize;
    }

    template<typename T, TEMPLATE_ENABLE_IMPL(math::hash::hash_info<T>::has_hash) Condition>
    template<typename KeyType>
    bool jhash_table_open<T, Condition>::_putKey(const KeyType& key, index_type& outIndex)
    {
        const hash_type hash = _getObjectHash(key);
        outIndex = _findIndexForInsert(data, size, hash, key);
        if (outIndex == -1)
        {
            _updateDataSize(size * 2);
            outIndex = _findIndexForInsert(data, size, hash, key);
        }

        node& node = data[outIndex];
        if (node.isValid())
        {
            return false;
        }

        node.markAsValid();
        node.cachedHash = hash;
        return true;
    }
    template<typename T, TEMPLATE_ENABLE_IMPL(math::hash::hash_info<T>::has_hash) Condition>
    template<typename KeyType, typename... Args>
    typename jhash_table_open<T, Condition>::type& jhash_table_open<T, Condition>::_putValue(const bool overrideValue, const KeyType& key, 
        Args&&... args)
    {
        index_type index = -1;
        const bool keyWasCreated = _putKey(key, index);
        if (keyWasCreated)
        {
            objectCount++;
            _constructObject(data + index, std::forward<Args>(args)...);
        }
        else if (overrideValue)
        {
            data[index].object = type(std::forward<Args>(args)...);
        }
        return data[index].object;
    }

    template<typename T, TEMPLATE_ENABLE_IMPL(math::hash::hash_info<T>::has_hash) Condition>
    template<typename KeyType>
    void jhash_table_open<T, Condition>::_removeKey(const KeyType& key)
    {
        const index_type index = _findIndex(key);
        if (index >= 0)
        {
            _destroyObject(data + index);
            data[index].markAsDeleted();

            objectCount--;
        }
    }

    template<typename T>
    jhash_table_open<T> operator+(const jhash_table_open<T>& container, const T& value) { return jhash_table_open<T>(container) += value; }
    template<typename T>
    jhash_table_open<T> operator+(const T& value, const jhash_table_open<T>& container) { return container + value; }
    template<typename T>
    jhash_table_open<T> operator+(const jhash_table_open<T>& container1, const jhash_table_open<T>& container2) { return jhash_table_open<T>(container1) += container2; }
}
