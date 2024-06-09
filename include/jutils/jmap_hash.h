// Copyright © 2022-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "math/hash.h"
#include "jarray.h"

#include <algorithm>
#include <unordered_map>
#if JUTILS_STD_VERSION >= JUTILS_STD20
    #include <ranges>
#endif

namespace jutils
{
    template<typename KeyType, typename ValueType, typename KeyHash = jutils::hash<KeyType>, typename KeyEqual = std::equal_to<KeyType>>
    class jmap_hash
    {
    public:

        using key_type = KeyType;
        using value_type = ValueType;
        using key_hash_type = KeyHash;
        using key_equal_type = KeyEqual;
        using base_type = std::unordered_map<key_type, value_type, key_hash_type, key_equal_type>;
        using pair_type = typename base_type::value_type;
        using const_iterator = typename base_type::const_iterator;
        using iterator = typename base_type::iterator;

        jmap_hash() noexcept = default;
        jmap_hash(std::initializer_list<pair_type> values)
            : _internalData(values)
        {}
        jmap_hash(const base_type& value)
            : _internalData(value)
        {}
        jmap_hash(base_type&& value) noexcept
            : _internalData(std::move(value))
        {}
        jmap_hash(const jmap_hash&) = default;
        jmap_hash(jmap_hash&&) noexcept = default;
        ~jmap_hash() noexcept = default;

        jmap_hash& operator=(std::initializer_list<pair_type> values)
        {
            _internalData = values;
            return *this;
        }
        jmap_hash& operator=(const base_type& value)
        {
            _internalData = value;
            return *this;
        }
        jmap_hash& operator=(base_type&& value) noexcept
        {
            _internalData = std::move(value);
            return *this;
        }
        jmap_hash& operator=(const jmap_hash& value) = default;
        jmap_hash& operator=(jmap_hash&& value) noexcept = default;

        [[nodiscard]] const base_type& toBase() const noexcept { return _internalData; }

        [[nodiscard]] index_type getSize() const noexcept { return _internalData.size(); }
        [[nodiscard]] bool isEmpty() const noexcept { return _internalData.empty(); }

        [[nodiscard]] iterator begin() noexcept { return _internalData.begin(); }
        [[nodiscard]] iterator end() noexcept { return _internalData.end(); }
        [[nodiscard]] const_iterator begin() const noexcept { return _internalData.begin(); }
        [[nodiscard]] const_iterator end() const noexcept { return _internalData.end(); }

        [[nodiscard]] jmap_hash copy() const noexcept { return *this; }

        [[nodiscard]] value_type& getOrAdd(const key_type& key) noexcept { return _internalData[key]; }
        [[nodiscard]] value_type& getOrAdd(key_type&& key) noexcept { return _internalData[std::move(key)]; }
        [[nodiscard]] value_type& get(const key_type& key) noexcept { return _internalData.at(key); }
        [[nodiscard]] const value_type& get(const key_type& key) const noexcept { return _internalData.at(key); }
        [[nodiscard]] value_type& operator[](const key_type& key) noexcept { return _internalData[key]; }
        [[nodiscard]] value_type& operator[](key_type&& key) noexcept { return _internalData[std::move(key)]; }
        [[nodiscard]] const value_type& operator[](const key_type& key) const noexcept { return get(key); }

        [[nodiscard]] iterator findIter(const key_type& key) noexcept { return _internalData.find(key); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, key_type, value_type>), typename Pred)
        [[nodiscard]] iterator findIter(Pred pred) noexcept
        {
            for (auto iter = begin(); iter != end(); ++iter)
            {
                if (pred(iter->first, iter->second))
                {
                    return iter;
                }
            }
            return end();
        }
        [[nodiscard]] iterator findValueIter(const value_type& value) noexcept;
        [[nodiscard]] const_iterator findIter(const key_type& key) const noexcept { return _internalData.find(key); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, key_type, value_type>), typename Pred)
        [[nodiscard]] const_iterator findIter(Pred pred) const noexcept
        {
            for (auto iter = begin(); iter != end(); ++iter)
            {
                if (pred(iter->first, iter->second))
                {
                    return iter;
                }
            }
            return end();
        }
        [[nodiscard]] const_iterator findValueIter(const value_type& value) const noexcept;

        [[nodiscard]] value_type* find(const key_type& key) noexcept
        {
            const auto iter = findIter(key);
            return iter != end() ? &iter->second : nullptr;
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, key_type, value_type>), typename Pred)
        [[nodiscard]] value_type* find(Pred pred) noexcept
        {
            const auto iter = findIter(pred);
            return iter != end() ? &iter->second : nullptr;
        }
        [[nodiscard]] const value_type* find(const key_type& key) const noexcept
        {
            const auto iter = findIter(key);
            return iter != end() ? &iter->second : nullptr;
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, key_type, value_type>), typename Pred)
        [[nodiscard]] const value_type* find(Pred pred) const noexcept
        {
            const auto iter = findIter(pred);
            return iter != end() ? &iter->second : nullptr;
        }

        [[nodiscard]] bool contains(const key_type& key) const noexcept;
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, key_type, value_type>), typename Pred)
        [[nodiscard]] bool contains(Pred pred) const noexcept { return findIter(pred) != end(); }

