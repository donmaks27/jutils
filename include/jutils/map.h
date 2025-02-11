// Copyright © 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#include "macro/template_condition.h"

#include <vector>
#include <algorithm>
#include <map>
#if JUTILS_STD_VERSION >= JUTILS_STD20
    #include <ranges>
#endif

namespace jutils
{
    template<typename KeyType, typename ValueType, typename KeyCompare = std::less<KeyType>>
    class map
    {
    public:

        using key_type = KeyType;
        using value_type = ValueType;
        using key_compare_type = KeyCompare;
        using base_type = std::map<key_type, value_type, key_compare_type>;
        using pair_type = typename base_type::value_type;
        using const_iterator = typename base_type::const_iterator;
        using iterator = typename base_type::iterator;

        map() noexcept = default;
        map(std::initializer_list<pair_type> values)
            : _internalData(values)
        {}
        map(const base_type& value)
            : _internalData(value)
        {}
        map(base_type&& value) noexcept
            : _internalData(std::move(value))
        {}
        map(const map&) = default;
        map(map&&) noexcept = default;
        ~map() noexcept = default;

        map& operator=(std::initializer_list<pair_type> values)
        {
            _internalData = values;
            return *this;
        }
        map& operator=(const base_type& value)
        {
            _internalData = value;
            return *this;
        }
        map& operator=(base_type&& value) noexcept
        {
            _internalData = std::move(value);
            return *this;
        }
        map& operator=(const map& value) = default;
        map& operator=(map&& value) noexcept = default;

        [[nodiscard]] const base_type& toBase() const noexcept { return _internalData; }

        [[nodiscard]] std::size_t getSize() const noexcept { return _internalData.size(); }
        [[nodiscard]] bool isEmpty() const noexcept { return _internalData.empty(); }

        [[nodiscard]] iterator begin() noexcept { return _internalData.begin(); }
        [[nodiscard]] iterator end() noexcept { return _internalData.end(); }
        [[nodiscard]] const_iterator begin() const noexcept { return _internalData.begin(); }
        [[nodiscard]] const_iterator end() const noexcept { return _internalData.end(); }

        [[nodiscard]] map copy() const noexcept { return *this; }

        [[nodiscard]] value_type& getOrAdd(const key_type& key) noexcept { return _internalData[key]; }
        [[nodiscard]] value_type& getOrAdd(key_type&& key) noexcept { return _internalData[std::move(key)]; }
        [[nodiscard]] value_type& get(const key_type& key) { return _internalData.at(key); }
        [[nodiscard]] const value_type& get(const key_type& key) const { return _internalData.at(key); }
        [[nodiscard]] value_type& operator[](const key_type& key) noexcept { return this->getOrAdd(key); }
        [[nodiscard]] value_type& operator[](key_type&& key) noexcept { return this->getOrAdd(std::move(key)); }
        [[nodiscard]] const value_type& operator[](const key_type& key) const { return this->get(key); }

        [[nodiscard]] iterator findIter(const key_type& key) noexcept { return _internalData.find(key); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, pair_type>), typename Pred)
        [[nodiscard]] iterator findIter(Pred pred) noexcept { return std::find_if(begin(), end(), pred); }
        [[nodiscard]] iterator findValueIter(const value_type& value) noexcept
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
        [[nodiscard]] const_iterator findIter(const key_type& key) const noexcept { return _internalData.find(key); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, pair_type>), typename Pred)
        [[nodiscard]] const_iterator findIter(Pred pred) const noexcept { return std::find_if(begin(), end(), pred); }
        [[nodiscard]] const_iterator findValueIter(const value_type& value) const noexcept
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

        [[nodiscard]] value_type* find(const key_type& key) noexcept
        {
            const auto iter = this->findIter(key);
            return iter != end() ? &iter->second : nullptr;
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, pair_type>), typename Pred)
        [[nodiscard]] value_type* find(Pred pred) noexcept
        {
            const auto iter = this->findIter(pred);
            return iter != end() ? &iter->second : nullptr;
        }
        [[nodiscard]] const value_type* find(const key_type& key) const noexcept
        {
            const auto iter = this->findIter(key);
            return iter != end() ? &iter->second : nullptr;
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, pair_type>), typename Pred)
        [[nodiscard]] const value_type* find(Pred pred) const noexcept
        {
            const auto iter = this->findIter(pred);
            return iter != end() ? &iter->second : nullptr;
        }

        [[nodiscard]] bool contains(const key_type& key) const noexcept
        {
#if JUTILS_STD_VERSION >= JUTILS_STD20
            return _internalData.contains(key);
#else
            return this->findIter(key) != end();
#endif
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, pair_type>), typename Pred)
        [[nodiscard]] bool contains(Pred pred) const noexcept { return this->findIter(pred) != end(); }

