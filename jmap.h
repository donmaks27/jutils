// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"
#include "jtree_red_black.h"
#include "jmap_pair.h"

namespace jutils
{
    template<typename KeyType, typename ValueType, typename KeyComparePred = std::less<>>
    class jmap
    {
    public:

        using key_type = KeyType;
        using value_type = ValueType;
        using pair_type = jmap_pair<key_type, value_type>;

        using key_compare_predicator_type = KeyComparePred;
        struct compare_predicator
        {
            constexpr bool operator()(const key_type& value1, const key_type& value2) const
            {
                static constexpr key_compare_predicator_type predicator;
                return predicator(value1, value2);
            }
            constexpr bool operator()(const pair_type& value1, const pair_type& value2) const { return this->operator()(value1.key, value2.key); }
            constexpr bool operator()(const pair_type& value1, const key_type& value2) const { return this->operator()(value1.key, value2); }
            constexpr bool operator()(const key_type& value1, const pair_type& value2) const { return this->operator()(value1, value2.key); }
        };

        using container_type = jtree_red_black<pair_type, compare_predicator>;
        using iterator = typename container_type::iterator;
        using const_iterator = typename container_type::const_iterator;
        using index_type = int32;

        jmap() = default;
        jmap(std::initializer_list<pair_type> list)
            : container(list)
        {}
        jmap(const jmap&) = default;
        jmap(jmap&& value) noexcept
            : container(std::move(value.container))
        {}
        ~jmap() = default;

        jmap& operator=(std::initializer_list<pair_type> list)
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
        value_type& operator[](const key_type& key) { return getOrAdd(key); }
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

        jarray<key_type> getKeys() const
        {
            jarray<key_type> result;
            if (!this->isEmpty())
            {
                for (const auto& element : container)
                {
                    result.add(element.key);
                }
            }
            return result;
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
        value_type& add(const key_type& key) { return put(key); }
        value_type& add(key_type&& key) { return put(std::move(key)); }

        value_type& getOrAdd(const key_type& key) { return container.put(key, key).value; }
        
        void append(std::initializer_list<pair_type> list) { container.append(list); }
        void append(const jmap& value) { container.append(value.container); }

        bool remove(const key_type& key) { return container.remove(key); }
        void clear() { container.clear(); }

        jmap& operator+=(const pair_type& value)
        {
            add(value);
            return *this;
        }
        jmap& operator+=(pair_type&& value)
        {
            add(std::move(value));
            return *this;
        }
        jmap& operator+=(std::initializer_list<pair_type> list)
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
    jmap<KeyType, ValueType> operator+(const jmap<KeyType, ValueType>& container, const typename jmap<KeyType, ValueType>::pair_type& value)
    {
        return jmap<KeyType, ValueType>(container) += value;
    }
    template<typename KeyType, typename ValueType>
    jmap<KeyType, ValueType> operator+(const typename jmap<KeyType, ValueType>::pair_type& value, const jmap<KeyType, ValueType>& container)
    {
        return jmap<KeyType, ValueType>(container) += value;
    }
    template<typename KeyType, typename ValueType>
    jmap<KeyType, ValueType> operator+(const jmap<KeyType, ValueType>& container1, const jmap<KeyType, ValueType>& container2)
    {
        return jmap<KeyType, ValueType>(container1) += container2;
    }
}
