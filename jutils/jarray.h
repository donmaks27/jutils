// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "type_defines.h"
#include "math/jmath.h"

#include <cstring>
#include <limits>
#include <stdexcept>

namespace jutils
{
    template<typename T>
    class jarray
    {
    public:

        using type = T;
        using index_type = int32;

        class const_iterator;
        class iterator
        {
            friend const_iterator;

        public:
            iterator() = default;
            iterator(jarray* arrayPtr, const index_type objectIndex)
                : arrayPtr(arrayPtr), objectIndex(objectIndex)
            {}
            iterator(const iterator&) = default;

            type& operator*() const { return arrayPtr->get(objectIndex); }
            type* operator->() const { return _isValid() ? &arrayPtr->get(objectIndex) : nullptr; }

            iterator& operator++() { objectIndex++; return *this; }
            iterator operator++(int) { iterator temp = *this; ++*this; return temp; }

            iterator& operator--() { objectIndex--; return *this; }
            iterator operator--(int) { iterator temp = *this; --*this; return temp; }

            bool operator==(const iterator& iter) const { return _isValid() ? (arrayPtr == iter.arrayPtr) && (objectIndex == iter.objectIndex) : !iter._isValid(); }
            bool operator!=(const iterator& iter) const { return !this->operator==(iter); }

        private:

            jarray<type>* arrayPtr = nullptr;
            index_type objectIndex = -1;


            bool _isValid() const { return (arrayPtr != nullptr) && arrayPtr->isValidIndex(objectIndex); }
        };
        class const_iterator
        {
        public:
            const_iterator() = default;
            const_iterator(const jarray* arrayPtr, const index_type objectIndex)
                : arrayPtr(arrayPtr), objectIndex(objectIndex)
            {}
            const_iterator(const iterator& iter)
                : const_iterator(iter.arrayPtr, iter.objectIndex)
            {}
            const_iterator(const const_iterator&) = default;

            const type& operator*() const { return arrayPtr->get(objectIndex); }
            const type* operator->() const { return _isValid() ? &arrayPtr->get(objectIndex) : nullptr; }

            const_iterator& operator++() { objectIndex++; return *this; }
            const_iterator operator++(int) { const_iterator temp = *this; ++*this; return temp; }

            const_iterator& operator--() { objectIndex--; return *this; }
            const_iterator operator--(int) { const_iterator temp = *this; --*this; return temp; }

            bool operator==(const const_iterator& iter) const { return _isValid() ? (arrayPtr == iter.arrayPtr) && (objectIndex == iter.objectIndex) : !iter._isValid(); }
            bool operator!=(const const_iterator& iter) const { return !this->operator==(iter); }

        private:

            const jarray<type>* arrayPtr = nullptr;
            index_type objectIndex = -1;


            bool _isValid() const { return (arrayPtr != nullptr) && arrayPtr->isValidIndex(objectIndex); }
        };

        jarray() = default;
        jarray(std::initializer_list<type> list)
        {
            append(list);
        }
        jarray(const jarray& value)
        {
            append(value);
        }
        jarray(jarray&& value) noexcept
        {
            data = value.data;
            capacity = value.capacity;
            size = value.size;

            value.data = nullptr;
            value.capacity = 0;
            value.size = 0;
        }
        ~jarray()
        {
            if (data != nullptr)
            {
                clear();
                ::operator delete(data, sizeof(type) * capacity, static_cast<std::align_val_t>(alignof(type)));
                data = nullptr;
                capacity = 0;
                size = 0;
            }
        }

        jarray& operator=(const jarray& value)
        {
            if (this != &value)
            {
                clear();
                append(value);
            }
            return *this;
        }
        jarray& operator=(jarray&& value) noexcept
        {
            clear();

            data = value.data;
            capacity = value.capacity;
            size = value.size;

            value.data = nullptr;
            value.capacity = 0;
            value.size = 0;

            return *this;
        }

        type* getData() noexcept { return size > 0 ? data : nullptr; }
        const type* getData() const noexcept { return size > 0 ? data : nullptr; }

        index_type getSize() const { return size; }
        bool isEmpty() const { return size == 0; }
        bool isValidIndex(const index_type index) const { return jutils::math::isWithin(index, 0, size - 1); }
        
        iterator begin() noexcept { return iterator(this, 0); }
        iterator end() noexcept { return iterator(this, size); }

        const_iterator begin() const noexcept { return const_iterator(this, 0); }
        const_iterator end() const noexcept { return const_iterator(this, size); }

        type& get(const index_type index) { return data[index]; }
        const type& get(const index_type index) const { return data[index]; }
        type& operator[](const index_type index) { return get(index); }
        const type& operator[](const index_type index) const { return get(index); }

        type& getFirst() { return *data; }
        type& getLast() { return data[size - 1]; }
        const type& getFirst() const { return *data; }
        const type& getLast() const { return data[size - 1]; }

        index_type indexOf(const type& value) const
        {
            if (!isEmpty())
            {
                for (index_type index = 0; index < size; index++)
                {
                    if (get(index) == value)
                    {
                        return index;
                    }
                }
            }
            return -1;
        }
        bool contains(const type& value) const { return indexOf(value) != -1; }

