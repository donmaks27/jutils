// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include <vector>

#include "type_defines.h"
#include "math/jmath.h"

namespace jutils
{
    template<typename T, typename Allocator = std::allocator<T>>
    class jarray : std::vector<T, Allocator>
    {
        using base_class = std::vector<T, Allocator>;

    public:

        using type = T;
        using allocator = Allocator;
        using iterator = typename base_class::iterator;
        using const_iterator = typename base_class::const_iterator;

        jarray()
            : base_class()
        {}
        explicit jarray(const allocator& alloc)
            : base_class(alloc)
        {}
        explicit jarray(const int32 size, const allocator& alloc = allocator())
            : base_class(jutils::math::max(0, size), alloc)
        {}
        explicit jarray(const int32 size, const type& defaultValue, const allocator& alloc = allocator())
            : base_class(size, defaultValue, alloc)
        {}
        jarray(std::initializer_list<type> list, const allocator& alloc = allocator())
            : base_class(list, alloc)
        {}
        jarray(const jarray& value)
            : base_class(value)
        {}
        jarray(const jarray& value, const allocator& alloc)
            : base_class(value, alloc)
        {}
        jarray(jarray&& value) noexcept
            : base_class(std::move(value))
        {}
        jarray(jarray&& value, const allocator& alloc)
            : base_class(std::move(value), alloc)
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

        int32 getSize() const { return static_cast<int32>(this->base_class::size()); }
        bool isEmpty() const { return this->base_class::empty(); }
        bool isValidIndex(const int32 index) const { return jutils::math::isWithin(index, 0, getSize() - 1); }
        
        iterator begin() noexcept { return this->base_class::begin(); }
        iterator end() noexcept { return this->base_class::end(); }

        const_iterator begin() const noexcept { return this->base_class::begin(); }
        const_iterator end() const noexcept { return this->base_class::end(); }

        type& get(int32 index)
        {
            checkIsValidIndex(index);
            return getInternal(index);
        }
        const type& get(int32 index) const
        {
            checkIsValidIndex(index);
            return getInternal(index);
        }
        type& operator[](const int32 index) { return get(index); }
        const type& operator[](const int32 index) const { return get(index); }

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

        type* findByIndex(const int32 index) { return isValidIndex(index) ? &getInternal(index) : nullptr; }
        type* findByValue(const type& value)
        {
            const int32 index = indexOf(value);
            return index != -1 ? &getInternal(index) : nullptr;
        }
        const type* findByIndex(const int32 index) const { return isValidIndex(index) ? &getInternal(index) : nullptr; }
        const type* findByValue(const type& value) const
        {
            const int32 index = indexOf(value);
            return index != -1 ? &getInternal(index) : nullptr;
        }

        int32 indexOf(const type& value) const;
        bool contains(const type& value) const { return indexOf(value) != -1; }

        template<typename... Args>
        type& put(Args&&... args)
        {
            this->base_class::emplace_back(std::forward<Args>(args)...);
            return this->base_class::back();
        }
        template<typename... Args>
        type& putAt(int32 index, Args&&... args)
        {
            checkIsValidIndex(index);
            return this->base_class::emplace(getIterByIndex(index), std::forward<Args>(args)...);
        }

        type& add(const type& value) { return put(value); }
        type& add(type&& value) { return put(std::move(value)); }
        type& addDefault() { return put(); }
        type& addUnique(const type& value)
        {
            const int32 index = indexOf(value);
            return index == -1 ? add(value) : getInternal(index);
        }
        type& addUnique(type&& value)
        {
            const int32 index = indexOf(value);
            return index == -1 ? add(std::move(value)) : getInternal(index);
        }

        type& addAt(const int32 index, const type& value) { return putAt(index, value); }
        type& addAt(const int32 index, type&& value) { return putAt(index, std::move(value)); }
        type& addDefaultAt(const int32 index) { return putAt(index); }

