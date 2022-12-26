// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jlist_allocator.h"

#include <functional>

namespace jutils
{
    template<typename T, typename ComparePred = std::less<>>
    class jtree_red_black
    {
    public:

        using type = T;
        using compare_predicator = ComparePred;
        using index_type = int32;

    private:

        struct tree_node
        {
            type object;

            bool isRed = true;
            tree_node* parent = nullptr;
            tree_node* childLeft = nullptr;
            tree_node* childRight = nullptr;

            void markUnused() { parent = this; }
            bool isUnused() const { return parent == this; }
        };

    public:

        friend class iterator;
        friend class const_iterator;
        class const_iterator
        {
            friend jtree_red_black;

        public:
            const_iterator() = default;
            const_iterator(const const_iterator&) = default;
        protected:
            const_iterator(tree_node* nodePtr)
                : node(nodePtr)
            {}

        public:

            const type& operator*() const { return node->object; }
            const type* operator->() const { return _isValid() ? &node->object : nullptr; }

            const_iterator& operator++() { _moveToNextNode(); return *this; }
            const_iterator operator++(int) { const_iterator temp = *this; ++*this; return temp; }

            const_iterator& operator--() { _moveToPrevNode(); return *this; }
            const_iterator operator--(int) { const_iterator temp = *this; --*this; return temp; }
            
            const_iterator& operator+=(const index_type offset) { _offsetNode(offset); return *this; }
            const_iterator& operator-=(const index_type offset) { _offsetNode(-offset); return *this; }

            const_iterator operator+(const index_type offset) const { return const_iterator(*this) += offset; }
            const_iterator operator-(const index_type offset) const { return const_iterator(*this) -= offset; }

            bool operator==(const const_iterator& iter) const { return _isValid() ? node == iter.node : !_isValid(); }
            bool operator!=(const const_iterator& iter) const { return !this->operator==(iter); }

        protected:

            tree_node* node = nullptr;


            bool _isValid() const { return (node != nullptr) && !node->isUnused(); }

            void _moveToNextNode();
            void _moveToPrevNode();
            void _offsetNode(index_type offset);
        };
        class iterator : public const_iterator
        {
            friend jtree_red_black;

        public:
            iterator() = default;
            iterator(const iterator&) = default;
        protected:
            iterator(tree_node* nodePtr)
                : const_iterator(nodePtr)
            {}

        public:

            type& operator*() const { return this->node->object; }
            type* operator->() const { return this->_isValid() ? &this->node->object : nullptr; }

            iterator& operator++() { this->_moveToNextNode(); return *this; }
            iterator operator++(int) { iterator temp = *this; ++*this; return temp; }

            iterator& operator--() { this->_moveToPrevNode(); return *this; }
            iterator operator--(int) { iterator temp = *this; --*this; return temp; }
            
            iterator& operator+=(const index_type offset) { this->_offsetNode(offset); return *this; }
            iterator& operator-=(const index_type offset) { this->_offsetNode(-offset); return *this; }

            iterator operator+(const index_type offset) const { return iterator(*this) += offset; }
            iterator operator-(const index_type offset) const { return iterator(*this) -= offset; }
        };

        jtree_red_black() = default;
        jtree_red_black(std::initializer_list<type> list)
        {
            append(list);
        }
        jtree_red_black(const jtree_red_black& value)
        {
            append(value);
        }
        jtree_red_black(jtree_red_black&& value) noexcept
            : allocator(std::move(value.allocator))
            , rootNode(value.rootNode), size(value.size)
            , firstUnusedNode(value.firstUnusedNode), unusedNodesCount(value.unusedNodesCount)
        {
            value.rootNode = nullptr;
            value.size = 0;
            value.firstUnusedNode = nullptr;
            value.unusedNodesCount = 0;
        }
        ~jtree_red_black()
        {
            _clearAllData();
        }

        jtree_red_black& operator=(std::initializer_list<type> list)
        {
            clear();
            append(list);
            return *this;
        }
        jtree_red_black& operator=(const jtree_red_black& value)
        {
            if (this != &value)
            {
                clear();
                append(value);
            }
            return *this;
        }
        jtree_red_black& operator=(jtree_red_black&& value) noexcept
        {
            _clearAllData();

            allocator = std::move(value.allocator);

            rootNode = value.rootNode;
            size = value.size;
            firstUnusedNode = value.firstUnusedNode;
            unusedNodesCount = value.unusedNodesCount;

            value.rootNode = nullptr;
            value.size = 0;
            value.firstUnusedNode = nullptr;
            value.unusedNodesCount = 0;

            return *this;
        }

