// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jtree_red_black.h"

namespace jutils
{
    template<typename KeyType, typename ValueType>
    class jmap
    {
    public:

        using key_type = KeyType;
        using value_type = ValueType;

        struct pair
        {
            constexpr pair() = default;
            template<typename... Args>
            constexpr pair(const key_type& key, Args&&... valueArgs)
                : key(key), value(std::forward<Args>(valueArgs)...)
            {}
            template<typename... Args>
            constexpr pair(key_type&& key, Args&&... valueArgs)
                : key(std::move(key)), value(std::forward<Args>(valueArgs)...)
            {}
            constexpr pair(const pair& value) = default;
            constexpr pair(pair&& value) noexcept = default;

            constexpr pair& operator=(const pair& otherPair) = default;
            constexpr pair& operator=(pair&& otherPair) noexcept = default;

            key_type key;
            value_type value;
        };
        struct compare_predicator
        {
            constexpr bool operator()(const key_type& value1, const key_type& value2) const
            {
                static constexpr std::less<> predicator;
                return predicator(value1, value2);
            }
            constexpr bool operator()(const pair& value1, const pair& value2) const { return this->operator()(value1.key, value2.key); }
            constexpr bool operator()(const pair& value1, const key_type& value2) const { return this->operator()(value1.key, value2); }
            constexpr bool operator()(const key_type& value1, const pair& value2) const { return this->operator()(value1, value2.key); }
        };

        using container_type = jtree_red_black<pair, compare_predicator>;
        using iterator = typename container_type::iterator;
        using const_iterator = typename container_type::const_iterator;
        using index_type = int32;

        jmap() = default;
        jmap(std::initializer_list<pair> list)
            : container(list)
        {}
        jmap(const jmap&) = default;
        jmap(jmap&& value) noexcept
            : container(std::move(value.container))
        {}
        ~jmap() = default;

        jmap& operator=(std::initializer_list<pair> list)
        {
            container = list;
            return *this;
        }
        jmap& operator=(const jmap&) = default;
        jmap& operator=(jmap&& value) noexcept
        {
            container = std::move(value.container);
            return *this;
        }

        index_type getSize() const { return container.getSize(); }
        bool isEmpty() const { return getSize() == 0; }

        iterator begin() noexcept { return container.begin(); }
        iterator end() noexcept { return container.end(); }

        const_iterator begin() const noexcept { return container.begin(); }
        const_iterator end() const noexcept { return container.end(); }

        value_type& get(const key_type& key) { return container.find(key)->value; }
        const value_type& get(const key_type& key) const { return container.find(key)->value; }
        value_type& operator[](const key_type& key) { return put(key); }
        const value_type& operator[](const key_type& key) const { return get(key); }

        value_type* find(const key_type& key)
        {
            pair* value = container.find(key);
            return value != nullptr ? &value->value : nullptr;
        }
        const value_type* find(const key_type& key) const
        {
            const pair* value = container.find(key);
            return value != nullptr ? &value->value : nullptr;
        }

        bool contains(const key_type& key) const { return container.find(key) != nullptr; }

        void reserve(const index_type capacity) { container.reserve(capacity); }

        template<typename... Args>
        value_type& put(const key_type& key, Args&&... args) { return container.set(key, key, std::forward<Args>(args)...).value; }
        template<typename... Args>
        value_type& put(key_type&& key, Args&&... args) { return container.set(key, std::move(key), std::forward<Args>(args)...).value; }

        value_type& add(const key_type& key, const value_type& value) { return put(key, value); }
        value_type& add(const key_type& key, value_type&& value) { return put(key, std::move(value)); }
        value_type& add(key_type&& key, const value_type& value) { return put(std::move(key), value); }
        value_type& add(key_type&& key, value_type&& value) { return put(std::move(key), std::move(value)); }
        value_type& add(const pair& value) { return put(value.key, value); }
        value_type& add(pair&& value) { return put(value.key, std::move(value)); }
        
        void append(std::initializer_list<pair> list) { container.append(list); }
        void append(const jmap& value) { container.append(value.container); }

        void remove(const key_type& key) { container.remove(key); }
        void clear() { container.clear(); }

        jmap& operator+=(const pair& value)
        {
            add(value);
            return *this;
        }
        jmap& operator+=(pair&& value)
        {
            add(std::move(value));
            return *this;
        }
        jmap& operator+=(std::initializer_list<pair> list)
        {
            append(list);
            return *this;
        }
        jmap& operator+=(const jmap& value)
        {
            append(value);
            return *this;
        }

    private:

        container_type container;
    };

    template<typename KeyType, typename ValueType>
    jmap<KeyType, ValueType> operator+(const jmap<KeyType, ValueType>& container, const typename jmap<KeyType, ValueType>::pair& value)
    {
        return jmap<KeyType, ValueType>(container) += value;
    }
    template<typename KeyType, typename ValueType>
    jmap<KeyType, ValueType> operator+(const typename jmap<KeyType, ValueType>::pair& value, const jmap<KeyType, ValueType>& container)
    {
        return jmap<KeyType, ValueType>(container) += value;
    }
    template<typename KeyType, typename ValueType>
    jmap<KeyType, ValueType> operator+(const jmap<KeyType, ValueType>& container1, const jmap<KeyType, ValueType>& container2)
    {
        return jmap<KeyType, ValueType>(container1) += container2;
    }
}
