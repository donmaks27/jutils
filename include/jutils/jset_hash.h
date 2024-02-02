// Copyright © 2022-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"
#include "math/hash.h"

#include <unordered_set>

namespace jutils
{
    template<typename T, typename KeyHash = jutils::hash<T>, typename KeyEqual = std::equal_to<T>>
    class jset_hash
    {
    public:

        using type = T;
        using key_hash_type = KeyHash;
        using key_equal_type = KeyEqual;
        using base_type = std::unordered_set<type, key_hash_type, key_equal_type>;
        using const_iterator = typename base_type::const_iterator;
        using iterator = typename base_type::iterator;

        jset_hash() noexcept = default;
        jset_hash(std::initializer_list<type> values)
            : _internalData(values)
        {}
        jset_hash(const base_type& value)
            : _internalData(value)
        {}
        jset_hash(base_type&& value) noexcept
        : _internalData(std::move(value))
        {}
        jset_hash(const jset_hash&) = default;
        jset_hash(jset_hash&&) noexcept =default;
        ~jset_hash() noexcept = default;

        jset_hash& operator=(std::initializer_list<type> values)
        {
            _internalData = values;
            return *this;
        }
        jset_hash& operator=(const base_type& value)
        {
            _internalData = value;
            return *this;
        }
        jset_hash& operator=(base_type&& value) noexcept
        {
            _internalData = std::move(value);
            return *this;
        }
        jset_hash& operator=(const jset_hash&) = default;
        jset_hash& operator=(jset_hash&&) noexcept = default;

        [[nodiscard]] const base_type& toBase() const noexcept { return _internalData; }

        [[nodiscard]] index_type getSize() const noexcept { return _internalData.size(); }
        [[nodiscard]] bool isEmpty() const noexcept { return _internalData.empty(); }

        [[nodiscard]] iterator begin() noexcept { return _internalData.begin(); }
        [[nodiscard]] iterator end() noexcept { return _internalData.end(); }
        [[nodiscard]] const_iterator begin() const noexcept { return _internalData.begin(); }
        [[nodiscard]] const_iterator end() const noexcept { return _internalData.end(); }

        [[nodiscard]] jset_hash copy() const noexcept { return *this; }

        [[nodiscard]] iterator findIter(const type& key) noexcept { return _internalData.find(key); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] iterator findIter(Pred pred) noexcept;
        [[nodiscard]] const_iterator findIter(const type& key) const noexcept { return _internalData.find(key); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] const_iterator findIter(Pred pred) const noexcept;

        [[nodiscard]] type* find(const type& key) noexcept
        {
            const auto iter = findIter(key);
            return iter != end() ? &*iter : nullptr;
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] type* find(Pred pred) noexcept
        {
            const auto iter = findIter(pred);
            return iter != end() ? &*iter : nullptr;
        }
        [[nodiscard]] const type* find(const type& key) const noexcept
        {
            const auto iter = findIter(key);
            return iter != end() ? &*iter : nullptr;
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] const type* find(Pred pred) const noexcept
        {
            const auto iter = findIter(pred);
            return iter != end() ? &*iter : nullptr;
        }

        [[nodiscard]] bool contains(const type& key) const noexcept;
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] bool contains(Pred pred) const noexcept { return findIter(pred) != end(); }

        [[nodiscard]] jarray<type> getKeys() const noexcept;

        template<typename... Args>
        type& put(Args&&... args);

        type& add(const type& key);
        type& add(type&& key);

        jset_hash& append(std::initializer_list<type> values)
        {
            _internalData.insert(values);
            return *this;
        }
        jset_hash& append(const base_type& value)
        {
            if (this != &value)
            {
                _internalData.insert(value.begin(), value.end());
            }
            return *this;
        }
        jset_hash& append(base_type&& value)
        {
            _internalData.merge(std::move(value));
            return *this;
        }
        jset_hash& append(const jset_hash& value) { return append(value.toBase()); }
        jset_hash& append(jset_hash&& value) { return append(std::move(value._internalData)); }

        jset_hash& operator+=(const type& value)
        {
            add(value);
            return *this;
        }
        jset_hash& operator+=(type&& value)
        {
            add(std::move(value.first));
            return *this;
        }
        jset_hash& operator+=(std::initializer_list<type> values) { return append(values); }
        jset_hash& operator+=(const base_type& value) { return append(value); }
        jset_hash& operator+=(base_type&& value) { return append(std::move(value)); }
        jset_hash& operator+=(const jset_hash& value) { return append(value); }
        jset_hash& operator+=(jset_hash&& value) { return append(std::move(value)); }

        index_type remove(const type& key) noexcept { return _internalData.erase(key); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        index_type remove(Pred pred) noexcept;
        void clear() noexcept { _internalData.clear(); }

    private:

        base_type _internalData;
    };

    template<typename T, typename KeyHash, typename KeyEqual>
    [[nodiscard]] jset_hash<T, KeyHash, KeyEqual> operator+(const jset_hash<T, KeyHash, KeyEqual>& container, const typename jset_hash<T, KeyHash, KeyEqual>::pair_type& value) { return container.copy() += value; }
    template<typename T, typename KeyHash, typename KeyEqual>
    [[nodiscard]] jset_hash<T, KeyHash, KeyEqual> operator+(const jset_hash<T, KeyHash, KeyEqual>& container, typename jset_hash<T, KeyHash, KeyEqual>::pair_type&& value) { return container.copy() += std::forward(value); }
    template<typename T, typename KeyHash, typename KeyEqual>
    [[nodiscard]] jset_hash<T, KeyHash, KeyEqual> operator+(jset_hash<T, KeyHash, KeyEqual>&& container, const typename jset_hash<T, KeyHash, KeyEqual>::pair_type& value) { return container += value; }
    template<typename T, typename KeyHash, typename KeyEqual>
    [[nodiscard]] jset_hash<T, KeyHash, KeyEqual> operator+(jset_hash<T, KeyHash, KeyEqual>&& container, typename jset_hash<T, KeyHash, KeyEqual>::pair_type&& value) { return container += std::forward(value); }

