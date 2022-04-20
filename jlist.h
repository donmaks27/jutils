// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jlist_allocator.h"
#include "math/math.h"

#include <stdexcept>

namespace jutils
{
    template<typename T>
    class jlist
    {
    public:

        using type = T;
        using index_type = int32;

    private:

        struct list_node
        {
            type object;
            list_node* nextNode = nullptr;
            list_node* prevNode = nullptr;

            void markUnused() { prevNode = this; }
            bool isUnused() const { return prevNode == this; }
        };

    public:

        friend class iterator;
        friend class const_iterator;
        class const_iterator
        {
            friend jlist;

        public:
            const_iterator() = default;
            const_iterator(const const_iterator&) = default;
        protected:
            const_iterator(list_node* nodePtr)
                : listNodePtr(nodePtr)
            {}

        public:

            const type& operator*() const { return listNodePtr->object; }
            const type* operator->() const { return _isValid() ? &listNodePtr->object : nullptr; }

            const_iterator& operator++() { _incrementIterator(); return *this; }
            const_iterator operator++(int) { iterator temp = *this; ++*this; return temp; }

            const_iterator& operator--() { _decrementIterator(); return *this; }
            const_iterator operator--(int) { iterator temp = *this; --*this; return temp; }

            const_iterator& operator+=(const index_type offset) { _offsetIterator(offset); return *this; }
            const_iterator& operator-=(const index_type offset) { _offsetIterator(-offset); return *this; }

            const_iterator operator+(const index_type offset) const { return iterator(&this) += offset; }
            const_iterator operator-(const index_type offset) const { return iterator(&this) -= offset; }

            bool operator==(const const_iterator& iter) const { return _isValid() ? (listNodePtr == iter.listNodePtr) : !iter._isValid(); }
            bool operator!=(const const_iterator& iter) const { return !this->operator==(iter); }

        protected:

            list_node* listNodePtr = nullptr;


            bool _isValid() const { return (listNodePtr != nullptr) && !listNodePtr->isUnused(); }
            void _offsetIterator(index_type offset);
            void _incrementIterator();
            void _decrementIterator();
        };
        class iterator : public const_iterator
        {
            friend jlist;

        public:
            iterator() = default;
            iterator(const iterator&) = default;
        private:
            iterator(list_node* nodePtr)
                : const_iterator(nodePtr)
            {}

        public:

            type& operator*() const { return this->listNodePtr->object; }
            type* operator->() const { return this->_isValid() ? &this->listNodePtr->object : nullptr; }

            iterator& operator++() { this->_incrementIterator(); return *this; }
            iterator operator++(int) { iterator temp = *this; ++*this; return temp; }

            iterator& operator--() { this->_decrementIterator(); return *this; }
            iterator operator--(int) { iterator temp = *this; --*this; return temp; }

            iterator& operator+=(const index_type offset) { this->_offsetIterator(offset); return *this; }
            iterator& operator-=(const index_type offset) { this->_offsetIterator(-offset); return *this; }

            iterator operator+(const index_type offset) const { return iterator(&this) += offset; }
            iterator operator-(const index_type offset) const { return iterator(&this) -= offset; }
        };

        jlist() = default;
        jlist(const index_type size)
        {
            resize(size);
        }
        jlist(const index_type size, const type& defaultValue)
        {
            resize(size, defaultValue);
        }
        jlist(std::initializer_list<type> list)
        {
            append(list);
        }
        jlist(const jlist& list)
        {
            _appendList(list);
        }
        jlist(jlist&& list) noexcept
        {
            allocator = std::move(list.allocator);

            firstNode = list.firstNode;
            lastNode = list.lastNode;
            nodeCount = list.nodeCount;
            firstUnusedNode = list.firstUnusedNode;

            list.firstNode = nullptr;
            list.lastNode = nullptr;
            list.nodeCount = 0;
            list.firstUnusedNode = nullptr;
        }
        ~jlist()
        {
            _clearAllData();
        }

        jlist& operator=(std::initializer_list<type> list)
        {
            clear();
            append(list);
            return *this;
        }
        jlist& operator=(const jlist& list)
        {
            if (this != &list)
            {
                clear();
                _appendList(list);
            }
            return *this;
        }
        jlist& operator=(jlist&& list) noexcept
        {
            _clearAllData();

            allocator = std::move(list.allocator);

            firstNode = list.firstNode;
            lastNode = list.lastNode;
            nodeCount = list.nodeCount;
            firstUnusedNode = list.firstUnusedNode;

            list.firstNode = nullptr;
            list.lastNode = nullptr;
            list.nodeCount = 0;
            list.firstUnusedNode = nullptr;

            return *this;
        }

