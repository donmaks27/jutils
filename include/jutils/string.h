// Copyright © 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#include "math/hash.h"
#include "vector.h"

#include <string>
#include <regex>

namespace jutils
{
    class string
    {
    public:

        using base_type = std::string;
        using char_type = base_type::value_type;
        using const_iterator = base_type::const_iterator;
        using iterator = base_type::iterator;

        JUTILS_STD20_CONSTEXPR string() noexcept = default;
        JUTILS_STD20_CONSTEXPR string(const char_type character)
            : _internalString(1, character)
        {}
        JUTILS_STD20_CONSTEXPR string(const char_type* str)
            : _internalString(str)
        {}
        JUTILS_STD20_CONSTEXPR string(const char_type* const str, const std::size_t count)
            : _internalString(str, count)
        {}
        JUTILS_STD20_CONSTEXPR string(const std::size_t count, const char_type character)
            : _internalString(count, character)
        {}
        JUTILS_STD20_CONSTEXPR string(const base_type& str)
            : _internalString(str)
        {}
        JUTILS_STD20_CONSTEXPR string(base_type&& str) noexcept
            : _internalString(std::move(str))
        {}
        JUTILS_STD20_CONSTEXPR string(const string& str) = default;
        JUTILS_STD20_CONSTEXPR string(string&& str) noexcept = default;
        JUTILS_STD20_CONSTEXPR ~string() = default;

