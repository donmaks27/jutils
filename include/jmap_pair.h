// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "math/hash.h"

namespace jutils
{
    template<typename KeyType, typename ValueType>
    class jmap_pair
    {
    public:

        using key_type = KeyType;
        using value_type = ValueType;

        constexpr jmap_pair() = default;
        template<typename... Args>
        constexpr jmap_pair(const key_type& keyValue, Args&&... valueArgs)
            : key(keyValue), value(std::forward<Args>(valueArgs)...)
        {}
        template<typename... Args>
        constexpr jmap_pair(key_type&& keyValue, Args&&... valueArgs)
            : key(std::move(keyValue)), value(std::forward<Args>(valueArgs)...)
        {}
        constexpr jmap_pair(const jmap_pair&) = default;
        constexpr jmap_pair(jmap_pair&& pair) noexcept
            : key(std::move(pair.key)), value(std::move(pair.value))
        {}

        constexpr jmap_pair& operator=(const jmap_pair&) = default;
        constexpr jmap_pair& operator=(jmap_pair&& pair) noexcept
        {
            key = std::move(pair.key);
            value = std::move(pair.value);
            return *this;
        }

        key_type key;
        value_type value;

        uint64 hash() const { return math::hash::getHash(key); }
    };

    template<typename KeyType, typename ValueType>
    constexpr bool operator==(const jmap_pair<KeyType, ValueType>& pair, const KeyType& key) { return pair.key == key; }
    template<typename KeyType, typename ValueType>
    constexpr bool operator==(const KeyType& key, const jmap_pair<KeyType, ValueType>& pair) { return pair == key; }
    template<typename KeyType, typename ValueType>
    constexpr bool operator==(const jmap_pair<KeyType, ValueType>& pair1, const jmap_pair<KeyType, ValueType>& pair2) { return pair1.key == pair2.key; }
}
