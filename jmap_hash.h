// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jhash_table_open.h"
#include "jmap_pair.h"

namespace jutils
{
    template<typename KeyType, typename ValueType>
    class jmap_hash
    {
    public:

        using key_type = KeyType;
        using value_type = ValueType;
        using pair_type = jmap_pair<key_type, value_type>;
        using container_type = jhash_table_open<pair_type>;
        using iterator = typename container_type::iterator;
        using const_iterator = typename container_type::const_iterator;
        using index_type = int32;

        jmap_hash() = default;
        jmap_hash(std::initializer_list<pair_type> list)
            : container(list)
        {}
        jmap_hash(const jmap_hash&) = default;
        jmap_hash(jmap_hash&& value) noexcept
            : container(std::move(value.container))
        {}
        ~jmap_hash() = default;

        jmap_hash& operator=(std::initializer_list<pair_type> list)
        {
            container = list;
            return *this;
        }
        jmap_hash& operator=(const jmap_hash&) = default;
        jmap_hash& operator=(jmap_hash&& value) noexcept
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
            pair_type* value = container.find(key);
            return value != nullptr ? &value->value : nullptr;
        }
        const value_type* find(const key_type& key) const
        {
            const pair_type* value = container.find(key);
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
        value_type& add(const pair_type& value) { return put(value.key, value); }
        value_type& add(pair_type&& value) { return put(value.key, std::move(value)); }
        
        void append(std::initializer_list<pair_type> list) { container.append(list); }
        void append(const jmap_hash& value) { container.append(value.container); }

        void remove(const key_type& key) { container.remove(key); }
        void clear() { container.clear(); }

        jmap_hash& operator+=(const pair_type& value)
        {
            add(value);
            return *this;
        }
        jmap_hash& operator+=(pair_type&& value)
        {
            add(std::move(value));
            return *this;
        }
        jmap_hash& operator+=(std::initializer_list<pair_type> list)
        {
            append(list);
            return *this;
        }
        jmap_hash& operator+=(const jmap_hash& value)
        {
            append(value);
            return *this;
        }

    private:

        container_type container;
    };

    template<typename KeyType, typename ValueType>
    jmap_hash<KeyType, ValueType> operator+(const jmap_hash<KeyType, ValueType>& container, const typename jmap_hash<KeyType, ValueType>::pair_type& value)
    {
        return jmap_hash<KeyType, ValueType>(container) += value;
    }
    template<typename KeyType, typename ValueType>
    jmap_hash<KeyType, ValueType> operator+(const typename jmap_hash<KeyType, ValueType>::pair_type& value, const jmap_hash<KeyType, ValueType>& container)
    {
        return jmap_hash<KeyType, ValueType>(container) += value;
    }
    template<typename KeyType, typename ValueType>
    jmap_hash<KeyType, ValueType> operator+(const jmap_hash<KeyType, ValueType>& container1, const jmap_hash<KeyType, ValueType>& container2)
    {
        return jmap_hash<KeyType, ValueType>(container1) += container2;
    }
}