        JUTILS_STD20_CONSTEXPR string& operator=(const char_type character)
        {
            _internalString = character;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR string& operator=(const char_type* const str)
        {
            _internalString = str;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR string& operator=(const base_type& str)
        {
            _internalString = str;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR string& operator=(base_type&& str) noexcept
        {
            _internalString = std::move(str);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR string& operator=(const string&) = default;
        JUTILS_STD20_CONSTEXPR string& operator=(string&&) noexcept = default;

        [[nodiscard]] JUTILS_STD20_CONSTEXPR const base_type& toBase() const noexcept { return _internalString; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR operator std::string() const noexcept { return toBase(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR std::size_t getSize() const noexcept { return _internalString.size(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool isEmpty() const noexcept { return _internalString.empty(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool isValidIndex(const std::size_t index) const noexcept { return index < getSize(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR iterator begin() noexcept { return _internalString.begin(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR iterator end() noexcept { return _internalString.end(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const_iterator begin() const noexcept { return _internalString.begin(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const_iterator end() const noexcept { return _internalString.end(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR char_type* getData() noexcept { return _internalString.data(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const char_type* getData() const noexcept { return _internalString.c_str(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR char_type* operator*() noexcept { return getData(); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const char_type* operator*() const noexcept { return getData(); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR string copy() const noexcept { return *this; }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR char_type& get(const std::size_t index) noexcept { return _internalString[index]; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const char_type& get(const std::size_t index) const noexcept {  return _internalString[index]; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR char_type& operator[](const std::size_t index) noexcept { return get(index); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR const char_type& operator[](const std::size_t index) const noexcept { return get(index); }


        [[nodiscard]] JUTILS_STD20_CONSTEXPR std::size_t indexOf(const char_type character, const std::size_t startIndex = 0,
            const std::size_t finishIndex = index_invalid) const noexcept { return _indexOf(character, startIndex, finishIndex); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR std::size_t indexOf(const char_type* const str, const std::size_t startIndex = 0,
            const std::size_t finishIndex = index_invalid) const noexcept { return _indexOfStr(str, std::char_traits<char_type>::length(str), startIndex, finishIndex); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR std::size_t indexOf(const base_type& str, const std::size_t startIndex = 0,
            const std::size_t finishIndex = index_invalid) const noexcept { return _indexOf(str, startIndex, finishIndex); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR std::size_t indexOf(const string& str, const std::size_t startIndex = 0,
            const std::size_t finishIndex = index_invalid) const noexcept { return _indexOf(str.toBase(), startIndex, finishIndex); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool contains(const char_type character, const std::size_t startIndex = 0,
            const std::size_t finishIndex = index_invalid) const noexcept { return indexOf(character, startIndex, finishIndex) != index_invalid; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool contains(const char_type* const str, const std::size_t startIndex = 0,
            const std::size_t finishIndex = index_invalid) const noexcept { return indexOf(str, startIndex, finishIndex) != index_invalid; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool contains(const std::string& str, const std::size_t startIndex = 0,
            const std::size_t finishIndex = index_invalid) const noexcept { return indexOf(str, startIndex, finishIndex) != index_invalid; }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool contains(const string& str, const std::size_t startIndex = 0,
            const std::size_t finishIndex = index_invalid) const noexcept { return indexOf(str, startIndex, finishIndex) != index_invalid; }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool startsWith(const char_type character) const noexcept
            { return !isEmpty() && (get(0) == character); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool startsWith(const char_type* const str) const noexcept
            { return _startsWith(str, std::char_traits<char_type>::length(str)); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool startsWith(const base_type& str) const noexcept
            { return _startsWith(str.c_str(), str.size()); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool startsWith(const string& str) const noexcept
            { return _startsWith(getData(), getSize()); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool endsWith(const char_type character) const noexcept
            { return !isEmpty() && (get(getSize() - 1) == character); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool endsWith(const char_type* const str) const noexcept
            { return _endsWith(str, std::char_traits<char_type>::length(str)); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool endsWith(const base_type& str) const noexcept
            { return _endsWith(str.c_str(), str.size()); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR bool endsWith(const string& str) const noexcept
            { return _endsWith(str.getData(), str.getSize()); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR int compare(const char_type* const str) const noexcept { return _internalString.compare(str); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR int compare(const std::string& str) const noexcept { return _internalString.compare(str); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR int compare(const string& str) const noexcept { return _internalString.compare(*str); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR string substr(std::size_t startIndex = 0, std::size_t length = index_invalid) const noexcept
            { return _internalString.substr(startIndex, length); }

        [[nodiscard]] JUTILS_STD20_CONSTEXPR vector<string> split(const char_type delimiter) const
            { return _split(&delimiter, 1); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR vector<string> split(const char_type* delimiter) const
            { return _split(delimiter, std::char_traits<char_type>::length(delimiter)); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR vector<string> split(const std::string& delimiter) const
            { return _split(delimiter.c_str(), delimiter.size()); }
        [[nodiscard]] JUTILS_STD20_CONSTEXPR vector<string> split(const string& delimiter) const
            { return _split(delimiter.getData(), delimiter.getSize()); }


        JUTILS_STD20_CONSTEXPR string& replace(const char_type value, const std::size_t startIndex, const std::size_t replacedLength = 0)
            { return _replace(&value, 1, startIndex, replacedLength); }
        JUTILS_STD20_CONSTEXPR string& replace(const char_type* value, const std::size_t startIndex, const std::size_t replacedLength = 0)
            { return _replace(value, std::char_traits<char_type>::length(value), startIndex, replacedLength); }
        JUTILS_STD20_CONSTEXPR string& replace(const std::string& value, const std::size_t startIndex, const std::size_t replacedLength = 0)
            { return _replace(value.c_str(), value.size(), startIndex, replacedLength); }
        JUTILS_STD20_CONSTEXPR string& replace(const string& value, const std::size_t startIndex, const std::size_t replacedLength = 0)
            { return _replace(value.getData(), value.getSize(), startIndex, replacedLength); }

        JUTILS_STD20_CONSTEXPR string& replaceAll(const char_type searchValue, const char_type newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return _replaceAll(&searchValue, 1, &newValue, 1, startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR string& replaceAll(const char_type searchValue, const char_type* const newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return _replaceAll(&searchValue, 1, newValue, std::char_traits<char_type>::length(newValue), startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR string& replaceAll(const char_type searchValue, const std::string& newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return _replaceAll(&searchValue, 1, newValue.c_str(), newValue.size(), startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR string& replaceAll(const char_type searchValue, const string& newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return _replaceAll(&searchValue, 1, newValue.getData(), newValue.getSize(), startIndex, finishIndex); }

        JUTILS_STD20_CONSTEXPR string& replaceAll(const char_type* const searchValue, const char_type newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return _replaceAll(searchValue, std::char_traits<char_type>::length(searchValue), &newValue, 1, startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR string& replaceAll(const char_type* const searchValue, const char_type* const newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return _replaceAll(searchValue, std::char_traits<char_type>::length(searchValue), newValue, std::char_traits<char_type>::length(newValue), startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR string& replaceAll(const char_type* const searchValue, const std::string& newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return _replaceAll(searchValue, std::char_traits<char_type>::length(searchValue), newValue.c_str(), newValue.size(), startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR string& replaceAll(const char_type* const searchValue, const string& newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return _replaceAll(searchValue, std::char_traits<char_type>::length(searchValue), newValue.getData(), newValue.getSize(), startIndex, finishIndex); }

        JUTILS_STD20_CONSTEXPR string& replaceAll(const std::string& searchValue, const char_type newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return _replaceAll(searchValue.c_str(), searchValue.size(), &newValue, 1, startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR string& replaceAll(const std::string& searchValue, const char_type* const newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return _replaceAll(searchValue.c_str(), searchValue.size(), newValue, std::char_traits<char_type>::length(newValue), startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR string& replaceAll(const std::string& searchValue, const std::string& newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return _replaceAll(searchValue.c_str(), searchValue.size(), newValue.c_str(), newValue.size(), startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR string& replaceAll(const std::string& searchValue, const string& newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return _replaceAll(searchValue.c_str(), searchValue.size(), newValue.getData(), newValue.getSize(), startIndex, finishIndex); }

        JUTILS_STD20_CONSTEXPR string& replaceAll(const string& searchValue, const char_type newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return replaceAll(searchValue.toBase(), newValue, startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR string& replaceAll(const string& searchValue, const char_type* const newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return replaceAll(searchValue.toBase(), newValue, startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR string& replaceAll(const string& searchValue, const std::string& newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return replaceAll(searchValue.toBase(), newValue, startIndex, finishIndex); }
        JUTILS_STD20_CONSTEXPR string& replaceAll(const string& searchValue, const string& newValue,
            const std::size_t startIndex = 0, const std::size_t finishIndex = index_invalid)
            { return replaceAll(searchValue.toBase(), newValue, startIndex, finishIndex); }

        JUTILS_STD20_CONSTEXPR string& replaceAll(const std::basic_regex<char_type>& regex, const char_type replacement)
            { return replaceAll(regex, std::string(1, replacement)); }
        JUTILS_STD20_CONSTEXPR string& replaceAll(const std::basic_regex<char_type>& regex, const char_type* const replacement)
            { return _replaceAll(regex, replacement); }
        JUTILS_STD20_CONSTEXPR string& replaceAll(const std::basic_regex<char_type>& regex, const std::string& replacement)
            { return _replaceAll(regex, replacement); }
        JUTILS_STD20_CONSTEXPR string& replaceAll(const std::basic_regex<char_type>& regex, const string& replacement)
            { return replaceAll(regex, replacement.toBase()); }


        JUTILS_STD20_CONSTEXPR void reserve(const std::size_t size) { _internalString.reserve(size); }
        JUTILS_STD20_CONSTEXPR void resize(const std::size_t size, const char_type character = char_type()) { _internalString.resize(size, character); }

        JUTILS_STD20_CONSTEXPR string& assign(const char_type character) { return this->operator=(character); }
        JUTILS_STD20_CONSTEXPR string& assign(const char_type* const str) { return this->operator=(str); }
        JUTILS_STD20_CONSTEXPR string& assign(const char_type* str, const std::size_t strLength)
        {
            _internalString.assign(str, strLength);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR string& assign(const base_type& str) { return this->operator=(str); }
        JUTILS_STD20_CONSTEXPR string& assign(base_type&& str) noexcept { return this->operator=(std::move(str)); }
        JUTILS_STD20_CONSTEXPR string& assign(const string& str) { return this->operator=(str); }
        JUTILS_STD20_CONSTEXPR string& assign(string&& str) noexcept { return this->operator=(std::move(str)); }

        JUTILS_STD20_CONSTEXPR string& operator+=(const char_type character)
        {
            _internalString += character;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR string& operator+=(const char_type* const str)
        {
            _internalString += str;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR string& operator+=(const std::string& str)
        {
            _internalString += str;
            return *this;
        }
        JUTILS_STD20_CONSTEXPR string& operator+=(const string& str) { return this->operator+=(str.toBase()); }

        JUTILS_STD20_CONSTEXPR string& add(const char_type character) { return this->operator+=(character); }
        JUTILS_STD20_CONSTEXPR string& add(const char_type* const str) { return this->operator+=(str); }
        JUTILS_STD20_CONSTEXPR string& add(const char_type* const str, const std::size_t strLength)
        {
            _internalString.append(str, strLength);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR string& add(const std::string& str) { return this->operator+=(str); }
        JUTILS_STD20_CONSTEXPR string& add(const string& str) { return this->operator+=(str); }

        JUTILS_STD20_CONSTEXPR string& addAt(const std::size_t index, const char_type character)
        {
            _internalString.insert(index, 1, character);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR string& addAt(const std::size_t index, const char_type* const str)
        {
            _internalString.insert(index, str);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR string& addAt(const std::size_t index, const char_type* const str, const std::size_t strLength)
        {
            _internalString.insert(index, str, strLength);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR string& addAt(const std::size_t index, const std::string& str)
        {
            _internalString.insert(index, str);
            return *this;
        }
        JUTILS_STD20_CONSTEXPR string& addAt(const std::size_t index, const string& str) { return addAt(index, str.toBase()); }

        JUTILS_STD20_CONSTEXPR void removeAt(const std::size_t index, const std::size_t count = 1) noexcept
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
        JUTILS_STD20_CONSTEXPR std::size_t _indexOf(const T& value, const std::size_t startIndex, const std::size_t finishIndex) const noexcept
        {
            if ((finishIndex != index_invalid) && (finishIndex < startIndex))
            {
                return index_invalid;
            }
            const auto index = _internalString.find(value, startIndex);
            return (finishIndex == index_invalid) || (finishIndex >= index) ? index : index_invalid;
        }
        JUTILS_STD20_CONSTEXPR std::size_t _indexOfStr(const char_type* str, const std::size_t strLength, const std::size_t startIndex, 
            const std::size_t finishIndex) const noexcept
        {
            if ((finishIndex != index_invalid) && (finishIndex < startIndex))
            {
                return index_invalid;
            }
            const auto index = _internalString.find(str, startIndex, strLength);
            return (finishIndex == index_invalid) || (finishIndex >= index) ? index : index_invalid;
        }

        JUTILS_STD20_CONSTEXPR bool _startsWith(const char_type* str, const std::size_t strLength) const noexcept
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
        JUTILS_STD20_CONSTEXPR bool _endsWith(const char_type* str, const std::size_t strLength) const noexcept
        {
            if (strLength > getSize())
            {
                return false;
            }
            const std::size_t offset = getSize() - strLength;
            const char_type* data = getData();
            for (std::size_t index = 0; index < strLength; index++)
            {
                if (data[offset + index] != str[index])
                {
                    return false;
                }
            }
            return true;
        }

        JUTILS_STD20_CONSTEXPR vector<string> _split(const char_type* delimiter, const std::size_t delimiterSize) const
        {
            if (isEmpty())
            {
                return {};
            }
            if (delimiterSize == 0)
            {
                return { *this };
            }
            vector<string> result;
            std::size_t index = 0;
            while (index < getSize())
            {
                const std::size_t delimiterIndex = _indexOfStr(delimiter, delimiterSize, index, index_invalid);
                if (delimiterIndex == index_invalid)
                {
                    result.add(substr(index));
                    break;
                }
                if (delimiterIndex > 0)
                {
                    result.add(substr(index, delimiterIndex - index));
                }
                index = delimiterIndex + delimiterSize;
            }
            return result;
        }

        JUTILS_STD20_CONSTEXPR string& _replace(const char_type* value, const std::size_t valueLength, const std::size_t startIndex, 
            const std::size_t replacedLength)
        {
            if (startIndex < getSize())
            {
                _internalString.replace(startIndex, replacedLength, value, valueLength);
            }
            return *this;
        }

        JUTILS_STD20_CONSTEXPR string& _replaceAll(const char_type* searchValue, const std::size_t searchValueLength,
            const char_type* newValue, const std::size_t newValueLength, const std::size_t startIndex, std::size_t finishIndex)
        {
            const std::size_t replacedStrDiff = newValueLength - searchValueLength;
            std::size_t index = _indexOfStr(searchValue, searchValueLength, startIndex, finishIndex);
            while (index != index_invalid)
            {
                _replace(newValue, newValueLength, index, searchValueLength);
                finishIndex += replacedStrDiff;
                index = _indexOfStr(searchValue, searchValueLength, index + newValueLength, finishIndex);
            }
            return *this;
        }
        template<typename T>
        JUTILS_STD20_CONSTEXPR string& _replaceAll(const std::basic_regex<char_type>& searchRegex, const T& replacement)
        {
            _internalString = std::regex_replace(_internalString, searchRegex, replacement);
            return *this;
        }
    };

    [[nodiscard]] JUTILS_STD20_CONSTEXPR string operator+(string&& str, const string::char_type character)
    {
        string result = std::move(str);
        result += character;
        return result;
    }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR string operator+(const string::char_type character, string&& str)
    {
        string result = std::move(str);
        result.addAt(0, character);
        return result;
    }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR string operator+(string&& str1, const string& str2)
    {
        string result = std::move(str1);
        result += str2;
        return result;
    }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR string operator+(const string& str1, string&& str2)
    {
        string result = std::move(str2);
        result.addAt(0, str1);
        return result;
    }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR string operator+(string&& str1, const string::char_type* str2)
    {
        string result = std::move(str1);
        result += str2;
        return result;
    }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR string operator+(const string::char_type* const str1, string&& str2)
    {
        string result = std::move(str2);
        result.addAt(0, str1);
        return result;
    }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR string operator+(const string& str1, const string::char_type character)    { return str1.toBase() + character; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR string operator+(const string::char_type character, const string& str1)    { return character + str1.toBase(); }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR string operator+(const string& str1, const string& str2)                   { return str1.toBase() + str2.toBase(); }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR string operator+(const string& str1, const string::char_type* str2)        { return str1.toBase() + str2; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR string operator+(const string::char_type* const str1, const string& str2)  { return str1 + str2.toBase(); }

    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator==(const string& str1, const string& str2) noexcept                   { return str1.compare(str2) == 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator==(const string& str1, const string::char_type* const str2) noexcept  { return str1.compare(str2) == 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator==(const string::char_type* const str1, const string& str2) noexcept  { return str2 == str1; }

    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator!=(const string& str1, const string& str2) noexcept                   { return !(str1 == str2); }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator!=(const string& str1, const string::char_type* const str2) noexcept  { return !(str1 == str2); }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator!=(const string::char_type* const str1, const string& str2) noexcept  { return !(str1 == str2); }

    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator<(const string& str1, const string& str2) noexcept                    { return str1.compare(str2) < 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator<(const string& str1, const string::char_type* const str2) noexcept   { return str1.compare(str2) < 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator<(const string::char_type* const str1, const string& str2) noexcept   { return str2.compare(str1) > 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator<=(const string& str1, const string& str2) noexcept                   { return str1.compare(str2) <= 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator<=(const string& str1, const string::char_type* const str2) noexcept  { return str1.compare(str2) <= 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator<=(const string::char_type* const str1, const string& str2) noexcept  { return str2.compare(str1) >= 0; }
    
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator>(const string& str1, const string& str2) noexcept                    { return str1.compare(str2) > 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator>(const string& str1, const string::char_type* const str2) noexcept   { return str1.compare(str2) > 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator>(const string::char_type* const str1, const string& str2) noexcept   { return str2.compare(str1) < 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator>=(const string& str1, const string& str2) noexcept                   { return str1.compare(str2) >= 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator>=(const string& str1, const string::char_type* const str2) noexcept  { return str1.compare(str2) >= 0; }
    [[nodiscard]] JUTILS_STD20_CONSTEXPR bool operator>=(const string::char_type* const str1, const string& str2) noexcept  { return str2.compare(str1) <= 0; }
}

namespace jutils::math
{
    template<>
    struct hash<string>
    {
        [[nodiscard]] JUTILS_STD20_CONSTEXPR math::hash_t operator()(const string& str) const noexcept
            { return math::hash<const char*>{}(*str); }
    };
    template<>
    struct hash<std::string>
    {
        [[nodiscard]] JUTILS_STD20_CONSTEXPR math::hash_t operator()(const std::string& str) const noexcept
            { return math::hash<const char*>{}(str.c_str()); }
    };
}
