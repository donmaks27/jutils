// Copyright © 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"
#include "macro/template_condition.h"

#ifndef JUTILS_MODULE
    #include "math/math.h"
    #include <algorithm>
    #include <list>
#endif

JUTILS_MODULE_EXPORT namespace jutils
{
    template<typename T>
    class list
    {
    public:

        using type = T;
        using base_type = std::list<type>;
        using const_iterator = typename base_type::const_iterator;
        using iterator = typename base_type::iterator;

        list() noexcept = default;
        explicit list(const index_type count)
            : _internalData(count)
        {}
        list(const index_type count, const type& defaultValue)
            : _internalData(count, defaultValue)
        {}
        list(std::initializer_list<type> values)
            : _internalData(values)
        {}
        list(const base_type& value)
            : _internalData(value)
        {}
        list(base_type&& value) noexcept
            : _internalData(std::move(value))
        {}
        list(const list&) = default;
        list(list&&) noexcept = default;
        ~list() noexcept = default;

        list& operator=(std::initializer_list<type> values)
        {
            _internalData = values;
            return *this;
        }
        list& operator=(const base_type& value)
        {
            _internalData = value;
            return *this;
        }
        list& operator=(base_type&& value) noexcept
        {
            _internalData = std::move(value);
            return *this;
        }
        list& operator=(const list& value) = default;
        list& operator=(list&& value) noexcept = default;

        [[nodiscard]] const base_type& toBase() const noexcept { return _internalData; }

        [[nodiscard]] index_type getSize() const noexcept { return _internalData.size(); }
        [[nodiscard]] bool isEmpty() const noexcept { return _internalData.empty(); }
        [[nodiscard]] bool isValidIndex(const index_type index) const noexcept { return index < getSize(); }

        [[nodiscard]] iterator begin() noexcept { return _internalData.begin(); }
        [[nodiscard]] iterator end() noexcept { return _internalData.end(); }
        [[nodiscard]] const_iterator begin() const noexcept { return _internalData.begin(); }
        [[nodiscard]] const_iterator end() const noexcept { return _internalData.end(); }

        [[nodiscard]] list copy() const noexcept { return *this; }

        [[nodiscard]] type& get(const index_type index) noexcept { return *std::next(begin(), index); }
        [[nodiscard]] const type& get(const index_type index) const noexcept { return *std::next(begin(), index); }
        [[nodiscard]] type& operator[](const index_type index) noexcept { return get(index); }
        [[nodiscard]] const type& operator[](const index_type index) const noexcept { return get(index); }

        [[nodiscard]] type& getFirst() noexcept { return _internalData.front(); }
        [[nodiscard]] type& getLast() noexcept { return _internalData.back(); }
        [[nodiscard]] const type& getFirst() const noexcept { return _internalData.front(); }
        [[nodiscard]] const type& getLast() const noexcept { return _internalData.back(); }

        [[nodiscard]] iterator findIter(const type& value) noexcept { return std::find(begin(), end(), value); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] iterator findIter(Pred pred) noexcept { return std::find_if(begin(), end(), pred); }
        [[nodiscard]] const_iterator findIter(const type& value) const noexcept { return std::find(begin(), end(), value); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] const_iterator findIter(Pred pred) const noexcept { return std::find_if(begin(), end(), pred); }

        [[nodiscard]] index_type indexOf(const type& value) const noexcept
        {
            index_type index = 0;
            for (const auto& _value : _internalData)
            {
                if (_value == value)
                {
                    return index;
                }
                index++;
            }
            return index_invalid;
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] index_type indexOf(Pred pred) const noexcept
        {
            index_type index = 0;
            for (const auto& _value : _internalData)
            {
                if (pred(_value))
                {
                    return index;
                }
                index++;
            }
            return index_invalid;
        }

        [[nodiscard]] bool contains(const type& value) const noexcept { return findIter(value) != end(); }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] bool contains(Pred pred) const noexcept { return findIter(pred) != end(); }

        [[nodiscard]] type* find(const type& value) noexcept
        {
            const auto iter = findIter(value);
            return iter != end() ? &*iter : nullptr;
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] type* find(Pred pred) noexcept
        {
            const auto iter = findIter(pred);
            return iter != end() ? &*iter : nullptr;
        }
        [[nodiscard]] const type* find(const type& value) const noexcept
        {
            const auto iter = findIter(value);
            return iter != end() ? &*iter : nullptr;
        }
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        [[nodiscard]] const type* find(Pred pred) const noexcept
        {
            const auto iter = findIter(pred);
            return iter != end() ? &*iter : nullptr;
        }

        void resize(const index_type size) { _internalData.resize(size); }
        void resize(const index_type size, const type& defaultValue) { _internalData.resize(size, defaultValue); }

        template<typename... Args>
        type& put(Args&&... args) { return _internalData.emplace_back(std::forward<Args>(args)...); }
        template<typename... Args>
        type& putFirst(Args&&... args) { return _internalData.emplace_front(std::forward<Args>(args)...); }
        template<typename... Args>
        type& putAt(const_iterator place, Args&&... args)
        {
            if (place == begin())
            {
                return putFirst(std::forward<Args>(args)...);
            }
            return *_internalData.emplace(place, std::forward<Args>(args)...);
        }
        template<typename... Args>
        type& putAt(const index_type index, Args&&... args)
        {
            if (index == 0)
            {
                return putFirst(std::forward<Args>(args)...);
            }
            return putAt(std::next(begin(), jutils::math::min(index, getSize())), std::forward<Args>(args)...);
        }

        type& add(const type& value) { return put(value); }
        type& add(type&& value) { return put(std::move(value)); }
        type& addDefault() { return put(); }
        type& addUnique(const type& value)
        {
            type* v = find(value);
            return v == nullptr ? add(value) : *v;
        }
        type& addUnique(type&& value)
        {
            type* v = find(value);
            return v == nullptr ? add(std::move(value)) : *v;
        }

        type& addFirst(const type& value) { return putFirst(value); }
        type& addFirst(type&& value) { return putFirst(std::move(value)); }
        type& addFirstDefault() { return putFirst(); }
        type& addFirstUnique(const type& value)
        {
            type* v = find(value);
            return v == nullptr ? addFirst(value) : *v;
        }
        type& addFirstUnique(type&& value)
        {
            type* v = find(value);
            return v == nullptr ? addFirst(std::move(value)) : *v;
        }

        type& addAt(const_iterator place, const type& value) { return putAt(place, value); }
        type& addAt(const_iterator place, type&& value) { return putAt(place, std::move(value)); }
        type& addDefaultAt(const_iterator place) { return putAt(place); }

        type& addAt(const index_type index, const type& value) { return putAt(index, value); }
        type& addAt(const index_type index, type&& value) { return putAt(index, std::move(value)); }
        type& addDefaultAt(const index_type index) { return putAt(index); }

        list& append(std::initializer_list<type> values)
        {
            _internalData.insert(end(), values);
            return *this;
        }
        list& append(const base_type& value)
        {
            if (&_internalData != &value)
            {
                _internalData.insert(end(), value.begin(), value.end());
            }
            return *this;
        }
        list& append(base_type&& value)
        {
            _internalData.merge(std::move(value));
            return *this;
        }
        list& append(const list& value) { return append(value.toBase()); }
        list& append(list&& value) { return append(std::move(value._internalData)); }

        list& operator+=(const type& value)
        {
            add(value);
            return *this;
        }
        list& operator+=(type&& value)
        {
            add(std::move(value));
            return *this;
        }
        list& operator+=(std::initializer_list<type> values) { return append(values); }
        list& operator+=(const base_type& value) { return append(value); }
        list& operator+=(base_type&& value) { return append(std::move(value)); }
        list& operator+=(const list& value) { return append(value); }
        list& operator+=(list&& value) { return append(std::move(value)); }

        void removeAt(const_iterator placeStart, const_iterator placeEnd) noexcept { _internalData.erase(placeStart, placeEnd); }
        void removeAt(const_iterator place, index_type count = 1) noexcept
        {

            const auto endIter = end();
            auto placeEnd = place;
            while ((count > 0) && (placeEnd != endIter))
            {
                ++placeEnd;
                count--;
            }
            removeAt(place, placeEnd);
        }
        void removeAt(index_type index, index_type count = 1) noexcept
        {
            if (isValidIndex(index))
            {
                removeAt(std::next(begin(), index), count);
            }
        }
        void removeFirst() noexcept
        {
            if (!isEmpty())
            {
                _internalData.pop_front();
            }
        }
        void removeLast() noexcept
        {
            if (!isEmpty())
            {
                _internalData.pop_back();
            }
        }
        index_type remove(const type& value) noexcept
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
        JUTILS_TEMPLATE_CONDITION((jutils::is_predicate_v<Pred, type>), typename Pred)
        index_type remove(Pred pred) noexcept
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

        void clear() noexcept { _internalData.clear(); }

    private:

        base_type _internalData;
    };

    template<typename T>
    [[nodiscard]] list<T> operator+(list<T>&& container, const T& value)
    {
        list<T> result = std::move(container);
        result += value;
        return result;
    }
    template<typename T>
    [[nodiscard]] list<T> operator+(list<T>&& container, T&& value)
    {
        list<T> result = std::move(container);
        result += std::forward<T>(value);
        return result;
    }
    template<typename T>
    [[nodiscard]] list<T> operator+(const list<T>& container, const T& value) { return container.copy() + value; }
    template<typename T>
    [[nodiscard]] list<T> operator+(const list<T>& container, T&& value) { return container.copy() + std::forward<T>(value); }

    template<typename T>
    [[nodiscard]] list<T> operator+(const T& value, list<T>&& container)
    {
        list<T> result = std::move(container);
        result.addFirst(value);
        return result;
    }
    template<typename T>
    [[nodiscard]] list<T> operator+(T&& value, list<T>&& container)
    {
        list<T> result = std::move(container);
        result.addFirst(std::forward<T>(value));
        return result;
    }
    template<typename T>
    [[nodiscard]] list<T> operator+(const T& value, const list<T>& container) { return value + container.copy(); }
    template<typename T>
    [[nodiscard]] list<T> operator+(T&& value, const list<T>& container) { return std::forward<T>(value) + container.copy(); }

    template<typename T>
    [[nodiscard]] list<T> operator+(list<T>&& container1, std::initializer_list<T> list)
    {
        jutils::list<T> result = std::move(container1);
        result += list;
        return result;
    }
    template<typename T>
    [[nodiscard]] list<T> operator+(list<T>&& container1, const list<T>& container2)
    {
        list<T> result = std::move(container1);
        result += container2;
        return result;
    }
    template<typename T>
    [[nodiscard]] list<T> operator+(list<T>&& container1, list<T>&& container2)
    {
        list<T> result = std::move(container1);
        result += std::move(container2);
        return result;
    }
    template<typename T>
    [[nodiscard]] list<T> operator+(const list<T>& container1, std::initializer_list<T> list) { return container1.copy() + list; }
    template<typename T>
    [[nodiscard]] list<T> operator+(const list<T>& container1, const list<T>& container2) { return container1.copy() + container2; }
    template<typename T>
    [[nodiscard]] list<T> operator+(const list<T>& container1, list<T>&& container2) { return container1.copy() + std::move(container2); }
}
