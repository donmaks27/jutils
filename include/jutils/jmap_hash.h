// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"
#include "math/hash.h"

#include <unordered_map>
#include <ranges>

namespace jutils
{
    template<typename KeyType, typename ValueType, typename EqualPred = std::equal_to<KeyType>>
        requires jutils::math::hash::hash_info<KeyType>::has_hash && std::predicate<EqualPred, KeyType, KeyType>
    class jmap_hash : private std::unordered_map<KeyType, ValueType, jutils::math::hash::hasher<KeyType>, EqualPred>
    {
    public:

        using base_type = std::unordered_map<KeyType, ValueType, jutils::math::hash::hasher<KeyType>, EqualPred>;
        using key_type = typename base_type::key_type;
        using value_type = typename base_type::mapped_type;
        using key_hasher_predicator_type = typename base_type::hasher;
        using key_equal_predicator_type = typename base_type::key_equal;
        using size_type = typename base_type::size_type;
        using pair_type = typename base_type::value_type;
        
        using const_iterator = typename base_type::const_iterator;
        using iterator = typename base_type::iterator;

        jmap_hash() noexcept = default;
        jmap_hash(const std::initializer_list<pair_type> values)
            : base_type(values)
        {}
        jmap_hash(const base_type& value)
            : base_type(value)
        {}
        jmap_hash(base_type&& value) noexcept
            : base_type(std::move(value))
        {}
        jmap_hash(const jmap_hash&) = default;
        jmap_hash(jmap_hash&&) noexcept =default;
        ~jmap_hash() noexcept = default;

        jmap_hash& operator=(const std::initializer_list<pair_type> values)
        {
            base_type::operator=(values);
            return *this;
        }
        jmap_hash& operator=(const base_type& value)
        {
            base_type::operator=(value);
            return *this;
        }
        jmap_hash& operator=(base_type&& value) noexcept
        {
            base_type::operator=(std::move(value));
            return *this;
        }
        jmap_hash& operator=(const jmap_hash&) = default;
        jmap_hash& operator=(jmap_hash&&) noexcept = default;

        const base_type& toBase() const noexcept { return *this; }

        size_type getSize() const noexcept { return base_type::size(); }
        bool isEmpty() const noexcept { return base_type::empty(); }

        iterator begin() noexcept { return base_type::begin(); }
        iterator end() noexcept { return base_type::end(); }
        const_iterator begin() const noexcept { return base_type::begin(); }
        const_iterator end() const noexcept { return base_type::end(); }

        constexpr auto keys() { return std::views::keys(*this); }
        constexpr auto keys() const { return std::views::keys(*this); }
        constexpr auto values() { return std::views::values(*this); }
        constexpr auto values() const { return std::views::values(*this); }

        jmap_hash copy() const { return *this; }
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
        const value_type& operator[](const key_type& key) const { return get(key); }

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

        template<typename... Args>
        value_type& put(const key_type& key, Args&&... args)
        {
            std::pair<iterator, bool> result = base_type::try_emplace(key, std::forward<Args>(args)...);
            if (!result.second)
            {
                result.first->second = value_type(std::forward<Args>(args)...);
            }
            return result.first->second;
        }
        template<typename... Args>
        value_type& put(key_type&& key, Args&&... args)
        {
            std::pair<iterator, bool> result = base_type::try_emplace(std::move(key), std::forward<Args>(args)...);
            if (!result.second)
            {
                result.first->second = value_type(std::forward<Args>(args)...);
            }
            return result.first->second;
        }

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
        value_type& add(const key_type& key) { return put(key); }
        value_type& add(key_type&& key) { return put(std::move(key)); }

        jmap_hash& append(const std::initializer_list<pair_type> values)
        {
            for (const auto& pair : values)
            {
                add(pair.first, pair.second);
            }
            return *this;
        }
        jmap_hash& append(const base_type& value)
        {
            for (const auto& pair : values)
            {
                add(pair.first, pair.second);
            }
            return *this;
        }
        jmap_hash& append(base_type&& value)
        {
            base_type::merge(std::move(value));
            return *this;
        }
        jmap_hash& append(const jmap_hash& value) { return append(value.toBase()); }
        jmap_hash& append(jmap_hash&& value) { return append(static_cast<base_type&&>(value)); }

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