        type* findByIndex(const index_type index) { return isValidIndex(index) ? data + index : nullptr; }
        type* findByValue(const type& value)
        {
            const index_type index = indexOf(value);
            return index != -1 ? data + index : nullptr;
        }
        const type* findByIndex(const index_type index) const { return isValidIndex(index) ? data + index : nullptr; }
        const type* findByValue(const type& value) const
        {
            const index_type index = indexOf(value);
            return index != -1 ? data + index : nullptr;
        }

        void reserve(const index_type capacity) { _updateCapacity(capacity); }
        void resize(const index_type size) { _resize(size); }
        void resize(const index_type size, const type& defaultValue) { _resize(size, defaultValue); }

        template<typename... Args>
        type& put(Args&&... args)
        {
            _putBack(std::forward<Args>(args)...);
            return getLast();
        }
        template<typename... Args>
        type& putAt(const index_type index, Args&&... args)
        {
            _putAt(index, std::forward<Args>(args)...);
            return get(index);
        }

        type& add(const type& value) { return put(value); }
        type& add(type&& value) { return put(std::move(value)); }
        type& addDefault() { return put(); }
        type& addUnique(const type& value)
        {
            const index_type index = indexOf(value);
            return index == -1 ? add(value) : get(index);
        }
        type& addUnique(type&& value)
        {
            const index_type index = indexOf(value);
            return index == -1 ? add(std::move(value)) : get(index);
        }

        type& addAt(const index_type index, const type& value) { return putAt(index, value); }
        type& addAt(const index_type index, type&& value) { return putAt(index, std::move(value)); }
        type& addDefaultAt(const index_type index) { return putAt(index); }

        void append(std::initializer_list<type> list)
        {
            _updateCapacity(size + static_cast<index_type>(list.size()));
            index_type index = 0;
            for (auto& value : list)
            {
                _constructObject(data + size + index, value);
                index++;
            }
            size += index;
        }
        void append(const jarray& value)
        {
            if (!value.isEmpty())
            {
                _updateCapacity(size + value.size);
                for (index_type index = 0; index < value.size; index++)
                {
                    _constructObject(data + size + index, value.data[index]);
                }
                size += value.size;
            }
        }

        void removeAt(const index_type index) { _removeAt(index); }
        index_type remove(const type& value)
        {
            index_type count = 0;
            for (index_type index = getSize() - 1; index >= 0; --index)
            {
                if (get(index) == value)
                {
                    _removeAt(index);
                    count++;
                }
            }
            return count;
        }
        void clear()
        {
            for (index_type index = 0; index < size; index++)
            {
                _destroyObject(data + index);
            }
            size = 0;
        }

        jarray& operator+=(const type& value)
        {
            add(value);
            return *this;
        }
        jarray& operator+=(type&& value)
        {
            add(std::move(value));
            return *this;
        }
        jarray& operator+=(std::initializer_list<type> list)
        {
            append(list);
            return *this;
        }
        jarray& operator+=(const jarray& value)
        {
            if (this != &value)
            {
                append(value);
            }
            return *this;
        }

    private:

        type* data = nullptr;
        index_type capacity = 0;
        index_type size = 0;


        template<typename... Args>
        static void _constructObject(type* objectNode, Args&&... args) { ::new (objectNode) type(std::forward<Args>(args)...); }
        static void _destroyObject(type* objectNode) { objectNode->~T(); }

        static void _copyData(type* oldData, type* newData, const index_type newDataSize)
        {
            if (std::is_trivially_copyable_v<type>)
            {
                ::memcpy(newData, oldData, sizeof(type) * newDataSize);
            }
            else
            {
                for (index_type index = 0; index < newDataSize; index++)
                {
                    _constructObject(newData + index, std::move(oldData[index]));
                }
            }
        }
        static void _eraseData(type* data, const index_type dataSize, const index_type dataIndex)
        {
            if (std::is_trivially_copyable_v<type>)
            {
                ::memmove(data + dataIndex, data + dataIndex + 1, dataSize - dataIndex - 1);
            }
            else
            {
                for (index_type index = dataIndex + 1; index < dataSize; index++)
                {
                    data[index - 1] = std::move(data[index]);
                }
            }
            _destroyObject(data + dataSize - 1);
        }
        static void _insertData(type* data, const index_type dataSize, const index_type dataIndex)
        {
            if (std::is_trivially_copyable_v<type>)
            {
                ::memmove(data + dataIndex + 1, data + dataIndex, dataSize - dataIndex);
            }
            else
            {
                _constructObject(data + dataSize, std::move(data[dataSize - 1]));
                for (index_type index = dataSize - 1; index > dataIndex; index--)
                {
                    data[index] = std::move(data[index - 1]);
                }
            }
        }

