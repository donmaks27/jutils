// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jhash_table.h"

namespace jutils
{
    template<typename T>
    class jset_hash
    {
    public:

        using type = T;
        using container_type = jhash_table<type>;
        using const_iterator = typename container_type::const_iterator;
        using index_type = int32;

        jset_hash() = default;
        jset_hash(std::initializer_list<type> list)
            : container(list)
        {}
        jset_hash(const jset_hash&) = default;
        jset_hash(jset_hash&& value) noexcept
            : container(std::move(value.container))
        {}
        ~jset_hash() = default;

        jset_hash& operator=(std::initializer_list<type> list)
        {
            container = list;
            return *this;
        }
        jset_hash& operator=(const jset_hash&) = default;
        jset_hash& operator=(jset_hash&& value) noexcept
        {
            container = std::move(value.container);
            return *this;
        }

        index_type getSize() const { return container.getSize(); }
        bool isEmpty() const { return getSize() == 0; }

        const_iterator begin() const noexcept { return container.begin(); }
        const_iterator end() const noexcept { return container.end(); }

        bool contains(const type& value) const { return container.contains(value); }
        
        void reserve(const index_type capacity) { container.reserve(capacity); }

        const type& add(const type& value) { return container.add(value); }
        const type& add(type&& value) { return container.add(std::move(value)); }

        void append(std::initializer_list<type> list) { container.append(list); }
        void append(const jset_hash& value) { container.append(value.container); }

        void remove(const type& value) { container.remove(value); }
        void clear() { container.clear(); }

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
        jset_hash& operator+=(std::initializer_list<type> list)
        {
            append(list);
            return *this;
        }
        jset_hash& operator+=(const jset_hash& value)
        {
            append(value);
            return *this;
        }

    private:

        container_type container = container_type();
    };

    template<typename T>
    jset_hash<T> operator+(const jset_hash<T>& container, const T& value) { return jset_hash<T>(container) += value; }
    template<typename T>
    jset_hash<T> operator+(const T& value, const jset_hash<T>& container) { return jset_hash<T>(1, value) += container; }
    template<typename T>
    jset_hash<T> operator+(const jset_hash<T>& container1, const jset_hash<T>& container2) { return jset<T>(container1) += container2; }
}
