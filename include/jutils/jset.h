// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"

#include <set>

namespace jutils
{
    template<typename T, typename ComparePred = std::less<T>>
    class jset : private std::set<T, ComparePred>
    {
    public:

        using base_type = std::set<T, ComparePred>;
        using type = typename base_type::key_type;
        using key_compare_predicator_type = typename base_type::key_compare;
        using size_type = typename base_type::size_type;
        
        using const_iterator = typename base_type::const_iterator;
        using iterator = typename base_type::iterator;

        jset() noexcept = default;
        jset(const std::initializer_list<type> values)
            : base_type(values)
        {}
        jset(const base_type& value)
            : base_type(value)
        {}
        jset(base_type&& value) noexcept
            : base_type(std::move(value))
        {}
        jset(const jset&) = default;
        jset(jset&&) noexcept =default;
        ~jset() noexcept = default;

        jset& operator=(const std::initializer_list<type> values)
        {
            base_type::operator=(values);
            return *this;
        }
        jset& operator=(const base_type& value)
        {
            base_type::operator=(value);
            return *this;
        }
        jset& operator=(base_type&& value) noexcept
        {
            base_type::operator=(std::move(value));
            return *this;
        }
        jset& operator=(const jset&) = default;
        jset& operator=(jset&&) noexcept = default;

        const base_type& toBase() const noexcept { return *this; }

        size_type getSize() const noexcept { return base_type::size(); }
        bool isEmpty() const noexcept { return base_type::empty(); }

        iterator begin() noexcept { return base_type::begin(); }
        iterator end() noexcept { return base_type::end(); }
        const_iterator begin() const noexcept { return base_type::begin(); }
        const_iterator end() const noexcept { return base_type::end(); }
        
        jset copy() const { return *this; }
        jarray<T> getValues() const
        {
            jarray<T> values;
            for (const auto& value : *this)
            {
                values.add(value);
            }
            return values;
        }
        
        const type* find(const type& value) const noexcept
        {
            const iterator iter = base_type::find(value);
            return iter != end() ? iter.operator->() : nullptr;
        }
        template<typename Pred> requires std::predicate<Pred, type>
        const type* find(Pred pred) const noexcept
        {
            for (auto& value : *this)
            {
                if (pred(value))
                {
                    return &value;
                }
            }
            return nullptr;
        }

        bool contains(const type& value) const noexcept { return find(value) != nullptr; }
        template<typename Pred> requires std::predicate<Pred, type>
        bool contains(Pred pred) const noexcept { return find<Pred>(pred) != nullptr; }

        const type& add(const type& value) { return *base_type::insert(value).first; }
        const type& add(type&& value) { return *base_type::emplace(std::move(value)).first; }

        jset& append(const std::initializer_list<type> values)
        {
            for (const auto& value : values)
            {
                add(value);
            }
            return *this;
        }
        jset& append(const base_type& values)
        {
            for (const auto& value : values)
            {
                add(value);
            }
            return *this;
        }
        jset& append(base_type&& values)
        {
            base_type::merge(std::move(values));
            return *this;
        }
        jset& append(const jset& value) { return append(value.toBase()); }
        jset& append(jset&& value) { return append(static_cast<base_type&&>(value)); }

        bool remove(const type& value) noexcept { return base_type::erase(value) > 0; }
        template<typename Pred> requires std::predicate<Pred, type>
        size_type remove(Pred pred) noexcept
        {
            size_type count = 0;
            for (iterator iter = begin(); iter != end(); ++iter)
            {
                if (pred(*iter))
                {
                    iter = base_type::erase(iter);
                    ++count;
                }
            }
            return count;
        }
        void clear() noexcept { base_type::clear(); }

        jset& operator+=(const type& value)
        {
            add(value);
            return *this;
        }
        jset& operator+=(type&& value)
        {
            add(std::move(value));
            return *this;
        }
        jset& operator+=(const std::initializer_list<type> values) { return append(values); }
        jset& operator+=(const base_type& value) { return append(value); }
        jset& operator+=(base_type&& value) { return append(std::move(value)); }
        jset& operator+=(const jset& value) { return append(value); }
        jset& operator+=(jset&& value) { return append(std::move(value)); }
    };
    
    template<typename T, typename Pred>
    jset<T, Pred> operator+(const jset<T, Pred>& value1, const T& value2) { return value1.copy() += value2; }
    template<typename T, typename Pred>
    jset<T, Pred> operator+(const jset<T, Pred>& value1, T&& value2) { return value1.copy() += std::move(value2); }
    template<typename T, typename Pred>
    jset<T, Pred> operator+(jset<T, Pred>&& value1, const T& value2) { return value1 += value2; }
    template<typename T, typename Pred>
    jset<T, Pred> operator+(jset<T, Pred>&& value1, T&& value2) { return value1 += std::move(value2); }

    template<typename T, typename Pred>
    jset<T, Pred> operator+(const T& value1, const jset<T, Pred>& value2) { return value2.copy() += value1; }
    template<typename T, typename Pred>
    jset<T, Pred> operator+(const T& value1, jset<T, Pred>&& value2) { return value2 += value1; }
    template<typename T, typename Pred>
    jset<T, Pred> operator+(T&& value1, const jset<T, Pred>& value2) { return value2.copy() += std::move(value1); }
    template<typename T, typename Pred>
    jset<T, Pred> operator+(T&& value1, jset<T, Pred>&& value2) { return value2 += std::move(value1); }

    template<typename T, typename Pred>
    jset<T, Pred> operator+(const jset<T, Pred>& value1, const jset<T, Pred>& value2) { return value1.copy() += value2; }
    template<typename T, typename Pred>
    jset<T, Pred> operator+(const jset<T, Pred>& value1, jset<T, Pred>&& value2) { return value2 += value1; }
    template<typename T, typename Pred>
    jset<T, Pred> operator+(jset<T, Pred>&& value1, const jset<T, Pred>& value2) { return value1 += value2; }
    template<typename T, typename Pred>
    jset<T, Pred> operator+(jset<T, Pred>&& value1, jset<T, Pred>&& value2) { return value1 += std::move(value2); }
}