        jmap_hash& operator+=(const pair_type& value)
        {
            add(value.first, value.second);
            return *this;
        }
        jmap_hash& operator+=(pair_type&& value)
        {
            add(std::move(value.first), std::move(value.second));
            return *this;
        }
        jmap_hash& operator+=(const std::initializer_list<pair_type> values) { return append(values); }
        jmap_hash& operator+=(const base_type& value) { return append(value); }
        jmap_hash& operator+=(base_type&& value) { return append(std::move(value)); }
        jmap_hash& operator+=(const jmap_hash& value) { return append(value); }
        jmap_hash& operator+=(jmap_hash&& value) { return append(std::move(value)); }
    };
    
    template<typename Key, typename Value, typename Pred>
    jmap_hash<Key, Value, Pred> operator+(const jmap_hash<Key, Value, Pred>& value1, const typename jmap_hash<Key, Value, Pred>::pair_type& value2)
    {
        return value1.copy() += value2;
    }
    template<typename Key, typename Value, typename Pred>
    jmap_hash<Key, Value, Pred> operator+(const jmap_hash<Key, Value, Pred>& value1, typename jmap_hash<Key, Value, Pred>::pair_type&& value2)
    {
        return value1.copy() += std::move(value2);
    }
    template<typename Key, typename Value, typename Pred>
    jmap_hash<Key, Value, Pred> operator+(jmap_hash<Key, Value, Pred>&& value1, const typename jmap_hash<Key, Value, Pred>::pair_type& value2)
    {
        return value1 += value2;
    }
    template<typename Key, typename Value, typename Pred>
    jmap_hash<Key, Value, Pred> operator+(jmap_hash<Key, Value, Pred>&& value1, typename jmap_hash<Key, Value, Pred>::pair_type&& value2)
    {
        return value1 += std::move(value2);
    }

    template<typename Key, typename Value, typename Pred>
    jmap_hash<Key, Value, Pred> operator+(const typename jmap_hash<Key, Value, Pred>::pair_type& value1, const jmap_hash<Key, Value, Pred>& value2)
    {
        return value2.copy() += value1;
    }
    template<typename Key, typename Value, typename Pred>
    jmap_hash<Key, Value, Pred> operator+(const typename jmap_hash<Key, Value, Pred>::pair_type& value1, jmap_hash<Key, Value, Pred>&& value2)
    {
        return value2 += value1;
    }
    template<typename Key, typename Value, typename Pred>
    jmap_hash<Key, Value, Pred> operator+(typename jmap_hash<Key, Value, Pred>::pair_type&& value1, const jmap_hash<Key, Value, Pred>& value2)
    {
        return value2.copy() += std::move(value1);
    }
    template<typename Key, typename Value, typename Pred>
    jmap_hash<Key, Value, Pred> operator+(typename jmap_hash<Key, Value, Pred>::pair_type&& value1, jmap_hash<Key, Value, Pred>&& value2)
    {
        return value2 += std::move(value1);
    }

    template<typename Key, typename Value, typename Pred>
    jmap_hash<Key, Value, Pred> operator+(const jmap_hash<Key, Value, Pred>& value1, const jmap_hash<Key, Value, Pred>& value2)
    {
        return value1.copy() += value2;
    }
    template<typename Key, typename Value, typename Pred>
    jmap_hash<Key, Value, Pred> operator+(const jmap_hash<Key, Value, Pred>& value1, jmap_hash<Key, Value, Pred>&& value2)
    {
        return value2 += value1;
    }
    template<typename Key, typename Value, typename Pred>
    jmap_hash<Key, Value, Pred> operator+(jmap_hash<Key, Value, Pred>&& value1, const jmap_hash<Key, Value, Pred>& value2)
    {
        return value1 += value2;
    }
    template<typename Key, typename Value, typename Pred>
    jmap_hash<Key, Value, Pred> operator+(jmap_hash<Key, Value, Pred>&& value1, jmap_hash<Key, Value, Pred>&& value2)
    {
        return value1 += std::move(value2);
    }
}
