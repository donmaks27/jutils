// Copyright © 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"
#include "macro_template_condition.h"
#include "math/math.h"

#include <algorithm>
#include <vector>

namespace jutils
{
    template<typename T>
    class vector
    {
    public:

        static_assert(!std::is_same_v<T, bool>, "Use vector<uint8> or bitvector instead");

        using type = T;
        using base_type = std::vector<type>;
        using const_iterator = typename base_type::const_iterator;
        using iterator = typename base_type::iterator;

        JUTILS_STD20_CONSTEXPR vector() noexcept = default;
        JUTILS_STD20_CONSTEXPR explicit vector(const index_type count)
            : _internalData(count)
        {}
        JUTILS_STD20_CONSTEXPR vector(const index_type count, const type& defaultValue)
            : _internalData(count, defaultValue)
        {}
        JUTILS_STD20_CONSTEXPR vector(std::initializer_list<type> values)
            : _internalData(values)
        {}
        JUTILS_STD20_CONSTEXPR vector(const base_type& value)
            : _internalData(value)
        {}
        JUTILS_STD20_CONSTEXPR vector(base_type&& value) noexcept
            : _internalData(std::move(value))
        {}
        JUTILS_STD20_CONSTEXPR vector(const vector&) = default;
        JUTILS_STD20_CONSTEXPR vector(vector&&) noexcept = default;
        JUTILS_STD20_CONSTEXPR ~vector() noexcept = default;