        index_type getSize() const { return size; }
        bool isEmpty() const { return size == 0; }

        iterator begin() noexcept { return iterator(rootNode); }
        iterator end() noexcept { return iterator(); }

        const_iterator begin() const noexcept { return const_iterator(rootNode); }
        const_iterator end() const noexcept { return const_iterator(); }

        template<typename KeyType>
        type* find(const KeyType& key)
        {
            tree_node* node = _findNode(key);
            return node != nullptr ? &node->object : nullptr;
        }
        template<typename KeyType>
        const type* find(const KeyType& key) const
        {
            const tree_node* node = _findNode(key);
            return node != nullptr ? &node->object : nullptr;
        }
        template<typename KeyType>
        bool contains(const KeyType& key) const { return _findNode(key) != nullptr; }

        void reserve(const index_type capacity) { _reserveFreeNodes(capacity - size); }

        template<typename KeyType, typename... Args>
        type& put(const KeyType& key, Args&&... args) { return _putValue(false, key, std::forward<Args>(args)...); }
        template<typename KeyType, typename... Args>
        type& set(const KeyType& key, Args&&... args) { return _putValue(true, key, std::forward<Args>(args)...); }

        type& add(const type& value, const bool shouldOverride = true) { return shouldOverride ? set(value, value) : put(value, value); }
        type& add(type&& value, const bool shouldOverride = true) { return shouldOverride ? set(value, std::move(value)) : put(value, std::move(value)); }

        void append(std::initializer_list<type> list, const bool shouldOverride = true)
        {
            reserve(getSize() + static_cast<index_type>(list.size()));
            for (const auto& value : list)
            {
                add(value, shouldOverride);
            }
        }
        void append(const jtree_red_black& tree, const bool shouldOverride = true)
        {
            if ((this != &tree) && !tree.isEmpty())
            {
                reserve(getSize() + tree.getSize());
                for (const auto& value : tree)
                {
                    add(value, shouldOverride);
                }
            }
        }

        template<typename KeyType>
        bool remove(const KeyType& key)
        {
            tree_node* node = _findNode(key);
            if (node != nullptr)
            {
                size--;
                _removeValue(node);
                return true;
            }
            return false;
        }
        void clear();

        jtree_red_black& operator+=(const type& value)
        {
            add(value);
            return *this;
        }
        jtree_red_black& operator+=(type&& value)
        {
            add(std::move(value));
            return *this;
        }
        jtree_red_black& operator+=(std::initializer_list<type> list)
        {
            append(list);
            return *this;
        }
        jtree_red_black& operator+=(const jtree_red_black& tree)
        {
            append(tree);
            return *this;
        }

    private:

        using allocator_type = jlist_allocator<tree_node>;

        allocator_type allocator = allocator_type();

        tree_node* rootNode = nullptr;
        index_type size = 0;

        tree_node* firstUnusedNode = nullptr;
        index_type unusedNodesCount = 0;


        template<typename... Args>
        static void _constructNodeObject(tree_node* node, Args&&... args) { jutils::memory::construct(&node->object, std::forward<Args>(args)...); }
        static void _destroyNodeObject(tree_node* node) { jutils::memory::destruct(&node->object); }
        static void _copyNodeObject(tree_node* srcNode, tree_node* dstNode)
        {
            if (std::is_trivially_copyable_v<type>)
            {
                ::memcpy(&dstNode->object, &srcNode->object, sizeof(type));
            }
            else
            {
                dstNode->object = std::move(srcNode->object);
            }
        }

        template<typename Type1, typename Type2>
        static constexpr bool _compareObjects(const Type1& key1, const Type2& key2)
        {
            constexpr compare_predicator predicate;
            return predicate(key1, key2);
        }
        template<typename KeyType>
        tree_node* _findNode(const KeyType& key) const;

        void _allocateNewSegment();
        tree_node* _getNode();
        void _returnNode(tree_node* node);
        void _reserveFreeNodes(index_type count);

