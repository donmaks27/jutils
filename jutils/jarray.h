// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include <vector>

#include "type_defines.h"
#include "math/jmath.h"

namespace jutils
{
    template<typename T>
    class jarray : std::vector<T>
    {
        using base_class = std::vector<T>;

    public:

        using type = T;
        using iterator = typename base_class::iterator;
        using const_iterator = typename base_class::const_iterator;

        using index_type = int32;

        jarray()
            : base_class()
        {}
        explicit jarray(const index_type size)
            : base_class(jutils::math::max(0, size))
        {}
        explicit jarray(const index_type size, const type& defaultValue)
            : base_class(size, defaultValue)
        {}
        jarray(std::initializer_list<type> list)
            : base_class(list)
        {}
        jarray(const jarray& value)
            : base_class(value)
        {}
        jarray(jarray&& value) noexcept
            : base_class(std::move(value))
        {}

        jarray& operator=(std::initializer_list<type> list)
        {
            this->base_class::operator=(list);
            return *this;
        }
        jarray& operator=(jarray&& value) noexcept
        {
            this->base_class::operator=(std::move(value));
            return *this;
        }
        jarray& operator=(const jarray& value)
        {
            this->base_class::operator=(value);
            return *this;
        }
        
        type* getData() noexcept { return this->base_class::data(); }
        const type* getData() const noexcept { return this->base_class::data(); }

        index_type getSize() const { return static_cast<index_type>(this->base_class::size()); }
        bool isEmpty() const { return this->base_class::empty(); }
        bool isValidIndex(const index_type index) const { return jutils::math::isWithin(index, 0, getSize() - 1); }
        
        iterator begin() noexcept { return this->base_class::begin(); }
        iterator end() noexcept { return this->base_class::end(); }

        const_iterator begin() const noexcept { return this->base_class::begin(); }
        const_iterator end() const noexcept { return this->base_class::end(); }

        type& get(index_type index)
        {
            checkIsValidIndex(index);
            return getInternal(index);
        }
        const type& get(index_type index) const
        {
            checkIsValidIndex(index);
            return getInternal(index);
        }
        type& operator[](const index_type index) { return get(index); }
        const type& operator[](const index_type index) const { return get(index); }

        type& getFirst()
        {
            checkIfEmpty();
            return this->base_class::front();
        }
        type& getLast()
        {
            checkIfEmpty();
            return this->base_class::back();
        }
        const type& getFirst() const
        {
            checkIfEmpty();
            return this->base_class::front();
        }
        const type& getLast() const
        {
            checkIfEmpty();
            return this->base_class::back();
        }

        type* findByIndex(const index_type index) { return isValidIndex(index) ? &getInternal(index) : nullptr; }
        type* findByValue(const type& value)
        {
            const index_type index = indexOf(value);
            return index != -1 ? &getInternal(index) : nullptr;
        }
        const type* findByIndex(const index_type index) const { return isValidIndex(index) ? &getInternal(index) : nullptr; }
        const type* findByValue(const type& value) const
        {
            const index_type index = indexOf(value);
            return index != -1 ? &getInternal(index) : nullptr;
        }

        index_type indexOf(const type& value) const
        {
            if (!isEmpty())
            {
                for (index_type index = 0; index < getSize(); index++)
                {
                    if (getInternal(index) == value)
                    {
                        return index;
                    }
                }
            }
            return -1;
        }
        bool contains(const type& value) const { return indexOf(value) != -1; }

        template<typename... Args>
        type& put(Args&&... args)
        {
            this->base_class::emplace_back(std::forward<Args>(args)...);
            return this->base_class::back();
        }
        template<typename... Args>
        type& putAt(index_type index, Args&&... args)
        {
            checkIsValidIndex(index);
            return this->base_class::emplace(getIterByIndex(index), std::forward<Args>(args)...);
        }

