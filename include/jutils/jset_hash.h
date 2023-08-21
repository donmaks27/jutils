// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"
#include "math/hash.h"

#include <unordered_set>

namespace jutils
{
    template<typename T, typename EqualPred = std::equal_to<T>>
        requires jutils::math::hash::hash_info<T>::has_hash && std::predicate<EqualPred, T, T>
    class jset_hash : private std::unordered_set<T, jutils::math::hash::hasher<T>, EqualPred>
    {
    public:

        using base_type = std::unordered_set<T, jutils::math::hash::hasher<T>, EqualPred>;
        using type = typename base_type::key_type;
        using key_hasher_predicator_type = typename base_type::hasher;
        using key_equal_predicator_type = typename base_type::key_equal;
        using size_type = typename base_type::size_type;
        
        using const_iterator = typename base_type::const_iterator;
        using iterator = typename base_type::iterator;

        jset_hash() noexcept = default;
        jset_hash(const std::initializer_list<type> values)
            : base_type(values)
        {}
        jset_hash(const base_type& value)
            : base_type(value)
        {}
        jset_hash(base_type&& value) noexcept
            : base_type(std::move(value))
        {}
        jset_hash(const jset_hash&) = default;
        jset_hash(jset_hash&&) noexcept =default;
        ~jset_hash() noexcept = default;

        jset_hash& operator=(const std::initializer_list<type> values)
        {
            base_type::operator=(values);
            return *this;
        }
        jset_hash& operator=(const base_type& value)
        {
            base_type::operator=(value);
            return *this;
        }
        jset_hash& operator=(base_type&& value) noexcept
        {
            base_type::operator=(std::move(value));
            return *this;
        }
        jset_hash& operator=(const jset_hash&) = default;
        jset_hash& operator=(jset_hash&&) noexcept = default;

        const base_type& toBase() const noexcept { return *this; }

        size_type getSize() const noexcept { return base_type::size(); }
        bool isEmpty() const noexcept { return base_type::empty(); }

        iterator begin() noexcept { return base_type::begin(); }
        iterator end() noexcept { return base_type::end(); }
        const_iterator begin() const noexcept { return base_type::begin(); }
        const_iterator end() const noexcept { return base_type::end(); }
        
        jset_hash copy() const { return *this; }
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
            const const_iterator iter = base_type::find(value);
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

        jset_hash& append(const std::initializer_list<type> values)
        {
            for (const auto& value : values)
            {
                add(value);
            }
            return *this;
        }
        jset_hash& append(const base_type& values)
        {
            for (const auto& value : values)
            {
                add(value);
            }
            return *this;
        }
        jset_hash& append(base_type&& values)
        {
            base_type::merge(std::move(values));
            return *this;
        }
        jset_hash& append(const jset_hash& value) { return append(value.toBase()); }
        jset_hash& append(jset_hash&& value) { return append(static_cast<base_type&&>(value)); }

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

        jset_hash& operator+=(const type& value)
        {
            add(value);
            return *this;
        }
        jset_hash& operator+=(type&& value)
        {
            add(std::move(value));
            return *this;
        }
        jset_hash& operator+=(const std::initializer_list<type> values) { return append(values); }
        jset_hash& operator+=(const base_type& value) { return append(value); }
        jset_hash& operator+=(base_type&& value) { return append(std::move(value)); }
        jset_hash& operator+=(const jset_hash& value) { return append(value); }
        jset_hash& operator+=(jset_hash&& value) { return append(std::move(value)); }
    };
    
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(const jset_hash<T, Pred>& value1, const T& value2) { return value1.copy() += value2; }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(const jset_hash<T, Pred>& value1, T&& value2) { return value1.copy() += std::move(value2); }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(jset_hash<T, Pred>&& value1, const T& value2) { return value1 += value2; }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(jset_hash<T, Pred>&& value1, T&& value2) { return value1 += std::move(value2); }

    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(const T& value1, const jset_hash<T, Pred>& value2) { return value2.copy() += value1; }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(const T& value1, jset_hash<T, Pred>&& value2) { return value2 += value1; }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(T&& value1, const jset_hash<T, Pred>& value2) { return value2.copy() += std::move(value1); }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(T&& value1, jset_hash<T, Pred>&& value2) { return value2 += std::move(value1); }

    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(const jset_hash<T, Pred>& value1, const jset_hash<T, Pred>& value2) { return value1.copy() += value2; }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(const jset_hash<T, Pred>& value1, jset_hash<T, Pred>&& value2) { return value2 += value1; }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(jset_hash<T, Pred>&& value1, const jset_hash<T, Pred>& value2) { return value1 += value2; }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(jset_hash<T, Pred>&& value1, jset_hash<T, Pred>&& value2) { return value1 += std::move(value2); }
}