        static bool _rotateLeft(tree_node* node);
        static bool _rotateRight(tree_node* node);
        void _rotateNode(tree_node* node, const bool left);
        
        // Return false if key was exist before
        template<typename KeyType>
        bool _putKey(const KeyType& key, tree_node*& outNode);
        void _restoreBallanceAfterPut(tree_node* node);
        template<typename KeyType, typename... Args>
        type& _putValue(bool overrideValue, const KeyType& key, Args&&... args);

        void _removeValue(tree_node* node);
        void _restoreBallanceAfterRemove(tree_node* parent, bool left);

        void _clearAllData();
    };

    template<typename T, typename ComparePred>
    void jtree_red_black<T, ComparePred>::const_iterator::_moveToNextNode()
    {
        if (!_isValid())
        {
            return;
        }

        if (node->childLeft != nullptr)
        {
            node = node->childLeft;
        }
        else if (node->childRight != nullptr)
        {
            node = node->childRight;
        }
        else
        {
            tree_node* parent = node->parent;
            while (parent != nullptr)
            {
                if ((parent->childLeft == node) && (parent->childRight != nullptr))
                {
                    node = parent->childRight;
                    break;
                }

                node = parent;
                parent = node->parent;
            }
            if (parent == nullptr)
            {
                node = nullptr;
            }
        }
    }
    template<typename T, typename ComparePred>
    void jtree_red_black<T, ComparePred>::const_iterator::_moveToPrevNode()
    {
        if (!_isValid())
        {
            return;
        }

        tree_node* parent = node->parent;
        if (parent == nullptr)
        {
            node = nullptr;
        }
        else if ((parent->childLeft == nullptr) || (parent->childLeft == node))
        {
            node = parent;
        }
        else
        {
            while ((node->childLeft != nullptr) || (node->childRight != nullptr))
            {
                while (node->childRight != nullptr)
                {
                    node = node->childRight;
                }
                if (node->childLeft != nullptr)
                {
                    node = node->childLeft;
                }
            }
        }
    }
    template<typename T, typename ComparePred>
    void jtree_red_black<T, ComparePred>::const_iterator::_offsetNode(index_type offset)
    {
        while (_isValid() && (offset != 0))
        {
            if (offset > 0)
            {
                _moveToNextNode();
                offset--;
            }
            else
            {
                _moveToPrevNode();
                offset++;
            }
        }
    }

    template<typename T, typename ComparePred>
    template<typename KeyType>
    typename jtree_red_black<T, ComparePred>::tree_node* jtree_red_black<T, ComparePred>::_findNode(const KeyType& key) const
    {
        tree_node* node = rootNode;
        while (node != nullptr)
        {
            if (_compareObjects(key, node->object))
            {
                node = node->childLeft;
            }
            else if (_compareObjects(node->object, key))
            {
                node = node->childRight;
            }
            else
            {
                return node;
            }
        }
        return nullptr;
    }

    template<typename T, typename ComparePred>
    void jtree_red_black<T, ComparePred>::_allocateNewSegment()
    {
        tree_node* segmentData = nullptr;
        index_type segmentSize = 0;
        allocator.allocateSegment(segmentData, segmentSize);

        for (index_type index = 0; index < segmentSize - 1; index++)
        {
            segmentData[index].markUnused();
            segmentData[index].childRight = segmentData + index + 1;
        }
        segmentData[segmentSize - 1].markUnused();
        segmentData[segmentSize - 1].childRight = firstUnusedNode;
        firstUnusedNode = segmentData;
        unusedNodesCount += segmentSize;
    }
    template<typename T, typename ComparePred>
    typename jtree_red_black<T, ComparePred>::tree_node* jtree_red_black<T, ComparePred>::_getNode()
    {
        if (unusedNodesCount == 0)
        {
            _allocateNewSegment();
        }

        tree_node* node = firstUnusedNode;
        firstUnusedNode = firstUnusedNode->childRight;
        unusedNodesCount--;

        node->isRed = true;
        node->parent = nullptr;
        node->childLeft = nullptr;
        node->childRight = nullptr;
        return node;
    }
    template<typename T, typename ComparePred>
    void jtree_red_black<T, ComparePred>::_returnNode(tree_node* node)
    {
        if (node != nullptr)
        {
            node->markUnused();
            node->childRight = firstUnusedNode;
            firstUnusedNode = node;
            unusedNodesCount++;
        }
    }
    template<typename T, typename ComparePred>
    void jtree_red_black<T, ComparePred>::_reserveFreeNodes(const index_type count)
    {
        while (unusedNodesCount < count)
        {
            _allocateNewSegment();
        }
    }