        type& add(const type& value) { return put(value); }
        type& add(type&& value) { return put(std::move(value)); }
        type& addDefault() { return put(); }
        type& addUnique(const type& value)
        {
            const index_type index = indexOf(value);
            return index == -1 ? add(value) : getInternal(index);
        }
        type& addUnique(type&& value)
        {
            const index_type index = indexOf(value);
            return index == -1 ? add(std::move(value)) : getInternal(index);
        }

        type& addAt(const index_type index, const type& value) { return putAt(index, value); }
        type& addAt(const index_type index, type&& value) { return putAt(index, std::move(value)); }
        type& addDefaultAt(const index_type index) { return putAt(index); }

        void reserve(const index_type size) { this->base_class::reserve(jutils::math::max(0, size)); }
        void resize(const index_type size) { this->base_class::resize(jutils::math::max(0, size)); }
        void resize(const index_type size, const type& defaultValue) { this->base_class::resize(jutils::math::max(0, size), defaultValue); }

        void removeAt(const index_type index)
        {
            if (isValidIndex(index))
            {
                removeAtInternal(index);
            }
        }
        index_type remove(const type& value)
        {
            index_type count = 0;
            for (index_type index = getSize() - 1; index >= 0; --index)
            {
                if (getInternal(index) == value)
                {
                    removeAtInternal(index);
                    count++;
                }
            }
            return count;
        }
        void clear() { return this->base_class::clear(); }

        jarray& operator+=(const type& value)
        {
            add(value);
            return *this;
        }
        jarray& operator+=(type&& value)
        {
            add(std::move(value));
            return *this;
        }
        jarray& operator+=(std::initializer_list<type> list)
        {
            for (auto& value : list)
            {
                add(value);
            }
            return *this;
        }
        jarray& operator+=(const jarray& value)
        {
            if ((this != &value) && !value.isEmpty())
            {
                for (index_type index = 0; index < value.getSize(); index++)
                {
                    add(value[index]);
                }
            }
            return *this;
        }

    private:

        void checkIsValidIndex(const index_type index) const
        {
            if (!isValidIndex(index))
            {
                throw std::out_of_range("Invalid jarray<T> index!");
            }
        }
        void checkIfEmpty() const
        {
            if (isEmpty())
            {
                throw std::length_error("jarray<T> is empty!");
            }
        }

        iterator getIterByIndex(const index_type index)
        {
            if (!isValidIndex(index))
            {
                return end();
            }
            auto iter = begin();
            for (index_type i = 0; i < index; i++)
            {
                ++iter;
            }
            return iter;
        }
        iterator getIterByValue(const type& value)
        {
            for (auto iter = begin(); iter != end(); ++iter)
            {
                if (*iter == value)
                {
                    return iter;
                }
            }
            return end();
        }
        const_iterator getIterByIndex(const index_type index) const
        {
            if (!isValidIndex(index))
            {
                return end();
            }
            auto iter = begin();
            for (index_type i = 0; i < index; i++)
            {
                ++iter;
            }
            return iter;
        }
        const_iterator getIterByValue(const type& value) const
        {
            for (auto iter = begin(); iter != end(); ++iter)
            {
                if (*iter == value)
                {
                    return iter;
                }
            }
            return end();
        }

        type& getInternal(const index_type index) { return this->base_class::operator[](index); }
        const type& getInternal(const index_type index) const { return this->base_class::operator[](index); }

        void removeAtInternal(const index_type index) { this->base_class::erase(begin() + index); }
    };
    
    template<typename T>
    jarray<T> operator+(const jarray<T>& value1, const T& value2) { return jarray<T>(value1) += value2; }
    template<typename T>
    jarray<T> operator+(const T& value1, const jarray<T>& value2) { return (jarray<T>() += value1) += value2; }
    template<typename T>
    jarray<T> operator+(const jarray<T>& value1, const jarray<T>& value2) { return jarray<T>(value1) += value2; }
}