        JUTILS_STD20_CONSTEXPR vector& operator=(std::initializer_list<type> values)
        {
            _internalData = values;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR vector& operator=(const base_type& value)
        {
            _internalData = value;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR vector& operator=(base_type&& value) noexcept
        {
            _internalData = std::move(value);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR vector& operator=(const vector& value) = default;
        JUTILS_STD20_CONSTEXPR vector& operator=(vector&& value) noexcept = default;

        [[nodiscard]] JUTILS_STD20_CONSTEXPR const base_type& toBase() const noexcept { return _internalData; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR operator base_type() const noexcept { return toBase(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR index_type getSize() const noexcept { return _internalData.size(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool isEmpty() const noexcept { return _internalData.empty(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool isValidIndex(const index_type index) const noexcept { return index < getSize(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR iterator begin() noexcept { return _internalData.begin(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR iterator end() noexcept { return _internalData.end(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const_iterator begin() const noexcept { return _internalData.begin(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const_iterator end() const noexcept { return _internalData.end(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR type* getData() noexcept { return _internalData.data(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const type* getData() const noexcept { return _internalData.data(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR type* operator*() noexcept { return getData(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const type* operator*() const noexcept { return getData(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR vector copy() const noexcept { return *this; }
        
        [[nodiscard]] JUTILS_STD20_CONSTEXPR type& get(const index_type index) noexcept { return _internalData[index]; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const type& get(const index_type index) const noexcept { return _internalData[index]; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR type& operator[](const index_type index) noexcept { return get(index); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const type& operator[](const index_type index) const noexcept { return get(index); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR type& getFirst() noexcept { return _internalData.front(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR type& getLast() noexcept { return _internalData.back(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const type& getFirst() const noexcept { return _internalData.front(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const type& getLast() const noexcept { return _internalData.back(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR iterator findIter(const type& value) noexcept { return std::find(begin(), end(), value); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] JUTILS_STD20_CONSTEXPR iterator findIter(Pred pred) noexcept { return std::find_if(begin(), end(), pred); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const_iterator findIter(const type& value) const noexcept { return std::find(begin(), end(), value); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const_iterator findIter(Pred pred) const noexcept { return std::find_if(begin(), end(), pred); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR index_type indexOf(const type& value) const noexcept
        {
            const index_type index = std::distance(begin(), findIter(value));
            return index < getSize() ? index : index_invalid;
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] JUTILS_STD20_CONSTEXPR index_type indexOf(Pred pred) const noexcept
        {
            const index_type index = std::distance(begin(), findIter(pred));
            return index < getSize() ? index : index_invalid;
        }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool contains(const type& value) const noexcept { return findIter(value) != end(); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool contains(Pred pred) const noexcept { return findIter(pred) != end(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR type* find(const type& value) noexcept
        {
            const auto iter = findIter(value);
            return iter != end() ? &*iter : nullptr;
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] JUTILS_STD20_CONSTEXPR type* find(Pred pred) noexcept
        {
            const auto iter = findIter(pred);
            return iter != end() ? &*iter : nullptr;
        }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const type* find(const type& value) const noexcept
        {
            const auto iter = findIter(value);
            return iter != end() ? &*iter : nullptr;
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const type* find(Pred pred) const noexcept
        {
            const auto iter = findIter(pred);
            return iter != end() ? &*iter : nullptr;
        }

        JUTILS_STD20_CONSTEXPR void reserve(const index_type capacity) { _internalData.reserve(capacity); }
        JUTILS_STD20_CONSTEXPR void resize(const index_type size) { _internalData.resize(size); }
        JUTILS_STD20_CONSTEXPR void resize(const index_type size, const type& defaultValue) { _internalData.resize(size, defaultValue); }

        template<typename... Args>
        JUTILS_STD20_CONSTEXPR type& put(Args&&... args) { return _internalData.emplace_back(std::forward<Args>(args)...); }
        template<typename... Args>
        JUTILS_STD20_CONSTEXPR type& putAt(const_iterator place, Args&&... args)
        {
            return *_internalData.emplace(place, std::forward<Args>(args)...);
        }
        template<typename... Args>
        JUTILS_STD20_CONSTEXPR type& putAt(const index_type index, Args&&... args)
        {
            return putAt(std::next(begin(), jutils::math::min(index, getSize())), std::forward<Args>(args)...);
        }

        JUTILS_STD20_CONSTEXPR type& add(const type& value) { return put(value); }
        JUTILS_STD20_CONSTEXPR type& add(type&& value) { return put(std::move(value)); }
        JUTILS_STD20_CONSTEXPR type& addDefault() { return put(); }
        JUTILS_STD20_CONSTEXPR type& addUnique(const type& value)
        {
            type* v = find(value);
            return v == nullptr ? add(value) : *v;
        }
        JUTILS_STD20_CONSTEXPR type& addUnique(type&& value)
        {
            type* v = find(value);
            return v == nullptr ? add(std::move(value)) : *v;
        }

        JUTILS_STD20_CONSTEXPR type& addAt(const const_iterator place, const type& value) { return putAt(place, value); }
        JUTILS_STD20_CONSTEXPR type& addAt(const const_iterator place, type&& value) { return putAt(place, std::move(value)); }
        JUTILS_STD20_CONSTEXPR type& addDefaultAt(const const_iterator place) { return putAt(place); }

        JUTILS_STD20_CONSTEXPR type& addAt(const index_type index, const type& value) { return putAt(index, value); }
        JUTILS_STD20_CONSTEXPR type& addAt(const index_type index, type&& value) { return putAt(index, std::move(value)); }
        JUTILS_STD20_CONSTEXPR type& addDefaultAt(const index_type index) { return putAt(index); }

        JUTILS_STD20_CONSTEXPR vector& append(std::initializer_list<type> values)
        {
            _internalData.insert(end(), values);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR vector& append(const base_type& value)
        {
            if (&_internalData != &value)
            {
                _internalData.insert(end(), value.begin(), value.end());
            }
            return *this;
        }
        JUTILS_STD20_CONSTEXPR vector& append(const vector& value) { return append(value.toBase()); }

        JUTILS_STD20_CONSTEXPR vector& operator+=(const type& value)
        {
            add(value);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR vector& operator+=(type&& value)
        {
            add(std::move(value));
            return *this;
        }
        JUTILS_STD20_CONSTEXPR vector& operator+=(std::initializer_list<type> values) { return append(values); }
        JUTILS_STD20_CONSTEXPR vector& operator+=(const base_type& value) { return append(value); }
        JUTILS_STD20_CONSTEXPR vector& operator+=(const vector& value) { return append(value); }

        JUTILS_STD20_CONSTEXPR void removeAt(const const_iterator& placeStart, const const_iterator& placeEnd) noexcept
            { _internalData.erase(placeStart, placeEnd); }
        JUTILS_STD20_CONSTEXPR void removeAt(const const_iterator& place, index_type count = 1) noexcept
        {
            removeAt(place, std::next(place, static_cast<typename const_iterator::difference_type>(
                jutils::math::clamp(count, 0, getSize() - std::distance<const_iterator>(begin(), place))
            )));
        }
        JUTILS_STD20_CONSTEXPR void removeAt(index_type index, index_type count = 1) noexcept
        {
            if (isValidIndex(index))
            {
                removeAt(std::next(begin(), static_cast<typename const_iterator::difference_type>(index)), count);
            }
        }
        JUTILS_STD20_CONSTEXPR void removeFirst() noexcept
        {
            if (!isEmpty())
            {
                _internalData.erase(begin());
            }
        }
        JUTILS_STD20_CONSTEXPR void removeLast() noexcept
        {
            if (!isEmpty())
            {
                _internalData.pop_back();
            }
        }
        JUTILS_STD20_CONSTEXPR index_type remove(const type& value) noexcept
        {
#if JUTILS_STD_VERSION >= JUTILS_STD20
            return std::erase(_internalData, value);
#else
            const auto iter = std::remove(begin(), end(), value);
            const index_type deletedElementsCount = std::distance(iter, end());
            _internalData.erase(iter, end());
            return deletedElementsCount;
#endif
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        JUTILS_STD20_CONSTEXPR index_type remove(Pred pred) noexcept
        {
#if JUTILS_STD_VERSION >= JUTILS_STD20
            return std::erase_if(_internalData, pred);
#else
            const auto iter = std::remove_if(begin(), end(), pred);
            const index_type deletedElementsCount = std::distance(iter, end());
            _internalData.erase(iter, end());
            return deletedElementsCount;
#endif
        }

        JUTILS_STD20_CONSTEXPR void clear() noexcept { _internalData.clear(); }
        
    private:

        base_type _internalData;
    };

    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR vector<T> operator+(const vector<T>& container, const T& value) { return container.copy() += value; }
    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR vector<T> operator+(const vector<T>& container, T&& value) { return container.copy() += std::forward<T>(value); }
    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR vector<T> operator+(vector<T>&& container, const T& value) { return std::move(vector<T>(std::move(container)) += value); }
    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR vector<T> operator+(vector<T>&& container, T&& value) { return std::move(vector<T>(std::move(container)) += std::forward<T>(value)); }

    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR vector<T> operator+(const T& value, const vector<T>& container)
    {
        vector<T> result;
        result.reserve(container.getSize() + 1);
        result.add(value);
        return result += container;
    }
    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR vector<T> operator+(T&& value, const vector<T>& container)
    {
        vector<T> result;
        result.reserve(container.getSize() + 1);
        result.add(std::forward<T>(value));
        return result += container;
    }
    
    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR vector<T> operator+(const vector<T>& container1, std::initializer_list<T> list) { return container1.copy() += list; }
    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR vector<T> operator+(vector<T>&& container1, std::initializer_list<T> list) { return std::move(vector<T>(std::move(container1)) += list); }
    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR vector<T> operator+(const vector<T>& container1, const vector<T>& container2) { return container1.copy() += container2; }
    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR vector<T> operator+(vector<T>&& container1, const vector<T>& container2) { return std::move(vector<T>(std::move(container1)) += container2); }
}