    template<typename T, typename ComparePred>
    bool jtree_red_black<T, ComparePred>::_rotateLeft(tree_node* node)
    {
        if (node == nullptr)
        {
            return false;
        }

        tree_node* child = node->childRight;
        if (child == nullptr)
        {
            return false;
        }

        node->childRight = child->childLeft;
        if (node->childRight != nullptr)
        {
            node->childRight->parent = node;
        }
        child->childLeft = node;

        if (node->parent != nullptr)
        {
            if (node->parent->childLeft == node)
            {
                node->parent->childLeft = child;
            }
            else
            {
                node->parent->childRight = child;
            }
        }
        child->parent = node->parent;
        node->parent = child;

        return true;
    }
    template<typename T, typename ComparePred>
    bool jtree_red_black<T, ComparePred>::_rotateRight(tree_node* node)
    {
        if (node == nullptr)
        {
            return false;
        }

        tree_node* child = node->childLeft;
        if (child == nullptr)
        {
            return false;
        }

        node->childLeft = child->childRight;
        if (node->childLeft != nullptr)
        {
            node->childLeft->parent = node;
        }
        child->childRight = node;

        if (node->parent != nullptr)
        {
            if (node->parent->childLeft == node)
            {
                node->parent->childLeft = child;
            }
            else
            {
                node->parent->childRight = child;
            }
        }
        child->parent = node->parent;
        node->parent = child;

        return true;
    }
    template<typename T, typename ComparePred>
    void jtree_red_black<T, ComparePred>::_rotateNode(tree_node* node, const bool left)
    {
        if ((left && _rotateLeft(node)) || (!left && _rotateRight(node)))
        {
            if (node == rootNode)
            {
                rootNode = node->parent;
            }
        }
    }

    template<typename T, typename ComparePred>
    template<typename KeyType>
    bool jtree_red_black<T, ComparePred>::_putKey(const KeyType& key, tree_node*& outNode)
    {
        if (rootNode == nullptr)
        {
            rootNode = _getNode();
            _restoreBallanceAfterPut(rootNode);

            outNode = rootNode;
            return true;
        }

        tree_node* node = rootNode;
        while (true)
        {
            if (_compareObjects(key, node->object))
            {
                if (node->childLeft != nullptr)
                {
                    node = node->childLeft;
                }
                else
                {
                    tree_node* newNode = _getNode();
                    newNode->parent = node;
                    node->childLeft = newNode;
                    _restoreBallanceAfterPut(newNode);

                    outNode = newNode;
                    return true;
                }
            }
            else if (_compareObjects(node->object, key))
            {
                if (node->childRight != nullptr)
                {
                    node = node->childRight;
                }
                else
                {
                    tree_node* newNode = _getNode();
                    newNode->parent = node;
                    node->childRight = newNode;
                    _restoreBallanceAfterPut(newNode);

                    outNode = newNode;
                    return true;
                }
            }
            else
            {
                outNode = node;
                return false;
            }
        }
    }
    template<typename T, typename ComparePred>
    void jtree_red_black<T, ComparePred>::_restoreBallanceAfterPut(tree_node* node)
    {
        while (node != nullptr)
        {
            tree_node* parent = node->parent;
            if (parent == nullptr)
            {
                // Root should be black
                node->isRed = false;
                return;
            }

            if (!parent->isRed)
            {
                // Parent black - OK
                return;
            }

            // Parent is red, so it can't be root, grand exists and black
            tree_node* grand = parent->parent;
            tree_node* uncle = grand->childLeft == parent ? grand->childRight : grand->childLeft;
            if ((uncle != nullptr) && uncle->isRed)
            {
                parent->isRed = false;
                uncle->isRed = false;
                grand->isRed = true;

                node = grand;
                continue;
            }

            // Parent is red, uncle is black (or not exists), grand is black
            if (((grand->childRight == uncle) && (parent->childRight == node)) || ((grand->childLeft == uncle) && (
                parent->childLeft == node)))
            {
                _rotateNode(parent, parent->childRight == node);
                node = parent;
                parent = node->parent;
            }

            // Node is left child and uncle is right, or vise versa
            parent->isRed = false;
            grand->isRed = true;
            _rotateNode(grand, parent->childRight == node);
            return;
        }
    }
    template<typename T, typename ComparePred>
    template<typename KeyType, typename... Args>
    typename jtree_red_black<T, ComparePred>::type& jtree_red_black<T, ComparePred>::_putValue(const bool overrideValue, const KeyType& key, Args&&... args)
    {
        tree_node* node = nullptr;
        const bool keyWasCreated = _putKey(key, node);
        if (keyWasCreated)
        {
            size++;
            _constructNodeObject(node, std::forward<Args>(args)...);
        }
        else if (overrideValue)
        {
            node->object = type(std::forward<Args>(args)...);
        }
        return node->object;
    }