    template<typename T, typename KeyHash, typename KeyEqual>
    [[nodiscard]] jset_hash<T, KeyHash, KeyEqual> operator+(const jset_hash<T, KeyHash, KeyEqual>& container1, std::initializer_list<typename jset_hash<T, KeyHash, KeyEqual>::pair_type> list) { return container1.copy() += list; }
    template<typename T, typename KeyHash, typename KeyEqual>
    [[nodiscard]] jset_hash<T, KeyHash, KeyEqual> operator+(jset_hash<T, KeyHash, KeyEqual>&& container1, std::initializer_list<typename jset_hash<T, KeyHash, KeyEqual>::pair_type> list) { return container1 += list; }
    template<typename T, typename KeyHash, typename KeyEqual>
    [[nodiscard]] jset_hash<T, KeyHash, KeyEqual> operator+(const jset_hash<T, KeyHash, KeyEqual>& container1, const jset_hash<T, KeyHash, KeyEqual>& container2) { return container1.copy() += container2; }
    template<typename T, typename KeyHash, typename KeyEqual>
    [[nodiscard]] jset_hash<T, KeyHash, KeyEqual> operator+(jset_hash<T, KeyHash, KeyEqual>&& container1, const jset_hash<T, KeyHash, KeyEqual>& container2) { return container1 += container2; }
    template<typename T, typename KeyHash, typename KeyEqual>
    [[nodiscard]] jset_hash<T, KeyHash, KeyEqual> operator+(const jset_hash<T, KeyHash, KeyEqual>& container1, jset_hash<T, KeyHash, KeyEqual>&& container2) { return container1.copy() += std::move(container2); }
    template<typename T, typename KeyHash, typename KeyEqual>
    [[nodiscard]] jset_hash<T, KeyHash, KeyEqual> operator+(jset_hash<T, KeyHash, KeyEqual>&& container1, jset_hash<T, KeyHash, KeyEqual>&& container2) { return container1 += std::move(container2); }

    template<typename T, typename KeyHash, typename KeyEqual>
    JUTILS_TEMPLATE_CONDITION_IMPL((jutils::is_predicate_v<Pred, T>), typename Pred)
    typename jset_hash<T, KeyHash, KeyEqual>::iterator jset_hash<T, KeyHash, KeyEqual>::findIter(Pred pred) noexcept
    {
        for (auto iter = begin(); iter != end(); ++iter)
        {
            if (pred(*iter))
            {
                return iter;
            }
        }
        return end();
    }
    template<typename T, typename KeyHash, typename KeyEqual>
    JUTILS_TEMPLATE_CONDITION_IMPL((jutils::is_predicate_v<Pred, T>), typename Pred)
    typename jset_hash<T, KeyHash, KeyEqual>::const_iterator jset_hash<T, KeyHash, KeyEqual>::findIter(Pred pred) const noexcept
    {
        for (auto iter = begin(); iter != end(); ++iter)
        {
            if (pred(*iter))
            {
                return iter;
            }
        }
        return end();
    }

    template<typename T, typename KeyHash, typename KeyEqual>
    bool jset_hash<T, KeyHash, KeyEqual>::contains(const type& key) const noexcept
    {
#if JUTILS_STD_VERSION >= JUTILS_STD20
        return _internalData.contains(key);
#else
        return findIter(key) != end();
#endif
    }

    template<typename T, typename KeyHash, typename KeyEqual>
    jarray<T> jset_hash<T, KeyHash, KeyEqual>::getKeys() const noexcept
    {
        jarray<type> keys;
        for (const auto& key : _internalData)
        {
            keys.add(key);
        }
        return keys;
    }

    template<typename T, typename KeyHash, typename KeyEqual>
    template<typename... Args>
    T& jset_hash<T, KeyHash, KeyEqual>::put(Args&& ... args)
    {
        auto result = _internalData.emplace(std::forward<Args>(args)...);
        if (!result.second)
        {
            *result.first = type(std::forward<Args>(args)...);
        }
        return *result.first;
    }

    template<typename T, typename KeyHash, typename KeyEqual>
    T& jset_hash<T, KeyHash, KeyEqual>::add(const type& key)
    {
        auto result = _internalData.insert(key);
        if (!result.second)
        {
            *result.first = key;
        }
        return *result.first;
    }
    template<typename T, typename KeyHash, typename KeyEqual>
    T& jset_hash<T, KeyHash, KeyEqual>::add(type&& key)
    {
        auto result = _internalData.insert(std::move(key));
        if (!result.second)
        {
            *result.first = std::move(key);
        }
        return *result.first;
    }

    template<typename T, typename KeyHash, typename KeyEqual>
    JUTILS_TEMPLATE_CONDITION_IMPL((jutils::is_predicate_v<Pred, T>), typename Pred)
    index_type jset_hash<T, KeyHash, KeyEqual>::remove(Pred pred) noexcept
    {
        index_type count = 0;
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
}
