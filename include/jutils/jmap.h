// Copyright © 2022-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"

#include <map>

namespace jutils
{
    template<typename KeyType, typename ValueType, typename KeyComparePred = std::less<KeyType>>
        requires std::predicate<KeyComparePred, KeyType, KeyType>
    class jmap : private std::map<KeyType, ValueType, KeyComparePred>
    {
    public:

        using base_type = std::map<KeyType, ValueType, KeyComparePred>;
        using key_type = typename base_type::key_type;
        using value_type = typename base_type::mapped_type;
        using key_compare_predicator_type = typename base_type::key_compare;
        using size_type = typename base_type::size_type;
        using pair_type = typename base_type::value_type;
        
        using const_iterator = typename base_type::const_iterator;
        using iterator = typename base_type::iterator;

        jmap() noexcept = default;
        jmap(const std::initializer_list<pair_type> values)
            : base_type(values)
        {}
        jmap(const base_type& value)
            : base_type(value)
        {}
        jmap(base_type&& value) noexcept
            : base_type(std::move(value))
        {}
        jmap(const jmap&) = default;
        jmap(jmap&&) noexcept =default;
        ~jmap() noexcept = default;

        jmap& operator=(const std::initializer_list<pair_type> values)
        {
            base_type::operator=(values);
            return *this;
        }
        jmap& operator=(const base_type& value)
        {
            base_type::operator=(value);
            return *this;
        }
        jmap& operator=(base_type&& value)
        {
            base_type::operator=(std::move(value));
            return *this;
        }
        jmap& operator=(const jmap&) = default;
        jmap& operator=(jmap&&) noexcept = default;

        const base_type& toBase() const noexcept { return *this; }

        size_type getSize() const noexcept { return base_type::size(); }
        bool isEmpty() const noexcept { return base_type::empty(); }

        iterator begin() noexcept { return base_type::begin(); }
        iterator end() noexcept { return base_type::end(); }

        const_iterator begin() const noexcept { return base_type::begin(); }
        const_iterator end() const noexcept { return base_type::end(); }

        jmap copy() const { return *this; }
        jarray<key_type> getKeys() const
        {
            jarray<key_type> keys;
            keys.reserve(getSize());
            for (const auto& pair : *this)
            {
                keys.add(pair.first);
            }
            return keys;
        }

        value_type& get(const key_type& key) { return base_type::at(key); }
        const value_type& get(const key_type& key) const { return base_type::at(key); }

        value_type& operator[](const key_type& key) { return base_type::operator[](key); }
        value_type& operator[](key_type&& key) { return base_type::operator[](std::move(key)); }
        const value_type& operator[](const key_type& key) const { return base_type::operator[](key); }

        value_type& getOrAdd(const key_type& key) { return base_type::operator[](key); }
        value_type& getOrAdd(key_type&& key) { return base_type::operator[](std::move(key)); }

        value_type* find(const key_type& key) noexcept
        {
            const iterator iter = base_type::find(key);
            return iter != end() ? &iter->second : nullptr;
        }
        template<typename Pred> requires std::predicate<Pred, key_type, value_type>
        value_type* find(Pred pred) noexcept
        {
            for (auto& pair : *this)
            {
                if (pred(pair.first, pair.second))
                {
                    return &pair.second;
                }
            }
            return nullptr;
        }
        const value_type* find(const key_type& key) const noexcept
        {
            const const_iterator iter = base_type::find(key);
            return iter != end() ? &iter->second : nullptr;
        }
        template<typename Pred> requires std::predicate<Pred, key_type, value_type>
        const value_type* find(Pred pred) const noexcept
        {
            for (const auto& pair : *this)
            {
                if (pred(pair.first, pair.second))
                {
                    return &pair.second;
                }
            }
            return nullptr;
        }

        bool contains(const key_type& key) const noexcept { return base_type::contains(key); }
        template<typename Pred> requires std::predicate<Pred, key_type, value_type>
        bool contains(Pred pred) const noexcept { return find(pred) != nullptr; }
        
