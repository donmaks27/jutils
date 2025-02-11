// Copyright © 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#include "vector.h"
#include "macro/template_condition.h"

#include <set>

namespace jutils
{
    template<typename T, typename KeyCompare = std::less<T>>
    class set
    {
    public:

        using type = T;
        using key_compare_type = KeyCompare;
        using base_type = std::set<type, key_compare_type>;
        using const_iterator = typename base_type::const_iterator;

        set() noexcept = default;
        set(std::initializer_list<type> values)
            : _internalData(values)
        {}
        set(const base_type& value)
            : _internalData(value)
        {}
        set(base_type&& value) noexcept
            : _internalData(std::move(value))
        {}
        set(const set&) = default;
        set(set&&) noexcept =default;
        ~set() noexcept = default;

        set& operator=(std::initializer_list<type> values)
        {
            _internalData = values;
            return *this;
        }
        set& operator=(const base_type& value)
        {
            _internalData = value;
            return *this;
        }
        set& operator=(base_type&& value) noexcept
        {
            _internalData = std::move(value);
            return *this;
        }
        set& operator=(const set&) = default;
        set& operator=(set&&) noexcept = default;

        [[nodiscard]] const base_type& toBase() const noexcept { return _internalData; }

        [[nodiscard]] std::size_t getSize() const noexcept { return _internalData.size(); }
        [[nodiscard]] bool isEmpty() const noexcept { return _internalData.empty(); }

        [[nodiscard]] const_iterator begin() const noexcept { return _internalData.begin(); }
        [[nodiscard]] const_iterator end() const noexcept { return _internalData.end(); }

        [[nodiscard]] set copy() const noexcept { return *this; }

        [[nodiscard]] const_iterator findIter(const type& key) const noexcept { return _internalData.find(key); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] const_iterator findIter(Pred pred) const noexcept { return std::find_if(begin(), end(), pred); }

        [[nodiscard]] const type* find(const type& key) const noexcept
        {
            const auto iter = this->findIter(key);
            return iter != end() ? &*iter : nullptr;
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] const type* find(Pred pred) const noexcept
        {
            const auto iter = this->findIter(pred);
            return iter != end() ? &*iter : nullptr;
        }

        [[nodiscard]] bool contains(const type& key) const noexcept
        {
#if JUTILS_STD_VERSION >= JUTILS_STD20
            return _internalData.contains(key);
#else
            return this->findIter(key) != end();
#endif
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] bool contains(Pred pred) const noexcept { return this->findIter(pred) != end(); }

        [[nodiscard]] vector<type> toVector() const noexcept
        {
            vector<type> keys;
            keys.reserve(getSize());
            for (const auto& key : _internalData)
            {
                keys.add(key);
            }
            return keys;
        }

        template<typename... Args>
        const type& put(Args&&... args) { return *_internalData.emplace(std::forward<Args>(args)...).first; }

        const type& add(const type& key) { return *_internalData.insert(key).first; }
        const type& add(type&& key) { return *_internalData.insert(std::move(key)).first; }

        set& append(std::initializer_list<type> values)
        {
            _internalData.insert(values);
            return *this;
        }
        set& append(const base_type& value)
        {
            if (&_internalData != &value)
            {
                _internalData.insert(value.begin(), value.end());
            }
            return *this;
        }
        set& append(base_type&& value)
        {
            _internalData.merge(std::move(value));
            return *this;
        }
        set& append(const set& value) { return this->append(value.toBase()); }
        set& append(set&& value) { return this->append(std::move(value._internalData)); }

        set& operator+=(const type& value)
        {
            this->add(value);
            return *this;
        }
        set& operator+=(type&& value)
        {
            this->add(std::move(value));
            return *this;
        }
        set& operator+=(std::initializer_list<type> values) { return this->append(values); }
        set& operator+=(const base_type& value) { return this->append(value); }
        set& operator+=(base_type&& value) { return this->append(std::move(value)); }
        set& operator+=(const set& value) { return this->append(value); }
        set& operator+=(set&& value) { return this->append(std::move(value)); }

        bool remove(const type& key) noexcept { return _internalData.erase(key) != 0; }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
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

    template<typename T, typename KeyCompare>
    [[nodiscard]] set<T, KeyCompare> operator+(set<T, KeyCompare>&& container, const T& value)
    {
        set<T, KeyCompare> result = std::move(container);
        result += value;
        return result;
    }
    template<typename T, typename KeyCompare>
    [[nodiscard]] set<T, KeyCompare> operator+(set<T, KeyCompare>&& container, T&& value)
    {
        set<T, KeyCompare> result = std::move(container);
        result += std::forward<T>(value);
        return result;
    }
    template<typename T, typename KeyCompare>
    [[nodiscard]] set<T, KeyCompare> operator+(const set<T, KeyCompare>& container, const T& value) 
        { return container.copy() + value; }
    template<typename T, typename KeyCompare>
    [[nodiscard]] set<T, KeyCompare> operator+(const set<T, KeyCompare>& container, T&& value) 
        { return container.copy() + std::forward<T>(value); }

    template<typename T, typename KeyCompare>
    [[nodiscard]] set<T, KeyCompare> operator+(set<T, KeyCompare>&& container1, std::initializer_list<T> list)
    {
        set<T, KeyCompare> result = std::move(container1);
        result += list;
        return result;
    }
    template<typename T, typename KeyCompare>
    [[nodiscard]] set<T, KeyCompare> operator+(set<T, KeyCompare>&& container1, const set<T, KeyCompare>& container2)
    {
        set<T, KeyCompare> result = std::move(container1);
        result += container2;
        return result;
    }
    template<typename T, typename KeyCompare>
    [[nodiscard]] set<T, KeyCompare> operator+(set<T, KeyCompare>&& container1, set<T, KeyCompare>&& container2)
    {
        set<T, KeyCompare> result = std::move(container1);
        result += std::move(container2);
        return result;
    }
    template<typename T, typename KeyCompare>
    [[nodiscard]] set<T, KeyCompare> operator+(const set<T, KeyCompare>& container1, std::initializer_list<T> list) 
        { return container1.copy() + list; }
    template<typename T, typename KeyCompare>
    [[nodiscard]] set<T, KeyCompare> operator+(const set<T, KeyCompare>& container1, const set<T, KeyCompare>& container2) 
        { return container1.copy() + container2; }
    template<typename T, typename KeyCompare>
    [[nodiscard]] set<T, KeyCompare> operator+(const set<T, KeyCompare>& container1, set<T, KeyCompare>&& container2) 
        { return container1.copy() + std::move(container2); }
}