#if JUTILS_STD_VERSION >= JUTILS_STD20
        [[nodiscard]] constexpr auto keys() { return std::views::keys(_internalData); }
        [[nodiscard]] constexpr auto keys() const { return std::views::keys(_internalData); }
        [[nodiscard]] constexpr auto values() { return std::views::values(_internalData); }
        [[nodiscard]] constexpr auto values() const { return std::views::values(_internalData); }
#endif
        [[nodiscard]] std::vector<key_type> getKeys() const noexcept
        {
            std::vector<key_type> keys;
            keys.reserve(getSize());
            for (const auto& pair : _internalData)
            {
                keys.push_back(pair.first);
            }
            return keys;
        }
        [[nodiscard]] std::vector<value_type> getValues() const noexcept
        {
            std::vector<value_type> values;
            values.reserve(getSize());
            for (const auto& pair : _internalData)
            {
                values.push_back(pair.second);
            }
            return values;
        }

        template<typename... Args>
        value_type& put(const key_type& key, Args&&... args)
        {
            auto result = _internalData.try_emplace(key, std::forward<Args>(args)...);
            if (!result.second)
            {
                result.first->second = value_type(std::forward<Args>(args)...);
            }
            return result.first->second;
        }
        template<typename... Args>
        value_type& put(key_type&& key, Args&&... args)
        {
            auto result = _internalData.try_emplace(std::move(key), std::forward<Args>(args)...);
            if (!result.second)
            {
                result.first->second = value_type(std::forward<Args>(args)...);
            }
            return result.first->second;
        }

        value_type& add(const key_type& key, const value_type& value) { return _internalData.insert_or_assign(key, value).first->second; }
        value_type& add(const key_type& key, value_type&& value) { return _internalData.insert_or_assign(key, std::move(value)).first->second; }
        value_type& add(key_type&& key, const value_type& value) { return _internalData.insert_or_assign(std::move(key), value).first->second; }
        value_type& add(key_type&& key, value_type&& value) { return _internalData.insert_or_assign(std::move(key), std::move(value)).first->second; }
        value_type& add(const key_type& key) { return put(key); }
        value_type& add(key_type&& key) { return put(std::move(key)); }

        map& append(std::initializer_list<pair_type> values)
        {
            _internalData.insert(values);
            return *this;
        }
        map& append(const base_type& value)
        {
            if (&_internalData != &value)
            {
                _internalData.insert(value.begin(), value.end());
            }
            return *this;
        }
        map& append(base_type&& value)
        {
            _internalData.merge(std::move(value));
            return *this;
        }
        map& append(const map& value) { return append(value.toBase()); }
        map& append(map&& value) { return append(std::move(value._internalData)); }

        map& operator+=(const pair_type& value)
        {
            add(value.first, value.second);
            return *this;
        }
        map& operator+=(pair_type&& value)
        {
            add(std::move(value.first), std::move(value.second));
            return *this;
        }
        map& operator+=(std::initializer_list<pair_type> values) { return append(values); }
        map& operator+=(const base_type& value) { return append(value); }
        map& operator+=(base_type&& value) { return append(std::move(value)); }
        map& operator+=(const map& value) { return append(value); }
        map& operator+=(map&& value) { return append(std::move(value)); }

        bool remove(const key_type& key) noexcept { return _internalData.erase(key) != 0; }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, pair_type>), typename Pred)
        std::size_t remove(Pred pred) noexcept
        {
            std::size_t count = 0;
            for (auto iter = begin(); iter != end(); ++iter)
            {
                if (pred(*iter))
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

    template<typename Key, typename Value, typename KeyCompare>
    [[nodiscard]] map<Key, Value, KeyCompare> operator+(map<Key, Value, KeyCompare>&& container, const typename map<Key, Value, KeyCompare>::pair_type& value)
    {
        map<Key, Value, KeyCompare> result = std::move(container);
        result += value;
        return result;
    }
    template<typename Key, typename Value, typename KeyCompare>
    [[nodiscard]] map<Key, Value, KeyCompare> operator+(map<Key, Value, KeyCompare>&& container, typename map<Key, Value, KeyCompare>::pair_type&& value)
    {
        map<Key, Value, KeyCompare> result = std::move(container);
        result += std::forward<typename map<Key, Value, KeyCompare>::pair_type>(value);
        return result;
    }
    template<typename Key, typename Value, typename KeyCompare>
    [[nodiscard]] map<Key, Value, KeyCompare> operator+(const map<Key, Value, KeyCompare>& container, const typename map<Key, Value, KeyCompare>::pair_type& value)
        { return container.copy() + value; }
    template<typename Key, typename Value, typename KeyCompare>
    [[nodiscard]] map<Key, Value, KeyCompare> operator+(const map<Key, Value, KeyCompare>& container, typename map<Key, Value, KeyCompare>::pair_type&& value)
        { return container.copy() + std::forward<typename map<Key, Value, KeyCompare>::pair_type>(value); }
    
    template<typename Key, typename Value, typename KeyCompare>
    [[nodiscard]] map<Key, Value, KeyCompare> operator+(map<Key, Value, KeyCompare>&& container1, std::initializer_list<typename map<Key, Value, KeyCompare>::pair_type> list)
    {
        map<Key, Value, KeyCompare> result = std::move(container1);
        result += list;
        return result;
    }
    template<typename Key, typename Value, typename KeyCompare>
    [[nodiscard]] map<Key, Value, KeyCompare> operator+(map<Key, Value, KeyCompare>&& container1, const map<Key, Value, KeyCompare>& container2)
    {
        map<Key, Value, KeyCompare> result = std::move(container1);
        result += container2;
        return result;
    }
    template<typename Key, typename Value, typename KeyCompare>
    [[nodiscard]] map<Key, Value, KeyCompare> operator+(map<Key, Value, KeyCompare>&& container1, map<Key, Value, KeyCompare>&& container2)
    {
        map<Key, Value, KeyCompare> result = std::move(container1);
        result += std::move(container2);
        return result;
    }
    template<typename Key, typename Value, typename KeyCompare>
    [[nodiscard]] map<Key, Value, KeyCompare> operator+(const map<Key, Value, KeyCompare>& container1, std::initializer_list<typename map<Key, Value, KeyCompare>::pair_type> list)
        { return container1.copy() + list; }
    template<typename Key, typename Value, typename KeyCompare>
    [[nodiscard]] map<Key, Value, KeyCompare> operator+(const map<Key, Value, KeyCompare>& container1, const map<Key, Value, KeyCompare>& container2)
        { return container1.copy() + container2; }
    template<typename Key, typename Value, typename KeyCompare>
    [[nodiscard]] map<Key, Value, KeyCompare> operator+(const map<Key, Value, KeyCompare>& container1, map<Key, Value, KeyCompare>&& container2)
        { return container1.copy() + std::move(container2); }
}
