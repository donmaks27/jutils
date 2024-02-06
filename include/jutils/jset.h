// Copyright © 2022-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"

#include <set>

namespace jutils
{
    template<typename T, typename KeyCompare = std::less<T>>
    class jset
    {
    public:

        using type = T;
        using key_compare_type = KeyCompare;
        using base_type = std::set<type, key_compare_type>;
        using const_iterator = typename base_type::const_iterator;

        jset() noexcept = default;
        jset(std::initializer_list<type> values)
            : _internalData(values)
        {}
        jset(const base_type& value)
            : _internalData(value)
        {}
        jset(base_type&& value) noexcept
            : _internalData(std::move(value))
        {}
        jset(const jset&) = default;
        jset(jset&&) noexcept =default;
        ~jset() noexcept = default;

        jset& operator=(std::initializer_list<type> values)
        {
            _internalData = values;
            return *this;
        }
        jset& operator=(const base_type& value)
        {
            _internalData = value;
            return *this;
        }
        jset& operator=(base_type&& value) noexcept
        {
            _internalData = std::move(value);
            return *this;
        }
        jset& operator=(const jset&) = default;
        jset& operator=(jset&&) noexcept = default;

        [[nodiscard]] const base_type& toBase() const noexcept { return _internalData; }

        [[nodiscard]] index_type getSize() const noexcept { return _internalData.size(); }
        [[nodiscard]] bool isEmpty() const noexcept { return _internalData.empty(); }

        [[nodiscard]] const_iterator begin() const noexcept { return _internalData.begin(); }
        [[nodiscard]] const_iterator end() const noexcept { return _internalData.end(); }

        [[nodiscard]] jset copy() const noexcept { return *this; }

        [[nodiscard]] const_iterator findIter(const type& key) const noexcept { return _internalData.find(key); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] const_iterator findIter(Pred pred) const noexcept;

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
        const type& put(Args&&... args) { return *_internalData.emplace(std::forward<Args>(args)...).first; }

        const type& add(const type& key) { return *_internalData.insert(key).first; }
        const type& add(type&& key) { return *_internalData.insert(std::move(key)).first; }

        jset& append(std::initializer_list<type> values)
        {
            _internalData.insert(values);
            return *this;
        }
        jset& append(const base_type& value)
        {
            if (this != &value)
            {
                _internalData.insert(value.begin(), value.end());
            }
            return *this;
        }
        jset& append(base_type&& value)
        {
            _internalData.merge(std::move(value));
            return *this;
        }
        jset& append(const jset& value) { return append(value.toBase()); }
        jset& append(jset&& value) { return append(std::move(value._internalData)); }

        jset& operator+=(const type& value)
        {
            add(value);
            return *this;
        }
        jset& operator+=(type&& value)
        {
            add(std::move(value.first));
            return *this;
        }
        jset& operator+=(std::initializer_list<type> values) { return append(values); }
        jset& operator+=(const base_type& value) { return append(value); }
        jset& operator+=(base_type&& value) { return append(std::move(value)); }
        jset& operator+=(const jset& value) { return append(value); }
        jset& operator+=(jset&& value) { return append(std::move(value)); }

        index_type remove(const type& key) noexcept { return _internalData.erase(key); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        index_type remove(Pred pred) noexcept;
        void clear() noexcept { _internalData.clear(); }

    private:

        base_type _internalData;
    };

    template<typename T, typename KeyCompare>
    [[nodiscard]] jset<T, KeyCompare> operator+(const jset<T, KeyCompare>& container, const typename jset<T, KeyCompare>::pair_type& value) { return container.copy() += value; }
    template<typename T, typename KeyCompare>
    [[nodiscard]] jset<T, KeyCompare> operator+(const jset<T, KeyCompare>& container, typename jset<T, KeyCompare>::pair_type&& value) { return container.copy() += std::forward(value); }
    template<typename T, typename KeyCompare>
    [[nodiscard]] jset<T, KeyCompare> operator+(jset<T, KeyCompare>&& container, const typename jset<T, KeyCompare>::pair_type& value) { return container += value; }
    template<typename T, typename KeyCompare>
    [[nodiscard]] jset<T, KeyCompare> operator+(jset<T, KeyCompare>&& container, typename jset<T, KeyCompare>::pair_type&& value) { return container += std::forward(value); }

    template<typename T, typename KeyCompare>
    [[nodiscard]] jset<T, KeyCompare> operator+(const jset<T, KeyCompare>& container1, std::initializer_list<typename jset<T, KeyCompare>::pair_type> list) { return container1.copy() += list; }
    template<typename T, typename KeyCompare>
    [[nodiscard]] jset<T, KeyCompare> operator+(jset<T, KeyCompare>&& container1, std::initializer_list<typename jset<T, KeyCompare>::pair_type> list) { return container1 += list; }
    template<typename T, typename KeyCompare>
    [[nodiscard]] jset<T, KeyCompare> operator+(const jset<T, KeyCompare>& container1, const jset<T, KeyCompare>& container2) { return container1.copy() += container2; }
    template<typename T, typename KeyCompare>
    [[nodiscard]] jset<T, KeyCompare> operator+(jset<T, KeyCompare>&& container1, const jset<T, KeyCompare>& container2) { return container1 += container2; }
    template<typename T, typename KeyCompare>
    [[nodiscard]] jset<T, KeyCompare> operator+(const jset<T, KeyCompare>& container1, jset<T, KeyCompare>&& container2) { return container1.copy() += std::move(container2); }
    template<typename T, typename KeyCompare>
    [[nodiscard]] jset<T, KeyCompare> operator+(jset<T, KeyCompare>&& container1, jset<T, KeyCompare>&& container2) { return container1 += std::move(container2); }

    template<typename T, typename KeyCompare>
    JUTILS_TEMPLATE_CONDITION_IMPL((jutils::is_predicate_v<Pred, T>), typename Pred)
    typename jset<T, KeyCompare>::const_iterator jset<T, KeyCompare>::findIter(Pred pred) const noexcept
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

    template<typename T, typename KeyCompare>
    bool jset<T, KeyCompare>::contains(const type& key) const noexcept
    {
#if JUTILS_STD_VERSION >= JUTILS_STD20
        return _internalData.contains(key);
#else
        return findIter(key) != end();
#endif
    }

    template<typename T, typename KeyCompare>
    jarray<T> jset<T, KeyCompare>::getKeys() const noexcept
    {
        jarray<type> keys;
        for (const auto& key : _internalData)
        {
            keys.add(key);
        }
        return keys;
    }

    template<typename T, typename KeyCompare>
    JUTILS_TEMPLATE_CONDITION_IMPL((jutils::is_predicate_v<Pred, T>), typename Pred)
    index_type jset<T, KeyCompare>::remove(Pred pred) noexcept
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
