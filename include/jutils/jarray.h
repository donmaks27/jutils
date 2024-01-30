// Copyright © 2022-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "math/math.h"

#include <vector>

namespace jutils
{
    template<typename T>
    class jarray
    {
    public:
        
        using type = T;
        using base_type = std::vector<type>;
        using const_iterator = typename base_type::const_iterator;
        using iterator = typename base_type::iterator;

        JUTILS_STD20_CONSTEXPR jarray() noexcept = default;
        JUTILS_STD20_CONSTEXPR explicit jarray(const index_type count)
            : _internalData(count)
        {}
        JUTILS_STD20_CONSTEXPR jarray(const index_type count, const type& defaultValue)
            : _internalData(count, defaultValue)
        {}
        JUTILS_STD20_CONSTEXPR jarray(const std::initializer_list<type> values)
            : _internalData(values)
        {}
        JUTILS_STD20_CONSTEXPR jarray(const base_type& value)
            : _internalData(value)
        {}
        JUTILS_STD20_CONSTEXPR jarray(base_type&& value) noexcept
            : _internalData(std::move(value))
        {}
        JUTILS_STD20_CONSTEXPR jarray(const jarray&) = default;
        JUTILS_STD20_CONSTEXPR jarray(jarray&&) noexcept = default;
        JUTILS_STD20_CONSTEXPR ~jarray() noexcept = default;

