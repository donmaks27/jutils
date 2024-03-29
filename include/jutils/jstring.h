﻿// Copyright © 2021-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types.h"
#include "math/hash.h"
#include "jarray.h"

#include <string>
#include <regex>

namespace jutils
{
    class jstring
    {
    public:

        using base_type = std::string;
        using char_type = base_type::value_type;
        using const_iterator = base_type::const_iterator;
        using iterator = base_type::iterator;

        JUTILS_STD20_CONSTEXPR jstring() noexcept = default;
        JUTILS_STD20_CONSTEXPR jstring(const char_type character)
            : _internalString(1, character)
        {}
        JUTILS_STD20_CONSTEXPR jstring(const char_type* str)
            : _internalString(str)
        {}
        JUTILS_STD20_CONSTEXPR jstring(const char_type* const str, const index_type count)
            : _internalString(str, count)
        {}
        JUTILS_STD20_CONSTEXPR jstring(const index_type count, const char_type character)
            : _internalString(count, character)
        {}
        JUTILS_STD20_CONSTEXPR jstring(const base_type& str)
            : _internalString(str)
        {}
        JUTILS_STD20_CONSTEXPR jstring(base_type&& str) noexcept
            : _internalString(std::move(str))
        {}
        JUTILS_STD20_CONSTEXPR jstring(const jstring& str) = default;
        JUTILS_STD20_CONSTEXPR jstring(jstring&& str) noexcept = default;
        JUTILS_STD20_CONSTEXPR ~jstring() = default;

