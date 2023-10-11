// Copyright © 2022-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"

#include <map>
#include <ranges>

namespace jutils
{
    template<typename KeyType, typename ValueType, typename KeyComparePred = std::less<KeyType>>
        requires std::predicate<KeyComparePred, KeyType, KeyType>
    class jmap
    {
    public:

        using key_type = KeyType;
        using value_type = ValueType;
        using key_compare_predicator_type = KeyComparePred;
        using base_type = std::map<key_type, value_type, key_compare_predicator_type>;
        using size_type = typename base_type::size_type;
        using pair_type = typename base_type::value_type;
        
        using const_iterator = typename base_type::const_iterator;
        using iterator = typename base_type::iterator;

        jmap() noexcept = default;
        jmap(const std::initializer_list<pair_type> values)
            : _internalData(values)
        {}
        jmap(const base_type& value)
            : _internalData(value)
        {}
        jmap(base_type&& value) noexcept
            : _internalData(std::move(value))
        {}
        jmap(const jmap&) = default;
        jmap(jmap&&) noexcept =default;
        ~jmap() noexcept = default;

        jmap& operator=(const std::initializer_list<pair_type> values)
        {
            _internalData = values;
            return *this;
        }
        jmap& operator=(const base_type& value)
        {
            _internalData = value;
            return *this;
        }
        jmap& operator=(base_type&& value) noexcept
        {
            _internalData = std::move(value);
            return *this;
        }
        jmap& operator=(const jmap&) = default;
        jmap& operator=(jmap&&) noexcept = default;

        const base_type& toBase() const noexcept { return _internalData; }

        size_type getSize() const noexcept { return _internalData.size(); }
        bool isEmpty() const noexcept { return _internalData.empty(); }

        iterator begin() noexcept { return _internalData.begin(); }
        iterator end() noexcept { return _internalData.end(); }
        const_iterator begin() const noexcept { return _internalData.begin(); }
        const_iterator end() const noexcept { return _internalData.end(); }

        constexpr auto keys() { return std::views::keys(_internalData); }
        constexpr auto keys() const { return std::views::keys(_internalData); }
        constexpr auto values() { return std::views::values(_internalData); }
        constexpr auto values() const { return std::views::values(_internalData); }

        jmap copy() const { return *this; }
        jarray<key_type> getKeys() const
        {
            jarray<key_type> keys;
            keys.reserve(getSize());
            for (const auto& pair : _internalData)
            {
                keys.add(pair.first);
            }
            return keys;
        }

        value_type& getOrAdd(const key_type& key) { return _internalData[key]; }
        value_type& getOrAdd(key_type&& key) { return _internalData[std::move(key)]; }
        value_type& get(const key_type& key) { return _internalData.at(key); }
        const value_type& get(const key_type& key) const { return _internalData.at(key); }

        value_type& operator[](const key_type& key) { return _internalData[key]; }
        value_type& operator[](key_type&& key) { return _internalData[std::move(key)]; }
        const value_type& operator[](const key_type& key) const { return get(key); }

        value_type* find(const key_type& key) noexcept
        {
            const iterator iter = _internalData.find(key);
            return iter != end() ? &iter->second : nullptr;
        }
        template<typename Pred> requires std::predicate<Pred, key_type, value_type>
        value_type* find(Pred pred) noexcept
        {
            for (auto& pair : _internalData)
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
            const const_iterator iter = _internalData.find(key);
            return iter != end() ? &iter->second : nullptr;
        }
        template<typename Pred> requires std::predicate<Pred, key_type, value_type>
        const value_type* find(Pred pred) const noexcept
        {
            for (const auto& pair : _internalData)
            {
                if (pred(pair.first, pair.second))
                {
                    return &pair.second;
                }
            }
            return nullptr;
        }

        bool contains(const key_type& key) const noexcept { return _internalData.contains(key); }
        template<typename Pred> requires std::predicate<Pred, key_type, value_type>
        bool contains(Pred pred) const noexcept { return find(pred) != nullptr; }

        template<typename... Args>
        value_type& put(const key_type& key, Args&&... args)
        {
            std::pair<iterator, bool> result = _internalData.try_emplace(key, std::forward<Args>(args)...);
            if (!result.second)
            {
                result.first->second = value_type(std::forward<Args>(args)...);
            }
            return result.first->second;
        }
        template<typename... Args>
        value_type& put(key_type&& key, Args&&... args)
        {
            std::pair<iterator, bool> result = _internalData.try_emplace(std::move(key), std::forward<Args>(args)...);
            if (!result.second)
            {
                result.first->second = value_type(std::forward<Args>(args)...);
            }
            return result.first->second;
        }

        value_type& add(const key_type& key, const value_type& value)
        {
            return _internalData.insert_or_assign(key, value).first->second;
        }
        value_type& add(const key_type& key, value_type&& value)
        {
            return _internalData.insert_or_assign(key, std::move(value)).first->second;
        }
        value_type& add(key_type&& key, const value_type& value)
        {
            return _internalData.insert_or_assign(std::move(key), value).first->second;
        }
        value_type& add(key_type&& key, value_type&& value)
        {
            return _internalData.insert_or_assign(std::move(key), std::move(value)).first->second;
        }
        value_type& add(const key_type& key) { return put(key); }
        value_type& add(key_type&& key) { return put(std::move(key)); }

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
            for (const auto& pair : value)
            {
                add(pair.first, pair.second);
            }
            return *this;
        }
        jmap& append(base_type&& value)
        {
            _internalData.merge(std::move(value));
            return *this;
        }
        jmap& append(const jmap& value) { return append(value.toBase()); }
        jmap& append(jmap&& value) { return append(std::move(value._internalData)); }

        bool remove(const key_type& key) noexcept { return _internalData.erase(key) > 0; }
        template<typename Pred> requires std::predicate<Pred, key_type, value_type>
        size_type remove(Pred pred) noexcept
        {
            size_type count = 0;
            for (iterator iter = _internalData.begin(); iter != _internalData.end(); ++iter)
            {
                if (pred(iter->first, iter->second))
                {
                    iter = _internalData.erase(iter);
                    ++count;
                }
            }
            return count;
        }
        void clear() noexcept { _internalData.clear(); }

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
        jmap& operator+=(base_type&& value) { return append(std::move(value)); }
        jmap& operator+=(const jmap& value) { return append(value); }
        jmap& operator+=(jmap&& value) { return append(std::move(value)); }

    private:

        base_type _internalData;
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
    jmap<Key, Value, Pred> operator+(const jmap<Key, Value, Pred>& value1, jmap<Key, Value, Pred>&& value2)
    {
        return value2 += value1;
    }
    template<typename Key, typename Value, typename Pred>
    jmap<Key, Value, Pred> operator+(jmap<Key, Value, Pred>&& value1, const jmap<Key, Value, Pred>& value2)
    {
        return value1 += value2;
    }
    template<typename Key, typename Value, typename Pred>
    jmap<Key, Value, Pred> operator+(jmap<Key, Value, Pred>&& value1, jmap<Key, Value, Pred>&& value2)
    {
        return value1 += std::move(value2);
    }
}