    template<typename T, typename ComparePred>
    void jtree_red_black<T, ComparePred>::_removeValue(tree_node* node)
    {
        if (node == nullptr)
        {
            return;
        }
        if ((node == rootNode) && (node->childLeft == nullptr) && (node->childRight == nullptr))
        {
            _destroyNodeObject(node);
            _returnNode(node);
            rootNode = nullptr;
            return;
        }

        if ((node->childLeft != nullptr) && (node->childRight != nullptr))
        {
            tree_node* maxNode = node->childLeft;
            while (maxNode->childRight != nullptr)
            {
                maxNode = maxNode->childRight;
            }

            if (node->parent != nullptr)
            {
                if (node->parent->childLeft == node)
                {
                    node->parent->childLeft = maxNode;
                }
                else
                {
                    node->parent->childRight = maxNode;
                }
            }
            if ((node->childLeft != nullptr) && (node->childLeft != maxNode))
            {
                node->childLeft->parent = maxNode;
            }
            if (node->childRight != nullptr)
            {
                node->childRight->parent = maxNode;
            }
            if (maxNode->parent != node)
            {
                maxNode->parent->childRight = node;
            }
            if (maxNode->childLeft != nullptr)
            {
                maxNode->childLeft->parent = node;
            }
            std::swap(maxNode->isRed, node->isRed);
            std::swap(maxNode->childRight, node->childRight);
            if (node->childLeft != maxNode)
            {
                std::swap(maxNode->parent, node->parent);
                std::swap(maxNode->childLeft, node->childLeft);
            }
            else
            {
                maxNode->parent = node->parent;
                node->parent = maxNode;

                node->childLeft = maxNode->childLeft;
                maxNode->childLeft = node;
            }
            if (node == rootNode)
            {
                rootNode = maxNode;
            }
        }

        tree_node* parent = node->parent;
        const bool left = (parent != nullptr) && (parent->childLeft == node);
        if (node->isRed)
        {
            if (left)
            {
                parent->childLeft = nullptr;
            }
            else
            {
                parent->childRight = nullptr;
            }

            _destroyNodeObject(node);
            _returnNode(node);
            return;
        }

        // Node is black
        if ((node->childLeft != nullptr) && node->childLeft->isRed)
        {
            node->childLeft->parent = parent;
            node->childLeft->isRed = false;
            if (parent != nullptr)
            {
                if (left)
                {
                    parent->childLeft = node->childLeft;
                }
                else
                {
                    parent->childRight = node->childLeft;
                }
            }
            else
            {
                rootNode = node->childLeft;
            }
            _destroyNodeObject(node);
            _returnNode(node);
            return;
        }
        if ((node->childRight != nullptr) && node->childRight->isRed)
        {
            node->childRight->parent = parent;
            node->childRight->isRed = false;
            if (parent != nullptr)
            {
                if (left)
                {
                    parent->childLeft = node->childRight;
                }
                else
                {
                    parent->childRight = node->childRight;
                }
            }
            else
            {
                rootNode = node->childRight;
            }
            _destroyNodeObject(node);
            _returnNode(node);
            return;
        }

        // No children, not root
        if (left)
        {
            parent->childLeft = nullptr;
        }
        else
        {
            parent->childRight = nullptr;
        }
        _destroyNodeObject(node);
        _returnNode(node);
        _restoreBallanceAfterRemove(parent, left);
    }
    template<typename T, typename ComparePred>
    void jtree_red_black<T, ComparePred>::_restoreBallanceAfterRemove(tree_node* parent, bool left)
    {
        while (true)
        {
            if (parent == nullptr)
            {
                // Node is root
                return;
            }

            tree_node* node = left ? parent->childLeft : parent->childRight;
            tree_node* sibling = left ? parent->childRight : parent->childLeft;
            if ((sibling != nullptr) && sibling->isRed)
            {
                parent->isRed = true;
                sibling->isRed = false;
                _rotateNode(parent, left);

                sibling = left ? parent->childRight : parent->childLeft;
            }

            // Sibling is black
            tree_node* siblingNear = sibling != nullptr ? (left ? sibling->childLeft : sibling->childRight) : nullptr;
            tree_node* siblingFar = sibling != nullptr ? (left ? sibling->childRight : sibling->childLeft) : nullptr;
            if (!parent->isRed && ((siblingNear == nullptr) || !siblingNear->isRed) && ((siblingFar == nullptr) || !
                siblingFar->isRed))
            {
                sibling->isRed = true;

                node = parent;
                parent = node->parent;
                left = (parent == nullptr) || (parent->childLeft == node);
                continue;
            }

            if (parent->isRed && ((siblingNear == nullptr) || !siblingNear->isRed) && ((siblingFar == nullptr) || !
                siblingFar->isRed))
            {
                parent->isRed = false;
                sibling->isRed = true;
                return;
            }

            if ((siblingNear != nullptr) && siblingNear->isRed && ((siblingFar == nullptr) || !siblingFar->isRed))
            {
                sibling->isRed = true;
                siblingNear->isRed = false;
                _rotateNode(sibling, !left);

                siblingFar = sibling;
                sibling = siblingNear;
                siblingNear = left ? sibling->childLeft : sibling->childRight;
            }

            sibling->isRed = parent->isRed;
            parent->isRed = false;
            if (siblingFar != nullptr)
            {
                siblingFar->isRed = false;
            }
            _rotateNode(parent, left);
            return;
        }
    }