        void reserve(const int32 size) { this->base_class::reserve(jutils::math::max(0, size)); }
        void resize(const int32 size) { this->base_class::resize(jutils::math::max(0, size)); }
        void resize(const int32 size, const type& defaultValue) { this->base_class::resize(jutils::math::max(0, size), defaultValue); }

        void removeAt(const int32 index)
        {
            if (isValidIndex(index))
            {
                removeAtInternal(index);
            }
        }
        int32 remove(const type& value);
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
        template<typename OtherAllocator>
        jarray& operator+=(const jarray<type, OtherAllocator>& value)
        {
            if ((this != &value) && !value.isEmpty())
            {
                for (int32 index = 0; index < value.getSize(); index++)
                {
                    add(value[index]);
                }
            }
            return *this;
        }

        template<typename OtherAllocator>
        bool operator==(const jarray<type, OtherAllocator>& value) const;
        template<typename OtherAllocator>
        bool operator!=(const jarray<type, OtherAllocator>& value) const { return !this->operator==(value); }

    private:

        void checkIsValidIndex(const int32 index) const
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

        iterator getIterByIndex(int32 index);
        iterator getIterByValue(const type& value);
        const_iterator getIterByIndex(int32 index) const;
        const_iterator getIterByValue(const type& value) const;

        type& getInternal(const int32 index) { return this->base_class::operator[](index); }
        const type& getInternal(const int32 index) const { return this->base_class::operator[](index); }

        void removeAtInternal(const int32 index) { this->base_class::erase(begin() + index); }
    };
    
    template<typename T, typename Allocator>
    jarray<T, Allocator> operator+(const jarray<T, Allocator>& value1, const T& value2) { return jarray<T, Allocator>(value1) += value2; }
    template<typename T, typename Allocator, typename OtherAllocator>
    jarray<T, Allocator> operator+(const jarray<T, Allocator>& value1, const jarray<T, OtherAllocator>& value2) { return jarray<T, Allocator>(value1) += value2; }



    template<typename T, typename Allocator>
    typename jarray<T, Allocator>::iterator jarray<T, Allocator>::getIterByIndex(const int32 index)
    {
        if (!isValidIndex(index))
        {
            return end();
        }
        auto iter = begin();
        for (int32 i = 0; i < index; i++)
        {
            ++iter;
        }
        return iter;
    }
    template<typename T, typename Allocator>
    typename jarray<T, Allocator>::iterator jarray<T, Allocator>::getIterByValue(const type& value)
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

    template<typename T, typename Allocator>
    typename jarray<T, Allocator>::const_iterator jarray<T, Allocator>::getIterByIndex(const int32 index) const
    {
        if (!isValidIndex(index))
        {
            return end();
        }
        auto iter = begin();
        for (int32 i = 0; i < index; i++)
        {
            ++iter;
        }
        return iter;
    }
    template<typename T, typename Allocator>
    typename jarray<T, Allocator>::const_iterator jarray<T, Allocator>::getIterByValue(const type& value) const
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

    template<typename T, typename Allocator>
    int32 jarray<T, Allocator>::indexOf(const type& value) const
    {
        if (!isEmpty())
        {
            for (int32 index = 0; index < getSize(); index++)
            {
                if (getInternal(index) == value)
                {
                    return index;
                }
            }
        }
        return -1;
    }

    template<typename T, typename Allocator>
    int32 jarray<T, Allocator>::remove(const type& value)
    {
        int32 count = 0;
        for (int32 index = getSize() - 1; index >= 0; --index)
        {
            if (getInternal(index) == value)
            {
                removeAtInternal(index);
                count++;
            }
        }
        return count;
    }

    template<typename T, typename Allocator>
    template<typename OtherAllocator>
    bool jarray<T, Allocator>::operator==(const jarray<type, OtherAllocator>& value) const
    {
        const int32 size = getSize();
        if (size != value.getSize())
        {
            return false;
        }
        if (size == 0)
        {
            return true;
        }
        for (int32 index = 0; index < size; index++)
        {
            if (getInternal(index) != value[index])
            {
                return false;
            }
        }
        return true;
    }
}
