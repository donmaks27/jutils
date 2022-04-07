// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jtree_red_black.h"

namespace jutils
{
    template<typename T>
    class jset
    {
    public:

        using type = T;
        using container_type = jtree_red_black<type>;
        using const_iterator = typename container_type::const_iterator;
        using index_type = int32;

        jset() = default;
        jset(std::initializer_list<type> list)
            : container(list)
        {}
        jset(const jset&) = default;
        jset(jset&& value) noexcept
            : container(std::move(value.container))
        {}
        ~jset() = default;

        jset& operator=(std::initializer_list<type> list)
        {
            container = list;
            return *this;
        }
        jset& operator=(const jset&) = default;
        jset& operator=(jset&& value) noexcept
        {
            container = std::move(value.container);
            return *this;
        }

        index_type getSize() const { return container.getSize(); }
        bool isEmpty() const { return getSize() == 0; }

        const_iterator begin() const noexcept { return container.begin(); }
        const_iterator end() const noexcept { return container.end(); }

        bool contains(const type& value) const { return container.contains(value); }
        
        void reserve(const index_type capacity) { container.resizeTable(capacity); }

        const type& add(const type& value) { return container.add(value, false); }
        const type& add(type&& value) { return container.add(std::move(value), false); }

        void append(std::initializer_list<type> list) { container.append(list, false); }
        void append(const jset& value) { container.append(value.container, false); }

        bool remove(const type& value) { return container.remove(value); }
        void clear() { container.clear(); }

        jset& operator+=(const type& value)
        {
            add(value);
            return *this;
        }
        jset& operator+=(type&& value)
        {
            add(std::move(value));
            return *this;
        }
        jset& operator+=(std::initializer_list<type> list)
        {
            append(list, false);
            return *this;
        }
        jset& operator+=(const jset& value)
        {
            append(value, false);
            return *this;
        }

    private:

        container_type container = container_type();
    };

    template<typename T>
    jset<T> operator+(const jset<T>& container, const T& value) { return jset<T>(container) += value; }
    template<typename T>
    jset<T> operator+(const T& value, const jset<T>& container) { return jset<T>(1, value) += container; }
    template<typename T>
    jset<T> operator+(const jset<T>& container1, const jset<T>& container2) { return jset<T>(container1) += container2; }
}