    template <typename T, typename ComparePred>
    void jtree_red_black<T, ComparePred>::clear()
    {
        if (!isEmpty())
        {
            tree_node* node = rootNode;
            while (node != nullptr)
            {
                if ((node->childLeft == nullptr) && (node->childRight == nullptr))
                {
                    tree_node* parent = node->parent;
                    _destroyNodeObject(node);
                    _returnNode(node);
                    node = parent;
                }
                else if (node->childLeft != nullptr)
                {
                    node = node->childLeft;
                    node->parent->childLeft = nullptr;
                }
                else
                {
                    node = node->childRight;
                    node->parent->childRight = nullptr;
                }
            }
            rootNode = nullptr;
            size = 0;
        }
    }

    template <typename T, typename ComparePred>
    void jtree_red_black<T, ComparePred>::_clearAllData()
    {
        tree_node* node = rootNode;
        while (node != nullptr)
        {
            if ((node->childLeft == nullptr) && (node->childRight == nullptr))
            {
                _destroyNodeObject(node);
                node = node->parent;
            }
            else if (node->childLeft != nullptr)
            {
                node = node->childLeft;
                node->parent->childLeft = nullptr;
            }
            else
            {
                node = node->childRight;
                node->parent->childRight = nullptr;
            }
        }

        allocator.clear();

        rootNode = nullptr;
        size = 0;
        firstUnusedNode = nullptr;
        unusedNodesCount = 0;
    }

    template<typename T, typename ComparePred>
    jtree_red_black<T, ComparePred> operator+(const jtree_red_black<T, ComparePred>& container, const T& value) { return jtree_red_black<T, ComparePred>(container) += value; }
    template<typename T, typename ComparePred>
    jtree_red_black<T, ComparePred> operator+(const T& value, const jtree_red_black<T, ComparePred>& container) { return container + value; }
    template<typename T, typename ComparePred>
    jtree_red_black<T, ComparePred> operator+(const jtree_red_black<T, ComparePred>& container1, const jtree_red_black<T, ComparePred>& container2) { return jtree_red_black<T, ComparePred>(container1) += container2; }
}
