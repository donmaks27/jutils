// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jtree_red_black.h"

namespace jutils
{
    template<typename T>
    class jset
    {
        using container_type = jtree_red_black<T>;

    public:

        using type = T;
        using iterator = typename container_type::iterator;
        using const_iterator = typename container_type::const_iterator;
        using index_type = int32;

        jset() = default;
        jset(std::initializer_list<type> list)
            : container(list)
        {}
        jset(const jset& value)
            : container(value.container)
        {}
        jset(jset&& value) noexcept
            : container(std::move(value.container))
        {}
        ~jset() = default;

        jset& operator=(std::initializer_list<type> list)
        {
            container = list;
            return *this;
        }
        jset& operator=(const jset& value)
        {
            if (this != &value)
            {
                container = value.container;
            }
            return *this;
        }
        jset& operator=(jset&& value) noexcept
        {
            container = std::move(value.container);
            return *this;
        }

        index_type getSize() const { return container.getSize(); }
        bool isEmpty() const { return getSize() == 0; }

        iterator begin() noexcept { return container.begin(); }
        iterator end() noexcept { return container.end(); }

        const_iterator begin() const noexcept { return container.begin(); }
        const_iterator end() const noexcept { return container.end(); }

        bool contains(const type& value) const { return container.contains(value); }

        template<typename... Args>
        const type& put(Args&&... args) { return add(type(std::forward<Args>(args)...)); }

        const type& add(const type& value) { return container.add(value); }
        const type& add(type&& value) { return container.add(std::move(value)); }

        void append(std::initializer_list<type> list) { append(list); }
        void append(const jset& value) { container.append(value.container); }

        void remove(const type& value) { container.remove(value); }
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
            append(list);
            return *this;
        }
        jset& operator+=(const jset& value)
        {
            append(value);
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