        static index_type _copyData(type* oldData, const index_type oldDataSize, type* newData, const index_type newDataCapacity)
        {
            const index_type newDataSize = jutils::math::min(oldDataSize, newDataCapacity);
            if (newDataSize > 0)
            {
                _copyData(oldData, newData, newDataSize);
            }
            if (oldDataSize > 0)
            {
                for (index_type index = 0; index < oldDataSize; index++)
                {
                    _destroyObject(oldData + index);
                }
            }
            return newDataSize;
        }
        static index_type _copyDataBeforeInsert(type* oldData, const index_type oldDataSize, type* newData, const index_type newDataCapacity, const index_type insertIndex)
        {
            const index_type newDataSize = jutils::math::min(jutils::math::max(oldDataSize, 0) + 1, newDataCapacity);
            if (!jutils::math::isWithin(insertIndex, 0, newDataSize - 1))
            {
                return _copyData(oldData, oldDataSize, newData, newDataCapacity);
            }

            _copyData(oldData, newData, insertIndex);
            _copyData(oldData + insertIndex, newData + insertIndex + 1, newDataSize - insertIndex - 1);
            if (oldDataSize > 0)
            {
                for (index_type index = 0; index < oldDataSize; index++)
                {
                    _destroyObject(oldData + index);
                }
            }
            return newDataSize;
        }

        

        static type* _allocateMemory(const index_type size)
        {
            return size > 0 ? static_cast<type*>(::operator new(sizeof(type) * size, static_cast<std::align_val_t>(alignof(type)))) : nullptr;
        }
        static void _deallocateMemory(type* data, const index_type size)
        {
            if (size > 0)
            {
                ::operator delete(data, sizeof(type) * size, static_cast<std::align_val_t>(alignof(type)));
            }
        }
        void _setCapacity(index_type newCapacity)
        {
            newCapacity = jutils::math::max(newCapacity, 0);
            if (newCapacity == capacity)
            {
                return;
            }

            type* newData = _allocateMemory(newCapacity);
            const index_type newSize = _copyData(data, size, newData, newCapacity);
            _deallocateMemory(data, capacity);

            data = newData;
            size = newSize;
            capacity = newCapacity;
        }
        template<typename... Args>
        void _setCapacity(index_type newCapacity, const index_type insertIndex, Args&&... args)
        {
            newCapacity = jutils::math::max(newCapacity, 0);
            if (newCapacity == capacity)
            {
                return;
            }
            
            type* newData = _allocateMemory(newCapacity);
            const index_type newSize = _copyDataBeforeInsert(data, size, newData, newCapacity, insertIndex);
            if (jutils::math::isWithin(insertIndex, 0, newSize - 1))
            {
                _constructObject(newData + insertIndex, std::forward<Args>(args)...);
            }
            _deallocateMemory(data, capacity);

            data = newData;
            size = newSize;
            capacity = newCapacity;
        }
        void _updateCapacity(const index_type newCapacity)
        {
            if (newCapacity > capacity)
            {
                _setCapacity(newCapacity);
            }
        }
        int32 _calculateIncreasedCapacity() const
        {
            constexpr index_type capacityLimit = std::numeric_limits<index_type>::max();
            constexpr index_type capacityHalfLimit = std::numeric_limits<index_type>::max() / 2;
            if (capacity <= capacityHalfLimit)
            {
                return capacity > 0 ? capacity * 2 : 1;
            }
            if (capacity != capacityLimit)
            {
                if (capacity == capacityLimit - 1)
                {
                    return capacity + 1;
                }
                return capacity + jutils::math::roundUp(static_cast<float>(capacityLimit - capacity) / 2);
            }
            throw std::length_error("Overflow of jarray<T> capacity!");
        }

        template<typename... Args>
        void _resize(index_type newSize, Args&&... args)
        {
            newSize = jutils::math::max(newSize, 0);
            if (newSize == size)
            {
                return;
            }

            if (newSize > size)
            {
                _updateCapacity(newSize);
                for (index_type index = size; index < newSize; index++)
                {
                    _constructObject(data + index, std::forward<Args>(args)...);
                }
            }
            else
            {
                for (index_type index = newSize; index < size; index++)
                {
                    _destroyObject(data + index);
                }
            }
            size = newSize;
        }

        void _throwIfNotValidIndex(const index_type index) const
        {
            if (!isValidIndex(index))
            {
                throw std::out_of_range("Invalid jarray<T> index!");
            }
        }
        template<typename... Args>
        void _putBack(Args&&... args)
        {
            if (size == capacity)
            {
                _setCapacity(_calculateIncreasedCapacity());
            }
            _constructObject(data + size, std::forward<Args>(args)...);
            size++;
        }
        template<typename... Args>
        void _putAt(const index_type index, Args&&... args)
        {
            _throwIfNotValidIndex(index);
            if (size == capacity)
            {
                _setCapacity(_calculateIncreasedCapacity(), index, std::forward<Args>(args)...);
            }
            else
            {
                _insertData(data, size, index);
                data[index] = type(std::forward<Args>(args)...);
                size++;
            }
        }

        void _removeAt(const index_type index)
        {
            if (isValidIndex(index))
            {
                _eraseData(data, size, index);
                size--;
            }
        }
    };
}