        index_type getSize() const { return nodeCount; }
        bool isEmpty() const { return getSize() == 0; }
        bool isValidIndex(const index_type index) const { return jutils::math::isWithin(index, 0, getSize() - 1); }

        iterator begin() noexcept { return iterator(firstNode); }
        iterator end() noexcept { return iterator(); }

        const_iterator begin() const noexcept { return const_iterator(firstNode); }
        const_iterator end() const noexcept { return const_iterator(); }

        iterator getIterByIndex(const index_type index) { return isValidIndex(index) ? _getIteratorByIndex(index) : end(); }
        const_iterator getIterByIndex(const index_type index) const { return isValidIndex(index) ? _getIteratorByIndex(index) : end(); }

        iterator getIterByValue(const type& value);
        const_iterator getIterByValue(const type& value) const;

        type& get(const index_type index) { return *_getIteratorByIndex(index); }
        const type& get(const index_type index) const { return *_getIteratorByIndex(index); }
        type& operator[](const index_type index) { return get(index); }
        const type& operator[](const index_type index) const { return get(index); }

        type& getFirst() { return firstNode->object; }
        type& getLast() { return lastNode->object; }
        const type& getFirst() const { return firstNode->object; }
        const type& getLast() const { return lastNode->object; }

        type* findByIndex(const index_type index) { return isValidIndex(index) ? &*_getIteratorByIndex(index) : nullptr; }
        type* findByValue(const type& value)
        {
            auto iter = getIterByValue(value);
            return iter._isValid() ? &*iter : nullptr;
        }
        const type* findByIndex(const index_type index) const { return isValidIndex(index) ? &*_getIteratorByIndex(index) : nullptr; }
        const type* findByValue(const type& value) const
        {
            auto iter = getIterByValue(value);
            return iter._isValid() ? &*iter : nullptr;
        }

        index_type indexOf(const type& value) const;
        bool contains(const type& value) const { return getIterByValue(value)._isValid(); }
        
        void reserve(const index_type size) { _reserveNodes(size); }
        template<typename... Args>
        void resize(const index_type size, Args&&... args)
        {
            while (nodeCount < size)
            {
                _putBack(std::forward<Args>(args)...);
            }

            const_iterator iter;
            while (nodeCount > size)
            {
                iter = lastNode;
                _removeAt(iter);
            }
        }

        template<typename... Args>
        type& put(Args&&... args)
        {
            _putBack(std::forward<Args>(args)...);
            return getLast();
        }
        template<typename... Args>
        type& putAt(const const_iterator& iter, Args&&... args)
        {
            if (!iter._isValid())
            {
                throw std::out_of_range("Invalid jlist<T> iterator!");
            }
            _putAt(iter, std::forward<Args>(args)...);
            return iter.listNodePtr->prevNode->object;
        }
        template<typename... Args>
        type& putAt(const index_type index, Args&&... args) { return putAt(getIterByIndex(index), std::forward<Args>(args)...); }

        type& add(const type& value) { return put(value); }
        type& add(type&& value) { return put(std::move(value)); }
        type& addDefault() { return put(); }
        type& addUnique(const type& value)
        {
            auto iter = getIterByValue(value);
            return !iter._isValid() ? add(value) : *iter;
        }
        type& addUnique(type&& value)
        {
            auto iter = getIterByValue(value);
            return !iter._isValid() ? add(std::move(value)) : *iter;
        }

        type& addAt(const const_iterator& iter, const type& value) { return putAt(iter, value); }
        type& addAt(const const_iterator& iter, type&& value) { return putAt(iter, std::move(value)); }
        type& addDefaultAt(const const_iterator& iter) { return putAt(iter); }

        type& addAt(const index_type index, const type& value) { return putAt(index, value); }
        type& addAt(const index_type index, type&& value) { return putAt(index, std::move(value)); }
        type& addDefaultAt(const index_type index) { return putAt(index); }

        void append(std::initializer_list<type> list)
        {
            reserve(getSize() + static_cast<index_type>(list.size()));
            for (const auto& value : list)
            {
                _putBack(value);
            }
        }
        void append(const jlist& list)
        {
            if (&list != this)
            {
                _appendList(list);
            }
        }