        JUTILS_STD20_CONSTEXPR jarray& operator=(const std::initializer_list<type> values)
        {
            _internalData = values;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jarray& operator=(const base_type& value)
        {
            _internalData = value;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jarray& operator=(base_type&& value) noexcept
        {
            _internalData = std::move(value);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jarray& operator=(const jarray& value) = default;
        JUTILS_STD20_CONSTEXPR jarray& operator=(jarray&& value) noexcept = default;

        [[nodiscard]] JUTILS_STD20_CONSTEXPR const base_type& toBase() const noexcept { return _internalData; }

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

        [[nodiscard]] JUTILS_STD20_CONSTEXPR jarray copy() const { return *this; }
        
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

        [[nodiscard]] JUTILS_STD20_CONSTEXPR index_type indexOf(const type& value) const noexcept;
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] JUTILS_STD20_CONSTEXPR index_type indexOf(Pred pred) const noexcept;

        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool contains(const type& value) const noexcept { return indexOf(value) != index_invalid; }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool contains(Pred pred) const noexcept { return indexOf(pred) != index_invalid; }

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
        JUTILS_STD20_CONSTEXPR void resize(const index_type size, const type& defaultValue = type()) { _internalData.resize(size, defaultValue); }

        template<typename... Args>
        JUTILS_STD20_CONSTEXPR type& put(Args&&... args) { return _internalData.emplace_back(std::forward<Args>(args)...); }
        template<typename... Args>
        JUTILS_STD20_CONSTEXPR type& putAt(const const_iterator place, Args&&... args)
        {
            return _internalData.emplace(place, std::forward<Args>(args)...);
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
            const index_type index = indexOf(value);
            return index == index_invalid ? add(value) : get(index);
        }
        JUTILS_STD20_CONSTEXPR type& addUnique(type&& value)
        {
            const index_type index = indexOf(value);
            return index == index_invalid ? add(std::move(value)) : get(index);
        }

        JUTILS_STD20_CONSTEXPR type& addAt(const const_iterator place, const type& value) { return putAt(place, value); }
        JUTILS_STD20_CONSTEXPR type& addAt(const const_iterator place, type&& value) { return putAt(place, std::move(value)); }
        JUTILS_STD20_CONSTEXPR type& addDefaultAt(const const_iterator place) { return putAt(place); }

        JUTILS_STD20_CONSTEXPR type& addAt(const index_type index, const type& value) { return putAt(index, value); }
        JUTILS_STD20_CONSTEXPR type& addAt(const index_type index, type&& value) { return putAt(index, std::move(value)); }
        JUTILS_STD20_CONSTEXPR type& addDefaultAt(const index_type index) { return putAt(index); }

        JUTILS_STD20_CONSTEXPR jarray& append(const std::initializer_list<type> values)
        {
            _internalData.insert(end(), values);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jarray& append(const base_type& value)
        {
            if (this != &value)
            {
                _internalData.insert(end(), value.begin(), value.end());
            }
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jarray& append(const jarray& value) { return append(value.toBase()); }

        JUTILS_STD20_CONSTEXPR jarray& operator+=(const type& value)
        {
            add(value);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jarray& operator+=(type&& value)
        {
            add(std::move(value));
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jarray& operator+=(const std::initializer_list<type> values) { return append(values); }
        JUTILS_STD20_CONSTEXPR jarray& operator+=(const base_type& value) { return append(value); }
        JUTILS_STD20_CONSTEXPR jarray& operator+=(const jarray& value) { return append(value); }

        JUTILS_STD20_CONSTEXPR void removeAt(const_iterator placeStart, const_iterator placeEnd) noexcept { _internalData.erase(placeStart, placeEnd); }
        JUTILS_STD20_CONSTEXPR void removeAt(const_iterator place, index_type count = 1) noexcept;
        JUTILS_STD20_CONSTEXPR void removeAt(index_type index, index_type count = 1) noexcept;
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
                _internalData.erase(--end());
            }
        }
        JUTILS_STD20_CONSTEXPR index_type remove(const type& value) noexcept;
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        JUTILS_STD20_CONSTEXPR index_type remove(Pred pred) noexcept;

        JUTILS_STD20_CONSTEXPR void clear() noexcept { _internalData.clear(); }
        
    private:

        base_type _internalData;
    };

    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR jarray<T> operator+(const jarray<T>& container, const T& value) { return container.copy() += value; }
    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR jarray<T> operator+(const jarray<T>& container, T&& value) { return container.copy() += std::forward(value); }
    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR jarray<T> operator+(jarray<T>&& container, const T& value) { return container += value; }
    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR jarray<T> operator+(jarray<T>&& container, T&& value) { return container += std::forward(value); }

    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR jarray<T> operator+(const T& value, const jarray<T>& container) { return jarray<T>(1, value) += container; }
    
    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR jarray<T> operator+(const jarray<T>& container1, const std::initializer_list<T> list) { return container1.copy() += list; }
    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR jarray<T> operator+(jarray<T>&& container1, const std::initializer_list<T> list) { return container1 += list; }
    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR jarray<T> operator+(const jarray<T>& container1, const jarray<T>& container2) { return container1.copy() += container2; }
    template<typename T>
    [[nodiscard]] JUTILS_STD20_CONSTEXPR jarray<T> operator+(jarray<T>&& container1, const jarray<T>& container2) { return container1 += container2; }

    template<typename T>
    JUTILS_STD20_CONSTEXPR index_type jarray<T>::indexOf(const type& value) const noexcept
    {
        const index_type size = getSize();
        for (index_type index = 0; index < size; ++index)
        {
            if (value == get(index))
            {
                return index;
            }
        }
        return index_invalid;
    }
    template<typename T>
    JUTILS_TEMPLATE_CONDITION_IMPL((jutils::is_predicate_v<Pred, T>), typename Pred)
    JUTILS_STD20_CONSTEXPR index_type jarray<T>::indexOf(Pred pred) const noexcept
    {
        const index_type size = getSize();
        for (index_type index = 0; index < size; ++index)
        {
            if (pred(get(index)))
            {
                return index;
            }
        }
        return index_invalid;
    }

    template<typename T>
    JUTILS_STD20_CONSTEXPR void jarray<T>::removeAt(const_iterator place, index_type count) noexcept
    {
        const auto endIter = end();
        auto placeEnd = place;
        while ((count > 0) || (placeEnd != endIter))
        {
            ++placeEnd;
            count--;
        }
        removeAt(place, placeEnd);
    }
    template<typename T>
    JUTILS_STD20_CONSTEXPR void jarray<T>::removeAt(const index_type index, const index_type count) noexcept
    {
        if (isValidIndex(index))
        {
            index_type elementsForDelete = jutils::math::min(count, getSize() - index);
            if (elementsForDelete == 1)
            {
                _internalData.erase(std::next(begin(), index));
            }
            else if (elementsForDelete > 1)
            {
                const auto startIter = std::next(begin(), index);
                _internalData.erase(startIter, std::next(startIter, count));
            }
        }
    }

    template<typename T>
    JUTILS_STD20_CONSTEXPR index_type jarray<T>::remove(const type& value) noexcept
    {
#if JUTILS_STD_VERSION >= JUTILS_STD20
        return std::erase(_internalData, value);
#else
        const auto iter = std::remove(begin(), end(), value);
        const index_type deletedElementsCount = end() - iter;
        _internalData.erase(iter, end());
        return deletedElementsCount;
#endif
    }
    template<typename T>
    JUTILS_TEMPLATE_CONDITION_IMPL((jutils::is_predicate_v<Pred, T>), typename Pred)
    JUTILS_STD20_CONSTEXPR index_type jarray<T>::remove(Pred pred) noexcept
    {
#if JUTILS_STD_VERSION >= JUTILS_STD20
        return std::erase_if(_internalData, pred);
#else
        const auto iter = std::remove_if(begin(), end(), pred);
        const index_type deletedElementsCount = end() - iter;
        _internalData.erase(iter, end());
        return deletedElementsCount;
#endif
    }
}
