// Copyright © 2022-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "math/math.h"

#include <stdexcept>
#include <vector>

namespace jutils
{
    template<typename T>
    class jarray : private std::vector<T>
    {
    public:
        
        using type = T;
        using base_type = std::vector<type>;
        using size_type = int32;

        static constexpr size_type invalidIndex = -1;
        static constexpr size_type maxSize = std::numeric_limits<size_type>::max();
        
        using const_iterator = typename base_type::const_iterator;
        using iterator = typename base_type::iterator;

        constexpr jarray() noexcept = default;
        constexpr explicit jarray(const size_type count)
            : base_type(count)
        {}
        constexpr jarray(const size_type count, const type& defaultValue)
            : base_type(count, defaultValue)
        {}
        constexpr jarray(const std::initializer_list<type> values)
            : base_type(values)
        {
            _correctSize();
        }
        constexpr jarray(const base_type& value)
            : base_type(value)
        {
            _correctSize();
        }
        constexpr jarray(base_type&& value) noexcept
            : base_type(std::move(value))
        {
            _correctSize();
        }
        constexpr jarray(const jarray&) = default;
        constexpr jarray(jarray&&) noexcept = default;
        constexpr ~jarray() noexcept = default;

        constexpr jarray& operator=(const std::initializer_list<type> values)
        {
            base_type::operator=(values);
            _correctSize();
            return *this;
        }
        constexpr jarray& operator=(const base_type& value)
        {
            base_type::operator=(value);
            _correctSize();
            return *this;
        }
        constexpr jarray& operator=(base_type&& value) noexcept
        {
            base_type::operator=(std::move(value));
            _correctSize();
            return *this;
        }
        constexpr jarray& operator=(const jarray& value) = default;
        constexpr jarray& operator=(jarray&& value) noexcept = default;

        constexpr const base_type& toBase() const noexcept { return *this; }

        constexpr size_type getSize() const noexcept { return static_cast<size_type>(jutils::math::min(base_type::size(), maxSize)); }
        constexpr bool isEmpty() const noexcept { return base_type::empty(); }
        constexpr bool isValidIndex(const size_type index) const noexcept
        {
            return (index >= 0) && (static_cast<typename base_type::size_type>(index) < base_type::size());
        }
        
        constexpr iterator begin() noexcept { return base_type::begin(); }
        constexpr iterator end() noexcept { return base_type::end(); }
        
        constexpr const_iterator begin() const noexcept { return base_type::begin(); }
        constexpr const_iterator end() const noexcept { return base_type::end(); }

        constexpr type* getData() noexcept { return base_type::data(); }
        constexpr const type* getData() const noexcept { return base_type::data(); }

        constexpr jarray copy() const { return *this; }
        
        constexpr type& get(const size_type index) noexcept { return base_type::operator[](index); }
        constexpr const type& get(const size_type index) const noexcept { return base_type::operator[](index); }
        constexpr type& operator[](const size_type index) noexcept { return get(index); }
        constexpr const type& operator[](const size_type index) const noexcept { return get(index); }

        constexpr type& getFirst() noexcept { return base_type::front(); }
        constexpr type& getLast() noexcept { return base_type::back(); }
        constexpr const type& getFirst() const noexcept { return base_type::front(); }
        constexpr const type& getLast() const noexcept { return base_type::back(); }
        
        constexpr size_type indexOf(const type& value) const noexcept
        {
            const size_type size = getSize();
            for (size_type index = 0; index < size; ++index)
            {
                if (value == get(index))
                {
                    return index;
                }
            }
            return invalidIndex;
        }
        template<typename Pred> requires std::predicate<Pred, type>
        constexpr size_type indexOf(Pred pred) const noexcept
        {
            const size_type size = getSize();
            for (size_type index = 0; index < size; ++index)
            {
                if (pred(get(index)))
                {
                    return index;
                }
            }
            return invalidIndex;
        }
        
        constexpr type* find(const type& value) noexcept
        {
            const size_type index = indexOf(value);
            return index != invalidIndex ? get(index) : nullptr;
        }
        template<typename Pred> requires std::predicate<Pred, type>
        constexpr type* find(Pred pred) noexcept
        {
            const size_type index = indexOf(pred);
            return index != invalidIndex ? get(index) : nullptr;
        }
        constexpr const type* find(const type& value) const noexcept
        {
            const size_type index = indexOf(value);
            return index != invalidIndex ? get(index) : nullptr;
        }
        template<typename Pred> requires std::predicate<Pred, type>
        constexpr const type* find(Pred pred) const noexcept
        {
            const size_type index = indexOf(pred);
            return index != invalidIndex ? get(index) : nullptr;
        }
        
        constexpr size_type contains(const type& value) const noexcept { return indexOf(value) != invalidIndex; }
        template<typename Pred> requires std::predicate<Pred, type>
        constexpr size_type contains(Pred pred) const noexcept { return indexOf(pred) != invalidIndex; }