        JUTILS_STD20_CONSTEXPR jstring& operator=(const char_type character)
        {
            _internalString = character;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jstring& operator=(const char_type* const str)
        {
            _internalString = str;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jstring& operator=(const base_type& str)
        {
            _internalString = str;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jstring& operator=(base_type&& str) noexcept
        {
            _internalString = std::move(str);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jstring& operator=(const jstring&) = default;
        JUTILS_STD20_CONSTEXPR jstring& operator=(jstring&&) noexcept = default;

        [[nodiscard]] JUTILS_STD20_CONSTEXPR const base_type& toBase() const noexcept { return _internalString; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR operator std::string() const noexcept { return toBase(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR index_type getSize() const noexcept { return _internalString.size(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool isEmpty() const noexcept { return _internalString.empty(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool isValidIndex(const index_type index) const noexcept { return index < getSize(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR iterator begin() noexcept { return _internalString.begin(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR iterator end() noexcept { return _internalString.end(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const_iterator begin() const noexcept { return _internalString.begin(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const_iterator end() const noexcept { return _internalString.end(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR char_type* getData() noexcept { return _internalString.data(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const char_type* getData() const noexcept { return _internalString.c_str(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR char_type* operator*() noexcept { return getData(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const char_type* operator*() const noexcept { return getData(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR jstring copy() const noexcept { return *this; }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR char_type& get(const index_type index) noexcept { return _internalString[index]; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const char_type& get(const index_type index) const noexcept {  return _internalString[index]; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR char_type& operator[](const index_type index) noexcept { return get(index); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const char_type& operator[](const index_type index) const noexcept { return get(index); }


        [[nodiscard]] JUTILS_STD20_CONSTEXPR index_type indexOf(const char_type character, const index_type startIndex = 0,
            const index_type finishIndex = index_invalid) const noexcept { return _indexOf(character, startIndex, finishIndex); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR index_type indexOf(const char_type* const str, const index_type startIndex = 0,
            const index_type finishIndex = index_invalid) const noexcept { return _indexOfStr(str, std::char_traits<char_type>::length(str), startIndex, finishIndex); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR index_type indexOf(const base_type& str, const index_type startIndex = 0,
            const index_type finishIndex = index_invalid) const noexcept { return _indexOf(str, startIndex, finishIndex); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR index_type indexOf(const jstring& str, const index_type startIndex = 0,
            const index_type finishIndex = index_invalid) const noexcept { return _indexOf(str.toBase(), startIndex, finishIndex); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool contains(const char_type character, const index_type startIndex = 0,
            const index_type finishIndex = index_invalid) const noexcept { return indexOf(character, startIndex, finishIndex) != index_invalid; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool contains(const char_type* const str, const index_type startIndex = 0,
            const index_type finishIndex = index_invalid) const noexcept { return indexOf(str, startIndex, finishIndex) != index_invalid; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool contains(const std::string& str, const index_type startIndex = 0,
            const index_type finishIndex = index_invalid) const noexcept { return indexOf(str, startIndex, finishIndex) != index_invalid; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool contains(const jstring& str, const index_type startIndex = 0,
            const index_type finishIndex = index_invalid) const noexcept { return indexOf(str, startIndex, finishIndex) != index_invalid; }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool startsWith(const char_type character) const noexcept { return !isEmpty() && (get(0) == character); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool startsWith(const char_type* const str) const noexcept { return _startsWith(str, std::char_traits<char_type>::length(str)); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool startsWith(const base_type& str) const noexcept { return _startsWith(str.c_str(), str.size()); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool startsWith(const jstring& str) const noexcept { return _startsWith(getData(), getSize()); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool endsWith(const char_type character) const noexcept { return !isEmpty() && (get(getSize() - 1) == character); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool endsWith(const char_type* const str) const noexcept { return _endsWith(str, std::char_traits<char_type>::length(str)); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool endsWith(const base_type& str) const noexcept { return _endsWith(str.c_str(), str.size()); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool endsWith(const jstring& str) const noexcept { return _endsWith(str.getData(), str.getSize()); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR int compare(const char_type* const str) const noexcept { return _internalString.compare(str); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR int compare(const std::string& str) const noexcept { return _internalString.compare(str); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR int compare(const jstring& str) const noexcept { return _internalString.compare(*str); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR jstring substr(index_type startIndex = 0, index_type length = index_invalid) const noexcept { return _internalString.substr(startIndex, length); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR jarray<jstring> split(const char_type delimiter) const { return _split(&delimiter, 1); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR jarray<jstring> split(const char_type* delimiter) const { return _split(delimiter, std::char_traits<char_type>::length(delimiter)); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR jarray<jstring> split(const std::string& delimiter) const { return _split(delimiter.c_str(), delimiter.size()); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR jarray<jstring> split(const jstring& delimiter) const { return _split(delimiter.getData(), delimiter.getSize()); }


        JUTILS_STD20_CONSTEXPR jstring& replace(const char_type value, const index_type startIndex, const index_type replacedLength = 0)
            { return _replace(&value, 1, startIndex, replacedLength); }
        JUTILS_STD20_CONSTEXPR jstring& replace(const char_type* value, const index_type startIndex, const index_type replacedLength = 0)
            { return _replace(value, std::char_traits<char_type>::length(value), startIndex, replacedLength); }
        JUTILS_STD20_CONSTEXPR jstring& replace(const std::string& value, const index_type startIndex, const index_type replacedLength = 0)
            { return _replace(value.c_str(), value.size(), startIndex, replacedLength); }
        JUTILS_STD20_CONSTEXPR jstring& replace(const jstring& value, const index_type startIndex, const index_type replacedLength = 0)
            { return _replace(value.getData(), value.getSize(), startIndex, replacedLength); }

        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const char_type searchValue, const char_type newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return _replaceAll(&searchValue, 1, &newValue, 1, startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const char_type searchValue, const char_type* const newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return _replaceAll(&searchValue, 1, newValue, std::char_traits<char_type>::length(newValue), startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const char_type searchValue, const std::string& newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return _replaceAll(&searchValue, 1, newValue.c_str(), newValue.size(), startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const char_type searchValue, const jstring& newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return _replaceAll(&searchValue, 1, newValue.getData(), newValue.getSize(), startIndex, finishIndex); }

        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const char_type* const searchValue, const char_type newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return _replaceAll(searchValue, std::char_traits<char_type>::length(searchValue), &newValue, 1, startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const char_type* const searchValue, const char_type* const newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return _replaceAll(searchValue, std::char_traits<char_type>::length(searchValue), newValue, std::char_traits<char_type>::length(newValue), startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const char_type* const searchValue, const std::string& newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return _replaceAll(searchValue, std::char_traits<char_type>::length(searchValue), newValue.c_str(), newValue.size(), startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const char_type* const searchValue, const jstring& newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return _replaceAll(searchValue, std::char_traits<char_type>::length(searchValue), newValue.getData(), newValue.getSize(), startIndex, finishIndex); }

        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const std::string& searchValue, const char_type newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return _replaceAll(searchValue.c_str(), searchValue.size(), &newValue, 1, startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const std::string& searchValue, const char_type* const newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return _replaceAll(searchValue.c_str(), searchValue.size(), newValue, std::char_traits<char_type>::length(newValue), startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const std::string& searchValue, const std::string& newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return _replaceAll(searchValue.c_str(), searchValue.size(), newValue.c_str(), newValue.size(), startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const std::string& searchValue, const jstring& newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return _replaceAll(searchValue.c_str(), searchValue.size(), newValue.getData(), newValue.getSize(), startIndex, finishIndex); }

        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const jstring& searchValue, const char_type newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return replaceAll(searchValue.toBase(), newValue, startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const jstring& searchValue, const char_type* const newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return replaceAll(searchValue.toBase(), newValue, startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const jstring& searchValue, const std::string& newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return replaceAll(searchValue.toBase(), newValue, startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const jstring& searchValue, const jstring& newValue,
            const index_type startIndex = 0, const index_type finishIndex = index_invalid)
            { return replaceAll(searchValue.toBase(), newValue, startIndex, finishIndex); }

        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const std::basic_regex<char_type>& searchRegex, const char_type newValue)
            { return replaceAll(searchRegex, std::string(1, newValue)); }
        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const std::basic_regex<char_type>& searchRegex, const char_type* const newValue)
            { return _replaceAll(searchRegex, newValue); }
        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const std::basic_regex<char_type>& searchRegex, const std::string& newValue)
            { return _replaceAll(searchRegex, newValue); }
        JUTILS_STD20_CONSTEXPR jstring& replaceAll(const std::basic_regex<char_type>& searchRegex, const jstring& newValue)
            { return replaceAll(searchRegex, newValue.toBase()); }


        JUTILS_STD20_CONSTEXPR void reserve(const index_type size) { _internalString.reserve(size); }
        JUTILS_STD20_CONSTEXPR void resize(const index_type size, const char_type character = char_type()) { _internalString.resize(size, character); }

        JUTILS_STD20_CONSTEXPR jstring& assign(const char_type character) { return this->operator=(character); }
        JUTILS_STD20_CONSTEXPR jstring& assign(const char_type* const str) { return this->operator=(str); }
        JUTILS_STD20_CONSTEXPR inline jstring& assign(const char_type* str, index_type strLength);
        JUTILS_STD20_CONSTEXPR jstring& assign(const base_type& str) { return this->operator=(str); }
        JUTILS_STD20_CONSTEXPR jstring& assign(base_type&& str) noexcept { return this->operator=(std::move(str)); }
        JUTILS_STD20_CONSTEXPR jstring& assign(const jstring& str) { return this->operator=(str); }
        JUTILS_STD20_CONSTEXPR jstring& assign(jstring&& str) noexcept { return this->operator=(std::move(str)); }

        JUTILS_STD20_CONSTEXPR jstring& operator+=(const char_type character)
        {
            _internalString += character;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jstring& operator+=(const char_type* const str)
        {
            _internalString += str;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jstring& operator+=(const std::string& str)
        {
            _internalString += str;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jstring& operator+=(const jstring& str) { return this->operator+=(str.toBase()); }

        JUTILS_STD20_CONSTEXPR jstring& add(const char_type character) { return this->operator+=(character); }
        JUTILS_STD20_CONSTEXPR jstring& add(const char_type* const str) { return this->operator+=(str); }
        JUTILS_STD20_CONSTEXPR jstring& add(const std::string& str) { return this->operator+=(str); }
        JUTILS_STD20_CONSTEXPR jstring& add(const jstring& str) { return this->operator+=(str); }

        JUTILS_STD20_CONSTEXPR jstring& addAt(const index_type index, const char_type character)
        {
            _internalString.insert(index, 1, character);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jstring& addAt(const index_type index, const char_type* const str)
        {
            _internalString.insert(index, str);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jstring& addAt(const index_type index, const std::string& str)
        {
            _internalString.insert(index, str);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR jstring& addAt(const index_type index, const jstring& str) { return addAt(index, str.toBase()); }

        JUTILS_STD20_CONSTEXPR void removeAt(const index_type index, const index_type count = 1) noexcept
        {
            if (isValidIndex(index) && (count > 0))
            {
                _internalString.erase(index, count);
            }
        }
        JUTILS_STD20_CONSTEXPR void clear() noexcept { _internalString.clear(); }

    private:

        std::string _internalString;

        template<typename T>
        [[nodiscard]] JUTILS_STD20_CONSTEXPR index_type _indexOf(T value, index_type startIndex, index_type finishIndex) const noexcept;
        [[nodiscard]] JUTILS_STD20_CONSTEXPR inline index_type _indexOfStr(const char_type* str, std::size_t strLength, index_type startIndex, index_type finishIndex) const noexcept;

        [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool _startsWith(const char_type* str, std::size_t strLength) const noexcept;
        [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool _endsWith(const char_type* str, std::size_t strLength) const noexcept;

        [[nodiscard]] JUTILS_STD20_CONSTEXPR inline jarray<jstring> _split(const jstring::char_type* delimiter, index_type delimiterSize) const;

        JUTILS_STD20_CONSTEXPR inline jstring& _replace(const jstring::char_type* value, std::size_t valueLength, index_type startIndex, index_type replacedLength);

        JUTILS_STD20_CONSTEXPR inline jstring& _replaceAll(const char_type* searchValue, std::size_t searchValueLength,
            const char_type* newValue, index_type newValueLength, index_type startIndex, index_type finishIndex);
        template<typename T>
        JUTILS_STD20_CONSTEXPR jstring& _replaceAll(const std::basic_regex<char_type>& searchRegex, const T newValue)
        {
            _internalString = std::regex_replace(_internalString, searchRegex, newValue);
            return *this;
        }
    };

    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline jstring operator+(const jstring& str1, const jstring::char_type character) { return str1.toBase() + character; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline jstring operator+(jstring&& str1, const jstring::char_type character) { return jstring(std::move(str1)) += character; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline jstring operator+(const jstring::char_type character, const jstring& str1) { return character + str1.toBase(); }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline jstring operator+(const jstring::char_type character, jstring&& str1) { return jstring(std::move(str1)).addAt(0, character); }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline jstring operator+(const jstring& str1, const jstring& str2) { return str1.toBase() + str2.toBase(); }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline jstring operator+(const jstring& str1, jstring&& str2) { return jstring(std::move(str2)).addAt(0, str1); }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline jstring operator+(jstring&& str1, const jstring& str2) { return jstring(std::move(str1)) += str2; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline jstring operator+(const jstring& str1, const jstring::char_type* str2) { return str1.toBase() + str2; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline jstring operator+(jstring&& str1, const jstring::char_type* str2) { return jstring(std::move(str1)) += str2; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline jstring operator+(const jstring::char_type* const str1, const jstring& str2) { return str1 + str2.toBase(); }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline jstring operator+(const jstring::char_type* const str1, jstring&& str2) { return jstring(std::move(str2)).addAt(0, str1); }

    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator==(const jstring& str1, const jstring& str2) noexcept { return str1.compare(str2) == 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator==(const jstring& str1, const jstring::char_type* const str2) noexcept { return str1.compare(str2) == 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator==(const jstring::char_type* const str1, const jstring& str2) noexcept { return str2 == str1; }

    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator!=(const jstring& str1, const jstring& str2) noexcept { return !(str1 == str2); }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator!=(const jstring& str1, const jstring::char_type* const str2) noexcept { return !(str1 == str2); }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator!=(const jstring::char_type* const str1, const jstring& str2) noexcept { return !(str1 == str2); }

    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator<(const jstring& str1, const jstring& str2) noexcept { return str1.compare(str2) < 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator<(const jstring& str1, const jstring::char_type* const str2) noexcept { return str1.compare(str2) < 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator<(const jstring::char_type* const str1, const jstring& str2) noexcept { return str2.compare(str1) > 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator<=(const jstring& str1, const jstring& str2) noexcept { return str1.compare(str2) <= 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator<=(const jstring& str1, const jstring::char_type* const str2) noexcept { return str1.compare(str2) <= 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator<=(const jstring::char_type* const str1, const jstring& str2) noexcept { return str2.compare(str1) >= 0; }
    
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator>(const jstring& str1, const jstring& str2) noexcept { return str1.compare(str2) > 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator>(const jstring& str1, const jstring::char_type* const str2) noexcept { return str1.compare(str2) > 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator>(const jstring::char_type* const str1, const jstring& str2) noexcept { return str2.compare(str1) < 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator>=(const jstring& str1, const jstring& str2) noexcept { return str1.compare(str2) >= 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator>=(const jstring& str1, const jstring::char_type* const str2) noexcept { return str1.compare(str2) >= 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR inline bool operator>=(const jstring::char_type* const str1, const jstring& str2) noexcept { return str2.compare(str1) <= 0; }



    template<typename T>
    JUTILS_STD20_CONSTEXPR index_type jstring::_indexOf(const T value, const index_type startIndex, const index_type finishIndex) const noexcept
    {
        if ((finishIndex != index_invalid) && (finishIndex < startIndex))
        {
            return index_invalid;
        }
        const auto index = _internalString.find(value, startIndex);
        return (finishIndex == index_invalid) || (finishIndex >= index) ? index : index_invalid;
    }
    JUTILS_STD20_CONSTEXPR index_type jstring::_indexOfStr(const char_type* const str, const std::size_t strLength, const index_type startIndex,
        const index_type finishIndex) const noexcept
    {
        if ((finishIndex != index_invalid) && (finishIndex < startIndex))
        {
            return index_invalid;
        }
        const auto index = _internalString.find(str, startIndex, strLength);
        return (finishIndex == index_invalid) || (finishIndex >= index) ? index : index_invalid;
    }

    JUTILS_STD20_CONSTEXPR bool jstring::_startsWith(const jstring::char_type* const str, const std::size_t strLength) const noexcept
    {
        if (strLength > getSize())
        {
            return false;
        }
        const char_type* data = getData();
        for (std::size_t index = 0; index < strLength; index++)
        {
            if (data[index] != str[index])
            {
                return false;
            }
        }
        return true;
    }
    JUTILS_STD20_CONSTEXPR bool jstring::_endsWith(const jstring::char_type* const str, const std::size_t strLength) const noexcept
    {
        if (strLength > getSize())
        {
            return false;
        }
        const index_type offset = getSize() - strLength;
        const char_type* data = getData();
        for (std::size_t index = 0; index < strLength; index++)
        {
            if (data[offset + index] != str[index])
            {
                return false;
            }
        }
        return false;
    }

    JUTILS_STD20_CONSTEXPR jarray<jstring> jstring::_split(const jstring::char_type* const delimiter, const index_type delimiterSize) const
    {
        if (isEmpty())
        {
            return {};
        }
        if (delimiterSize == 0)
        {
            return { *this };
        }
        jarray<jstring> result;
        index_type index = 0;
        while (index < getSize())
        {
            const index_type delimiterIndex = _indexOfStr(delimiter, delimiterSize, index, index_invalid);
            if (delimiterIndex != index_invalid)
            {
                if (delimiterIndex > 0)
                {
                    result.add(substr(index, delimiterIndex - index));
                }
                index = delimiterIndex + delimiterSize;
            }
            else
            {
                result.add(substr(index));
                index = getSize();
            }
        }
        return result;
    }

    JUTILS_STD20_CONSTEXPR jstring& jstring::_replace(const jstring::char_type* const value, const std::size_t valueLength,
        const index_type startIndex, const index_type replacedLength)
    {
        if (startIndex == getSize())
        {
            return add(value);
        }
        if (startIndex < getSize())
        {
            _internalString.replace(startIndex, replacedLength, value, valueLength);
        }
        return *this;
    }

    JUTILS_STD20_CONSTEXPR jstring& jstring::_replaceAll(const jstring::char_type* const searchValue, const std::size_t searchValueLength,
        const jstring::char_type* const newValue, const index_type newValueLength,
        const index_type startIndex, index_type finishIndex)
    {
        const index_type replacedStrDiff = newValueLength - searchValueLength;
        index_type index = _indexOfStr(searchValue, searchValueLength, startIndex, finishIndex);
        while (index != index_invalid)
        {
            _replace(newValue, newValueLength, index, searchValueLength);
            finishIndex += replacedStrDiff;
            index = _indexOfStr(searchValue, searchValueLength, index + newValueLength, finishIndex);
        }
        return *this;
    }

    JUTILS_STD20_CONSTEXPR jstring& jstring::assign(const jstring::char_type* const str, const index_type strLength)
    {
        if (strLength == 0)
        {
            clear();
        }
        else
        {
            _internalString.assign(str, strLength);
        }
        return *this;
    }
}

template<>
struct jutils::hash<jutils::jstring>
{
    [[nodiscard]] JUTILS_STD20_CONSTEXPR jutils::hash_type operator()(const jutils::jstring& str) const noexcept
        { return jutils::hash<const char*>{}(*str); }
};
template<>
struct jutils::hash<std::string>
{
    [[nodiscard]] JUTILS_STD20_CONSTEXPR jutils::hash_type operator()(const std::string& str) const noexcept
        { return jutils::hash<const char*>{}(str.c_str()); }
};