        const_iterator removeAt(const const_iterator& iter) { return _removeAt(iter); }
        void removeAt(const index_type index)
        {
            if (isValidIndex(index))
            {
                _removeAt(_getIteratorByIndex(index));
            }
        }
        void removeFirst() { _removeAt(begin()); }
        void removeLast() { _removeAt(_getLastIterator()); }
        index_type remove(const type& value);
        template<typename Function>
        index_type removeByPredicate(Function&& predicate);

        void clear();

        jlist& operator+=(const type& value)
        {
            _putBack(value);
            return *this;
        }
        jlist& operator+=(type&& value)
        {
            _putBack(std::move(value));
            return *this;
        }
        jlist& operator+=(std::initializer_list<type> list)
        {
            append(list);
            return *this;
        }
        jlist& operator+=(const jlist& value)
        {
            append(value);
            return *this;
        }

    private:

        using allocator_type = jlist_allocator<list_node>;

        allocator_type allocator = allocator_type();

        list_node* firstNode = nullptr;
        list_node* lastNode = nullptr;
        index_type nodeCount = 0;

        list_node* firstUnusedNode = nullptr;


        template<typename... Args>
        static void _constructObject(list_node* node, Args&&... args) { jutils::memory::construct(&node->object, std::forward<Args>(args)...); }
        static void _destroyObject(list_node* node) { jutils::memory::destruct(&node->object); }

        iterator _getIteratorByIndex(const index_type index) { return begin() += index; }
        const_iterator _getIteratorByIndex(const index_type index) const { return begin() += index; }

        iterator _getLastIterator() { return iterator(lastNode); }
        const_iterator _getLastIterator() const { return const_iterator(lastNode); }

        void _allocateMoreNodes();
        void _reserveNodes(index_type count);
        list_node* _getNewNode();
        void _returnNode(list_node* node);

        template<typename... Args>
        void _putBack(Args&&... args);
        template<typename... Args>
        void _putAt(const const_iterator& iter, Args&&... args);

        void _appendList(const jlist& list)
        {
            reserve(getSize() + list.getSize());

            list_node* node = list.firstNode;
            while (node != nullptr)
            {
                _putBack(node->object);
                node = node->nextNode;
            }
        }

        const_iterator _removeAt(const const_iterator& iterator);

        void _clearAllData();
    };

    template<typename T>
    void jlist<T>::const_iterator::_offsetIterator(index_type offset)
    {
        if (offset > 0)
        {
            while ((offset > 0) && _isValid())
            {
                listNodePtr = listNodePtr->nextNode;
                offset--;
            }
        }
        else
        {
            while ((offset < 0) && _isValid())
            {
                listNodePtr = listNodePtr->prevNode;
                offset++;
            }
        }
    }
    template<typename T>
    void jlist<T>::const_iterator::_incrementIterator()
    {
        if (_isValid())
        {
            listNodePtr = listNodePtr->nextNode;
        }
    }
    template<typename T>
    void jlist<T>::const_iterator::_decrementIterator()
    {
        if (_isValid())
        {
            listNodePtr = listNodePtr->prevNode;
        }
    }

    template<typename T>
    typename jlist<T>::iterator jlist<T>::getIterByValue(const type& value)
    {
        if (!isEmpty())
        {
            for (auto iter = begin(); iter != end(); ++iter)
            {
                if (*iter == value)
                {
                    return iter;
                }
            }
        }
        return end();
    }
    template<typename T>
    typename jlist<T>::const_iterator jlist<T>::getIterByValue(const type& value) const
    {
        if (!isEmpty())
        {
            for (auto iter = begin(); iter != end(); ++iter)
            {
                if (*iter == value)
                {
                    return iter;
                }
            }
        }
        return end();
    }

    template<typename T>
    typename jlist<T>::index_type jlist<T>::indexOf(const type& value) const
    {
        if (!isEmpty())
        {
            index_type index = 0;
            for (const auto& element : *this)
            {
                if (element == value)
                {
                    return index;
                }
                index++;
            }
        }
        return -1;
    }

    template<typename T>
    void jlist<T>::_allocateMoreNodes()
    {
        list_node* segmentData = nullptr;
        index_type segmentSize = 0;
        allocator.allocateSegment(segmentData, segmentSize);

        for (index_type index = 0; index < segmentSize - 1; index++)
        {
            segmentData[index].markUnused();
            segmentData[index].nextNode = segmentData + index + 1;
        }
        segmentData[segmentSize - 1].markUnused();
        segmentData[segmentSize - 1].nextNode = firstUnusedNode;
        firstUnusedNode = segmentData;
    }
    template<typename T>
    void jlist<T>::_reserveNodes(const index_type count)
    {
        while (allocator.getSize() < count)
        {
            _allocateMoreNodes();
        }
    }
    template<typename T>
    typename jlist<T>::list_node* jlist<T>::_getNewNode()
    {
        if (firstUnusedNode == nullptr)
        {
            _allocateMoreNodes();
        }

        list_node* result = firstUnusedNode;
        firstUnusedNode = firstUnusedNode->nextNode;
        return result;
    }
    template<typename T>
    void jlist<T>::_returnNode(list_node* node)
    {
        node->markUnused();
        node->nextNode = firstUnusedNode;
        firstUnusedNode = node;
    }

