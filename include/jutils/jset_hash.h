// Copyright © 2022-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"
#include "math/hash.h"

#include <unordered_set>

namespace jutils
{
    JUTILS_TEMPLATE_CONDITION(
        (jutils::math::hash::hash_info<T>::has_hash && jutils::is_predicate_v<EqualPred, T, T>),
        typename T, typename EqualPred = std::equal_to<T>
    )
    class jset_hash
    {
    public:

        using type = T;
        using key_hasher_predicator_type = jutils::math::hash::hasher<T>;
        using key_equal_predicator_type = EqualPred;
        using base_type = std::unordered_set<type, key_hasher_predicator_type, key_equal_predicator_type>;
        using size_type = typename base_type::size_type;
        
        using const_iterator = typename base_type::const_iterator;
        using iterator = typename base_type::iterator;

        jset_hash() noexcept = default;
        jset_hash(const std::initializer_list<type> values)
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

        jset_hash& operator=(const std::initializer_list<type> values)
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

        const base_type& toBase() const noexcept { return _internalData; }

        size_type getSize() const noexcept { return _internalData.size(); }
        bool isEmpty() const noexcept { return _internalData.empty(); }

        iterator begin() noexcept { return _internalData.begin(); }
        iterator end() noexcept { return _internalData.end(); }
        const_iterator begin() const noexcept { return _internalData.begin(); }
        const_iterator end() const noexcept { return _internalData.end(); }
        
        jset_hash copy() const { return *this; }
        jarray<T> getValues() const
        {
            jarray<T> values;
            for (const auto& value : _internalData)
            {
                values.add(value);
            }
            return values;
        }
        
        const type* find(const type& value) const noexcept
        {
            const const_iterator iter = _internalData.find(value);
            return iter != end() ? iter.operator->() : nullptr;
        }
        template<typename Pred> requires std::predicate<Pred, type>
        const type* find(Pred pred) const noexcept
        {
            for (auto& value : _internalData)
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

        const type& add(const type& value) { return *_internalData.insert(value).first; }
        const type& add(type&& value) { return *_internalData.emplace(std::move(value)).first; }

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
            _internalData.merge(std::move(values));
            return *this;
        }
        jset_hash& append(const jset_hash& value) { return append(value.toBase()); }
        jset_hash& append(jset_hash&& value) { return append(std::move(value._internalData)); }

        bool remove(const type& value) noexcept { return _internalData.erase(value) > 0; }
        template<typename Pred> requires std::predicate<Pred, type>
        size_type remove(Pred pred) noexcept
        {
            size_type count = 0;
            for (iterator iter = _internalData.begin(); iter != _internalData.end(); ++iter)
            {
                if (pred(*iter))
                {
                    iter = _internalData.rase(iter);
                    ++count;
                }
            }
            return count;
        }
        void clear() noexcept { _internalData.clear(); }

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

    private:

        base_type _internalData;
    };
    
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(const jset_hash<T, Pred>& value1, const T& value2) { return value1.copy() += value2; }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(const jset_hash<T, Pred>& value1, T&& value2) { return value1.copy() += std::forward(value2); }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(jset_hash<T, Pred>&& value1, const T& value2) { return value1 += value2; }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(jset_hash<T, Pred>&& value1, T&& value2) { return value1 += std::forward(value2); }

    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(const T& value1, const jset_hash<T, Pred>& value2) { return value2.copy() += value1; }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(const T& value1, jset_hash<T, Pred>&& value2) { return value2 += value1; }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(T&& value1, const jset_hash<T, Pred>& value2) { return value2.copy() += std::forward(value1); }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(T&& value1, jset_hash<T, Pred>&& value2) { return value2 += std::forward(value1); }

    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(const jset_hash<T, Pred>& value1, const jset_hash<T, Pred>& value2) { return value1.copy() += value2; }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(const jset_hash<T, Pred>& value1, jset_hash<T, Pred>&& value2) { return value2 += value1; }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(jset_hash<T, Pred>&& value1, const jset_hash<T, Pred>& value2) { return value1 += value2; }
    template<typename T, typename Pred>
    jset_hash<T, Pred> operator+(jset_hash<T, Pred>&& value1, jset_hash<T, Pred>&& value2) { return value1 += std::forward(value2); }
}