        constexpr void reserve(const size_type capacity) { base_type::reserve(jutils::math::max(capacity, 0)); }
        constexpr void resize(const size_type size, const type& defaultValue = type()) { base_type::resize(jutils::math::max(size, 0), defaultValue); }

        template<typename... Args>
        constexpr type& putBack(Args&&... args)
        {
            _checkSize();
            return base_type::emplace_back(std::forward<Args>(args)...);
        }
        template<typename... Args>
        constexpr type& putAt(const size_type index, Args&&... args)
        {
            _checkSize();
            return base_type::emplace(std::next(base_type::begin(), jutils::math::clamp(index, 0, getSize())), std::forward<Args>(args)...);
        }

        constexpr type& add(const type& value) { return putBack(value); }
        constexpr type& add(type&& value) { return putBack(std::move(value)); }
        constexpr type& addDefault() { return putBack(); }
        constexpr type& addUnique(const type& value)
        {
            const size_type index = indexOf(value);
            return index == invalidIndex ? add(value) : get(index);
        }
        constexpr type& addUnique(type&& value)
        {
            const size_type index = indexOf(value);
            return index == invalidIndex ? add(std::move(value)) : get(index);
        }

        constexpr type& addAt(const size_type index, const type& value) { return putAt(index, value); }
        constexpr type& addAt(const size_type index, type&& value) { return putAt(index, std::move(value)); }
        constexpr type& addDefaultAt(const size_type index) { return putAt(index); }

        constexpr jarray& append(const std::initializer_list<type> values)
        {
            base_type::insert(end(), values);
            _correctSize();
            return *this;
        }
        constexpr jarray& append(const base_type& value)
        {
            if (this != &value)
            {
                base_type::insert(end(), value.begin(), value.end());
                _correctSize();
            }
            return *this;
        }
        constexpr jarray& append(const jarray& value) { return append(value.toBase()); }

        constexpr void removeAt(size_type index, size_type count = 1) noexcept
        {
            const size_type size = getSize();
            if (size > 0)
            {
                index = jutils::math::clamp(index, 0, size - 1);
                count = jutils::math::min(count, size - index);
                if (count == 1)
                {
                    base_type::erase(std::next(begin(), index));
                }
                else if (count > 1)
                {
                    base_type::erase(std::next(begin(), index), std::next(begin(), index + count));
                }
            }
        }
        constexpr void removeFirst() noexcept
        {
            if (!isEmpty())
            {
                base_type::erase(begin());
            }
        }
        constexpr void removeLast() noexcept
        {
            if (!isEmpty())
            {
                base_type::erase(end() - 1);
            }
        }
        constexpr size_type remove(const type& value) noexcept { return static_cast<size_type>(std::erase(*this, value)); }
        template<typename Pred> requires std::predicate<Pred, type>
        constexpr size_type remove(Pred pred) noexcept { return static_cast<size_type>(std::erase_if(*this, pred)); }

        constexpr void clear() noexcept { base_type::clear(); }

        constexpr jarray& operator+=(const type& value)
        {
            add(value);
            return *this;
        }
        constexpr jarray& operator+=(type&& value)
        {
            add(std::move(value));
            return *this;
        }
        constexpr jarray& operator+=(const std::initializer_list<type> values) { return append(values); }
        constexpr jarray& operator+=(const base_type& value) { return append(value); }
        constexpr jarray& operator+=(const jarray& value) { return append(value); }
        
    private:

        constexpr void _correctSize() noexcept
        {
            if (base_type::size() > static_cast<typename base_type::size_type>(maxSize))
            {
                base_type::resize(maxSize);
            }
        }
        constexpr void _checkSize() const
        {
            if (getSize() == maxSize)
            {
                throw std::length_error("jarray is too long");
            }
        }
    };
    
    template<typename T>
    jarray<T> operator+(const jarray<T>& container, const T& value) { return container.copy() += value; }
    template<typename T>
    jarray<T> operator+(const jarray<T>& container, T&& value) { return container.copy() += std::move(value); }
    template<typename T>
    jarray<T> operator+(jarray<T>&& container, const T& value) { return container += value; }
    template<typename T>
    jarray<T> operator+(jarray<T>&& container, T&& value) { return container += std::move(value); }

    template<typename T>
    jarray<T> operator+(const T& value, const jarray<T>& container) { return jarray<T>(1, value) += container; }
    
    template<typename T>
    jarray<T> operator+(const jarray<T>& container1, const std::initializer_list<T> list) { return container1.copy() += list; }
    template<typename T>
    jarray<T> operator+(jarray<T>&& container1, const std::initializer_list<T> list) { return container1 += list; }
    template<typename T>
    jarray<T> operator+(const jarray<T>& container1, const jarray<T>& container2) { return container1.copy() += container2; }
    template<typename T>
    jarray<T> operator+(jarray<T>&& container1, const jarray<T>& container2) { return container1 += container2; }
}