        value_type& add(const key_type& key, const value_type& value)
        {
            return base_type::insert_or_assign(key, value).first->second;
        }
        value_type& add(const key_type& key, value_type&& value)
        {
            return base_type::insert_or_assign(key, std::move(value)).first->second;
        }
        value_type& add(key_type&& key, const value_type& value)
        {
            return base_type::insert_or_assign(std::move(key), value).first->second;
        }
        value_type& add(key_type&& key, value_type&& value)
        {
            return base_type::insert_or_assign(std::move(key), std::move(value)).first->second;
        }
        value_type& add(const key_type& key) { return add(key, value_type()); }
        value_type& add(key_type&& key) { return add(std::move(key), value_type()); }

        jmap& append(const std::initializer_list<pair_type> values)
        {
            for (const auto& pair : values)
            {
                add(pair.first, pair.second);
            }
            return *this;
        }
        jmap& append(const base_type& value)
        {
            base_type::merge(value);
            return *this;
        }
        jmap& append(const jmap& value) { return append(value.toBase()); }

        bool remove(const key_type& key) noexcept { return base_type::erase(key) > 0; }
        template<typename Pred> requires std::predicate<Pred, key_type, value_type>
        size_type remove(Pred pred) noexcept
        {
            size_type count = 0;
            for (iterator iter = begin(); iter != end(); ++iter)
            {
                if (pred(iter->first, iter->second))
                {
                    iter = base_type::erase(iter);
                    ++count;
                }
            }
            return count;
        }
        void clear() noexcept { base_type::clear(); }

        jmap& operator+=(const pair_type& value)
        {
            add(value.first, value.second);
            return *this;
        }
        jmap& operator+=(pair_type&& value)
        {
            add(std::move(value.first), std::move(value.second));
            return *this;
        }
        jmap& operator+=(const std::initializer_list<pair_type> values) { return append(values); }
        jmap& operator+=(const base_type& value) { return append(value); }
        jmap& operator+=(const jmap& value) { return append(value); }
    };
    
    template<typename Key, typename Value, typename Pred>
    jmap<Key, Value, Pred> operator+(const jmap<Key, Value, Pred>& value1, const typename jmap<Key, Value, Pred>::pair_type& value2)
    {
        return value1.copy() += value2;
    }
    template<typename Key, typename Value, typename Pred>
    jmap<Key, Value, Pred> operator+(const jmap<Key, Value, Pred>& value1, typename jmap<Key, Value, Pred>::pair_type&& value2)
    {
        return value1.copy() += std::move(value2);
    }
    template<typename Key, typename Value, typename Pred>
    jmap<Key, Value, Pred> operator+(jmap<Key, Value, Pred>&& value1, const typename jmap<Key, Value, Pred>::pair_type& value2)
    {
        return value1 += value2;
    }
    template<typename Key, typename Value, typename Pred>
    jmap<Key, Value, Pred> operator+(jmap<Key, Value, Pred>&& value1, typename jmap<Key, Value, Pred>::pair_type&& value2)
    {
        return value1 += std::move(value2);
    }

    template<typename Key, typename Value, typename Pred>
    jmap<Key, Value, Pred> operator+(const typename jmap<Key, Value, Pred>::pair_type& value1, const jmap<Key, Value, Pred>& value2)
    {
        return value2.copy() += value1;
    }
    template<typename Key, typename Value, typename Pred>
    jmap<Key, Value, Pred> operator+(const typename jmap<Key, Value, Pred>::pair_type& value1, jmap<Key, Value, Pred>&& value2)
    {
        return value2 += value1;
    }
    template<typename Key, typename Value, typename Pred>
    jmap<Key, Value, Pred> operator+(typename jmap<Key, Value, Pred>::pair_type&& value1, const jmap<Key, Value, Pred>& value2)
    {
        return value2.copy() += std::move(value1);
    }
    template<typename Key, typename Value, typename Pred>
    jmap<Key, Value, Pred> operator+(typename jmap<Key, Value, Pred>::pair_type&& value1, jmap<Key, Value, Pred>&& value2)
    {
        return value2 += std::move(value1);
    }

    template<typename Key, typename Value, typename Pred>
    jmap<Key, Value, Pred> operator+(const jmap<Key, Value, Pred>& value1, const jmap<Key, Value, Pred>& value2)
    {
        return value1.copy() += value2;
    }
    template<typename Key, typename Value, typename Pred>
    jmap<Key, Value, Pred> operator+(jmap<Key, Value, Pred>&& value1, const jmap<Key, Value, Pred>& value2)
    {
        return value1 += value2;
    }
}