    template<typename T>
    template<typename ... Args>
    void jlist<T>::_putBack(Args&&... args)
    {
        list_node* newNode = _getNewNode();
        newNode->prevNode = lastNode;
        newNode->nextNode = nullptr;
        if (lastNode != nullptr)
        {
            lastNode->nextNode = newNode;
        }
        lastNode = newNode;
        if (firstNode == nullptr)
        {
            firstNode = newNode;
        }
        nodeCount++;

        _constructObject(newNode, std::forward<Args>(args)...);
    }
    template<typename T>
    template<typename ... Args>
    void jlist<T>::_putAt(const const_iterator& iter, Args&&... args)
    {
        if (iter.listNodePtr == lastNode)
        {
            _putBack(std::forward<Args>(args)...);
        }
        else
        {
            list_node* newNode = _getNewNode();
            if (firstNode == iter.listNodePtr)
            {
                newNode->prevNode = nullptr;
                newNode->nextNode = firstNode;
                firstNode->prevNode = newNode;
                firstNode = newNode;
            }
            else
            {
                newNode->prevNode = iter.listNodePtr->prevNode;
                newNode->nextNode = iter.listNodePtr;
                iter.listNodePtr->prevNode = newNode;
            }
            nodeCount++;

            _constructObject(newNode, std::forward<Args>(args)...);
        }
    }

    template<typename T>
    typename jlist<T>::const_iterator jlist<T>::_removeAt(const const_iterator& iterator)
    {
        if (!iterator._isValid())
        {
            return iterator;
        }

        list_node* node = iterator.listNodePtr;
        list_node* next_node = node->nextNode;
        if (node != firstNode)
        {
            node->prevNode->nextNode = node->nextNode;
        }
        else
        {
            firstNode = node->nextNode;
        }
        if (node != lastNode)
        {
            node->nextNode->prevNode = node->prevNode;
        }
        else
        {
            lastNode = node->prevNode;
        }
        nodeCount--;

        _destroyObject(node);
        _returnNode(node);
        return const_iterator(next_node);
    }
    template<typename T>
    typename jlist<T>::index_type jlist<T>::remove(const type& value)
    {
        index_type count = 0;
        if (!isEmpty())
        {
            const_iterator iter = begin();
            while (iter._isValid())
            {
                if (*iter == value)
                {
                    iter = _removeAt(iter);
                    count++;
                }
                else
                {
                    ++iter;
                }
            }
        }
        return count;
    }
    template<typename T>
    template<typename Function>
    typename jlist<T>::index_type jlist<T>::removeByPredicate(Function&& predicate)
    {
        index_type count = 0;
        if (!isEmpty())
        {
            const_iterator iter = begin();
            while (iter._isValid())
            {
                if (predicate(*iter))
                {
                    iter = _removeAt(iter);
                    count++;
                }
                else
                {
                    ++iter;
                }
            }
        }
        return count;
    }

    template<typename T>
    void jlist<T>::clear()
    {
        if (nodeCount > 0)
        {
            list_node* node = firstNode;
            while (node != nullptr)
            {
                list_node* nextNode = node->nextNode;
                _returnNode(node);
                node = nextNode;
            }
            firstNode = nullptr;
            nodeCount = 0;
        }
    }
    template<typename T>
    void jlist<T>::_clearAllData()
    {
        list_node* node = firstNode;
        while (node != nullptr)
        {
            _destroyObject(node);
            node = node->nextNode;
        }

        allocator.clear();

        firstNode = nullptr;
        lastNode = nullptr;
        nodeCount = 0;
        firstUnusedNode = nullptr;
    }

    template<typename T>
    jlist<T> operator+(const jlist<T>& container, const T& value) { return jlist<T>(container) += value; }
    template<typename T>
    jlist<T> operator+(const T& value, const jlist<T>& container) { return jlist<T>(1, value) += container; }
    template<typename T>
    jlist<T> operator+(const jlist<T>& container1, const jlist<T>& container2) { return jlist<T>(container1) += container2; }
}
