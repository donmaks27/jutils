// Copyright © 2025 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#include "math/math.h"
#include <algorithm>
#include <vector>

namespace jutils
{
    class bitvector
    {
    public:

        using base_type = std::vector<bool>;
        using const_iterator = base_type::const_iterator;
        using iterator = base_type::iterator;

        using reference = base_type::reference;
        using const_reference = base_type::const_reference;
        using pointer = base_type::pointer;
        using const_pointer = base_type::const_pointer;

        JUTILS_STD20_CONSTEXPR bitvector() noexcept = default;
        JUTILS_STD20_CONSTEXPR explicit bitvector(const index_type count, const bool defaultValue = false)
            : _internalData(count, defaultValue)
        {}
        JUTILS_STD20_CONSTEXPR bitvector(std::initializer_list<bool> values)
            : _internalData(values)
        {}
        JUTILS_STD20_CONSTEXPR bitvector(const base_type& value)
            : _internalData(value)
        {}
        JUTILS_STD20_CONSTEXPR bitvector(base_type&& value) noexcept
            : _internalData(std::move(value))
        {}
        JUTILS_STD20_CONSTEXPR bitvector(const bitvector&) = default;
        JUTILS_STD20_CONSTEXPR bitvector(bitvector&&) noexcept = default;
        JUTILS_STD20_CONSTEXPR ~bitvector() noexcept = default;

        JUTILS_STD20_CONSTEXPR bitvector& operator=(std::initializer_list<bool> values)
        {
            _internalData = values;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR bitvector& operator=(const base_type& value)
        {
            _internalData = value;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR bitvector& operator=(base_type&& value) noexcept
        {
            _internalData = std::move(value);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR bitvector& operator=(const bitvector& value) = default;
        JUTILS_STD20_CONSTEXPR bitvector& operator=(bitvector&& value) noexcept = default;

        [[nodiscard]] JUTILS_STD20_CONSTEXPR const base_type& toBase() const noexcept { return _internalData; }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR index_type getSize() const noexcept { return _internalData.size(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool isEmpty() const noexcept { return _internalData.empty(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool isValidIndex(const index_type index) const noexcept { return index < getSize(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR iterator begin() noexcept { return _internalData.begin(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR iterator end() noexcept { return _internalData.end(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const_iterator begin() const noexcept { return _internalData.begin(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const_iterator end() const noexcept { return _internalData.end(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR bitvector copy() const noexcept { return *this; }
        
        [[nodiscard]] JUTILS_STD20_CONSTEXPR reference get(const index_type index) noexcept { return _internalData[index]; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR reference operator[](const index_type index) noexcept { return get(index); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const_reference get(const index_type index) const noexcept { return _internalData[index]; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const_reference operator[](const index_type index) const noexcept { return get(index); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR reference getFirst() noexcept { return _internalData.front(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR reference getLast() noexcept { return _internalData.back(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const_reference getFirst() const noexcept { return _internalData.front(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const_reference getLast() const noexcept { return _internalData.back(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR iterator findIter(const bool value) noexcept { return std::find(begin(), end(), value); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const_iterator findIter(const bool value) const noexcept { return std::find(begin(), end(), value); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR index_type indexOf(const bool value) const noexcept
        {
            const index_type index = std::distance(begin(), findIter(value));
            return index < getSize() ? index : index_invalid;
        }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool contains(const bool value) const noexcept { return findIter(value) != end(); }

        JUTILS_STD20_CONSTEXPR void reserve(const index_type capacity) { _internalData.reserve(capacity); }
        JUTILS_STD20_CONSTEXPR void resize(const index_type size) { _internalData.resize(size); }
        JUTILS_STD20_CONSTEXPR void resize(const index_type size, const bool defaultValue) { _internalData.resize(size, defaultValue); }

        JUTILS_STD20_CONSTEXPR void add(const bool value = false) { _internalData.push_back(value); }
        JUTILS_STD20_CONSTEXPR void addAt(const const_iterator& place, const bool value = false) { _internalData.emplace(place, value); }
        JUTILS_STD20_CONSTEXPR void addAt(const index_type index, const bool value = false)
            { return addAt(std::next(begin(), jutils::math::min(index, getSize())), value); }
        
        JUTILS_STD20_CONSTEXPR bitvector& append(std::initializer_list<bool> values)
        {
            _internalData.insert(end(), values);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR bitvector& append(const base_type& value)
        {
            if (&_internalData != &value)
            {
                _internalData.insert(end(), value.begin(), value.end());
            }
            return *this;
        }
        JUTILS_STD20_CONSTEXPR bitvector& append(const bitvector& value) { return append(value.toBase()); }

        JUTILS_STD20_CONSTEXPR bitvector& operator+=(const bool value)
        {
            add(value);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR bitvector& operator+=(std::initializer_list<bool> values) { return append(values); }
        JUTILS_STD20_CONSTEXPR bitvector& operator+=(const base_type& value) { return append(value); }
        JUTILS_STD20_CONSTEXPR bitvector& operator+=(const bitvector& value) { return append(value); }
        
        JUTILS_STD20_CONSTEXPR void removeAt(const const_iterator& placeStart, const const_iterator& placeEnd) noexcept
            { _internalData.erase(placeStart, placeEnd); }
        JUTILS_STD20_CONSTEXPR void removeAt(const const_iterator& place, const index_type count = 1) noexcept
        {
            removeAt(place, std::next(place, static_cast<const_iterator::difference_type>(
                jutils::math::clamp(count, 0, getSize() - std::distance<const_iterator>(begin(), place))
            )));
        }
        JUTILS_STD20_CONSTEXPR void removeAt(const index_type index, index_type count = 1) noexcept
        {
            if (isValidIndex(index))
            {
                removeAt(std::next(begin(), static_cast<const_iterator::difference_type>(index)), count);
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
        JUTILS_STD20_CONSTEXPR index_type remove(const bool value) noexcept
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

        JUTILS_STD20_CONSTEXPR void clear() noexcept { _internalData.clear(); }
        
    private:

        base_type _internalData;
    };
    
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bitvector operator+(const bitvector& container, const bool value) { return container.copy() += value; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bitvector operator+(bitvector&& container, const bool value) { return bitvector(std::move(container)) += value; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bitvector operator+(const bool value, const bitvector& container)
    {
        bitvector result;
        result.reserve(container.getSize() + 1);
        result.add(value);
        return result += container;
    }
    
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bitvector operator+(const bitvector& container1, std::initializer_list<bool> list) { return container1.copy() += list; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bitvector operator+(bitvector&& container1, std::initializer_list<bool> list) { return bitvector(std::move(container1)) += list; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bitvector operator+(const bitvector& container1, const bitvector& container2) { return container1.copy() += container2; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bitvector operator+(bitvector&& container1, const bitvector& container2) { return bitvector(std::move(container1)) += container2; }
}