#if JUTILS_STD_VERSION >= JUTILS_STD20
        [[nodiscard]] constexpr auto keys() { return std::views::keys(_internalData); }
        [[nodiscard]] constexpr auto keys() const { return std::views::keys(_internalData); }
        [[nodiscard]] constexpr auto values() { return std::views::values(_internalData); }
        [[nodiscard]] constexpr auto values() const { return std::views::values(_internalData); }
#endif
        [[nodiscard]] jarray<key_type> getKeys() const noexcept;
        [[nodiscard]] jarray<value_type> getValues() const noexcept;

        template<typename... Args>
        value_type& put(const key_type& key, Args&&... args);
        template<typename... Args>
        value_type& put(key_type&& key, Args&&... args);

        value_type& add(const key_type& key, const value_type& value) { return _internalData.insert_or_assign(key, value).first->second; }
        value_type& add(const key_type& key, value_type&& value) { return _internalData.insert_or_assign(key, std::move(value)).first->second; }
        value_type& add(key_type&& key, const value_type& value) { return _internalData.insert_or_assign(std::move(key), value).first->second; }
        value_type& add(key_type&& key, value_type&& value) { return _internalData.insert_or_assign(std::move(key), std::move(value)).first->second; }
        value_type& add(const key_type& key) { return put(key); }
        value_type& add(key_type&& key) { return put(std::move(key)); }

        jmap_hash& append(std::initializer_list<pair_type> values)
        {
            _internalData.insert(values);
            return *this;
        }
        jmap_hash& append(const base_type& value)
        {
            if (&_internalData != &value)
            {
                _internalData.insert(value.begin(), value.end());
            }
            return *this;
        }
        jmap_hash& append(base_type&& value)
        {
            _internalData.merge(std::move(value));
            return *this;
        }
        jmap_hash& append(const jmap_hash& value) { return append(value.toBase()); }
        jmap_hash& append(jmap_hash&& value) { return append(std::move(value._internalData)); }

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
        jmap_hash& operator+=(std::initializer_list<pair_type> values) { return append(values); }
        jmap_hash& operator+=(const base_type& value) { return append(value); }
        jmap_hash& operator+=(base_type&& value) { return append(std::move(value)); }
        jmap_hash& operator+=(const jmap_hash& value) { return append(value); }
        jmap_hash& operator+=(jmap_hash&& value) { return append(std::move(value)); }

        index_type remove(const key_type& key) noexcept { return _internalData.erase(key); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, key_type, value_type>), typename Pred)
        index_type remove(Pred pred) noexcept
        {
            index_type count = 0;
            for (auto iter = begin(); iter != end(); ++iter)
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

    private:

        base_type _internalData;
    };

    template<typename Key, typename Value, typename Pred>
    [[nodiscard]] jmap_hash<Key, Value, Pred> operator+(const jmap_hash<Key, Value, Pred>& container, const typename jmap_hash<Key, Value, Pred>::pair_type& value) { return container.copy() += value; }
    template<typename Key, typename Value, typename Pred>
    [[nodiscard]] jmap_hash<Key, Value, Pred> operator+(const jmap_hash<Key, Value, Pred>& container, typename jmap_hash<Key, Value, Pred>::pair_type&& value) { return container.copy() += std::forward<typename jmap_hash<Key, Value, Pred>::pair_type>(value); }
    template<typename Key, typename Value, typename Pred>
    [[nodiscard]] jmap_hash<Key, Value, Pred> operator+(jmap_hash<Key, Value, Pred>&& container, const typename jmap_hash<Key, Value, Pred>::pair_type& value) { return jmap_hash<Key, Value, Pred>(std::move(container)) += value; }
    template<typename Key, typename Value, typename Pred>
    [[nodiscard]] jmap_hash<Key, Value, Pred> operator+(jmap_hash<Key, Value, Pred>&& container, typename jmap_hash<Key, Value, Pred>::pair_type&& value) { return jmap_hash<Key, Value, Pred>(std::move(container)) += std::forward<typename jmap_hash<Key, Value, Pred>::pair_type>(value); }

    template<typename Key, typename Value, typename Pred>
    [[nodiscard]] jmap_hash<Key, Value, Pred> operator+(const jmap_hash<Key, Value, Pred>& container1, std::initializer_list<typename jmap_hash<Key, Value, Pred>::pair_type> list) { return container1.copy() += list; }
    template<typename Key, typename Value, typename Pred>
    [[nodiscard]] jmap_hash<Key, Value, Pred> operator+(jmap_hash<Key, Value, Pred>&& container1, std::initializer_list<typename jmap_hash<Key, Value, Pred>::pair_type> list) { return jmap_hash<Key, Value, Pred>(std::move(container1)) += list; }
    template<typename Key, typename Value, typename Pred>
    [[nodiscard]] jmap_hash<Key, Value, Pred> operator+(const jmap_hash<Key, Value, Pred>& container1, const jmap_hash<Key, Value, Pred>& container2) { return container1.copy() += container2; }
    template<typename Key, typename Value, typename Pred>
    [[nodiscard]] jmap_hash<Key, Value, Pred> operator+(jmap_hash<Key, Value, Pred>&& container1, const jmap_hash<Key, Value, Pred>& container2) { return jmap_hash<Key, Value, Pred>(std::move(container1)) += container2; }
    template<typename Key, typename Value, typename Pred>
    [[nodiscard]] jmap_hash<Key, Value, Pred> operator+(const jmap_hash<Key, Value, Pred>& container1, jmap_hash<Key, Value, Pred>&& container2) { return container1.copy() += std::move(container2); }
    template<typename Key, typename Value, typename Pred>
    [[nodiscard]] jmap_hash<Key, Value, Pred> operator+(jmap_hash<Key, Value, Pred>&& container1, jmap_hash<Key, Value, Pred>&& container2) { return jmap_hash<Key, Value, Pred>(std::move(container1)) += std::move(container2); }

    template<typename KeyType, typename ValueType, typename KeyHash, typename KeyEqual>
    typename jmap_hash<KeyType, ValueType, KeyHash, KeyEqual>::iterator jmap_hash<KeyType, ValueType, KeyHash, KeyEqual>::findValueIter(const value_type& value) noexcept
    {
        for (auto iter = begin(); iter != end(); ++iter)
        {
            if (value == iter->second)
            {
                return iter;
            }
        }
        return end();
    }
    template<typename KeyType, typename ValueType, typename KeyHash, typename KeyEqual>
    typename jmap_hash<KeyType, ValueType, KeyHash, KeyEqual>::const_iterator jmap_hash<KeyType, ValueType, KeyHash, KeyEqual>::findValueIter(const value_type& value) const noexcept
    {
        for (auto iter = begin(); iter != end(); ++iter)
        {
            if (value == iter->second)
            {
                return iter;
            }
        }
        return end();
    }

    template<typename KeyType, typename ValueType, typename KeyHash, typename KeyEqual>
    bool jmap_hash<KeyType, ValueType, KeyHash, KeyEqual>::contains(const key_type& key) const noexcept
    {
#if JUTILS_STD_VERSION >= JUTILS_STD20
        return _internalData.contains(key);
#else
        return findIter(key) != end();
#endif
    }

    template<typename KeyType, typename ValueType, typename KeyHash, typename KeyEqual>
    jarray<KeyType> jmap_hash<KeyType, ValueType, KeyHash, KeyEqual>::getKeys() const noexcept
    {
        jarray<key_type> keys;
        for (const auto& pair : _internalData)
        {
            keys.add(pair.first);
        }
        return keys;
    }
    template<typename KeyType, typename ValueType, typename KeyHash, typename KeyEqual>
    jarray<ValueType> jmap_hash<KeyType, ValueType, KeyHash, KeyEqual>::getValues() const noexcept
    {
        jarray<value_type> values;
        for (const auto& pair : _internalData)
        {
            values.add(pair.second);
        }
        return values;
    }

    template<typename KeyType, typename ValueType, typename KeyHash, typename KeyEqual>
    template<typename... Args>
    ValueType& jmap_hash<KeyType, ValueType, KeyHash, KeyEqual>::put(const key_type& key, Args&& ... args)
    {
        auto result = _internalData.try_emplace(key, std::forward<Args>(args)...);
        if (!result.second)
        {
            result.first->second = value_type(std::forward<Args>(args)...);
        }
        return result.first->second;
    }
    template<typename KeyType, typename ValueType, typename KeyHash, typename KeyEqual>
    template<typename... Args>
    ValueType& jmap_hash<KeyType, ValueType, KeyHash, KeyEqual>::put(key_type&& key, Args&& ... args)
    {
        auto result = _internalData.try_emplace(std::move(key), std::forward<Args>(args)...);
        if (!result.second)
        {
            result.first->second = value_type(std::forward<Args>(args)...);
        }
        return result.first->second;
    }
}
