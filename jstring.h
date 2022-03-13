// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "type_defines.h"
#include "math/math.h"
#include "math/hash.h"

#include <string>

namespace jutils
{
    class jstring
    {
    public:

        using character_type = char;
        using internal_type = std::basic_string<character_type>;
        using const_iterator = internal_type::const_iterator;
        using iterator = internal_type::iterator;
        using index_type = int32;

        jstring() = default;
        jstring(const character_type* const str)
            : internalString(str)
        {}
        jstring(const character_type* const str, const index_type count)
            : internalString(str, math::max(count, 0))
        {}
        jstring(const int32 count, const character_type character)
            : internalString(math::max(count, 0), character)
        {}
        explicit jstring(const internal_type& str)
            : internalString(str)
        {}
        explicit jstring(internal_type&& str) noexcept
            : internalString(std::move(str))
        {}
        jstring(const jstring& str) = default;
        jstring(jstring&& string) noexcept
            : jstring(std::move(string.internalString))
        {}
        ~jstring() = default;
        
        jstring& operator=(const character_type character)
        {
            assign(character);
            return *this;
        }
        jstring& operator=(const character_type* const str)
        {
            assign(str);
            return *this;
        }
        jstring& operator=(const internal_type& str)
        {
            assign(str);
            return *this;
        }
        jstring& operator=(internal_type&& str)
        {
            assign(std::move(str));
            return *this;
        }
        jstring& operator=(const jstring& str)
        {
            assign(str);
            return *this;
        }
        jstring& operator=(jstring&& str) noexcept
        {
            assign(std::move(str));
            return *this;
        }

        index_type getSize() const { return static_cast<index_type>(internalString.size()); }
        bool isEmpty() const { return internalString.empty(); }
        bool isValidIndex(const index_type index) const { return math::isWithin(index, 0, getSize() - 1); }

        iterator begin() { return internalString.begin(); }
        iterator end() { return internalString.end(); }
        
        const_iterator begin() const { return internalString.begin(); }
        const_iterator end() const { return internalString.end(); }

        character_type* getString() noexcept { return internalString.data(); }
        character_type* operator*() noexcept { return getString(); }

        const character_type* getString() const noexcept { return internalString.c_str(); }
        const character_type* operator*() const noexcept { return getString(); }

        character_type& get(const index_type index)
        {
            _checkIfOutOfRange(index);
            return internalString[index];
        }
        const character_type& get(const index_type index) const
        {
            _checkIfOutOfRange(index);
            return internalString[index];
        }
        character_type& operator[](const index_type index) { return get(index); }
        const character_type& operator[](const index_type index) const { return get(index); }

        index_type indexOf(const character_type character, const index_type startIndex = 0, const index_type finishIndex = -1) const
        {
            const size_t index = internalString.find(character, math::max(startIndex, 0));
            return (index != internal_type::npos) && ((finishIndex < 0) || (index <= static_cast<size_t>(finishIndex))) ? static_cast<index_type>(index) : -1;
        }
        index_type indexOf(const character_type* const str, const index_type startIndex = 0, const index_type finishIndex = -1) const
        {
            const size_t index = internalString.find(str, math::max(startIndex, 0));
            return (index != internal_type::npos) && ((finishIndex < 0) || (index <= static_cast<size_t>(finishIndex))) ? static_cast<index_type>(index) : -1;
        }
        index_type indexOf(const character_type* const str, const index_type strLength, const index_type startIndex, const index_type finishIndex) const
        {
            if (strLength <= 0)
            {
                return -1;
            }
            const size_t index = internalString.find(str, math::max(0, startIndex), strLength);
            return (index != internal_type::npos) && ((finishIndex < 0) || (index <= static_cast<size_t>(finishIndex))) ? static_cast<index_type>(index) : -1;
        }
        index_type indexOf(const jstring& str, const index_type startIndex = 0, const index_type finishIndex = -1) const { return indexOf(*str, str.getSize(), startIndex, finishIndex); }
        
        bool contains(const character_type character, const index_type startIndex = 0, const index_type finishIndex = -1) const { return indexOf(character, startIndex, finishIndex) != -1; }
        bool contains(const character_type* const str, const index_type startIndex = 0, const index_type finishIndex = -1) const { return indexOf(str, startIndex, finishIndex) != -1; }
        bool contains(const character_type* const str, const index_type strLength, const index_type startIndex, const int32 finishIndex) const { return indexOf(str, strLength, startIndex, finishIndex) != -1; }
        bool contains(const jstring& str, const index_type startIndex = 0, const index_type finishIndex = -1) const { return indexOf(*str, str.getSize(), startIndex, finishIndex); }
        
        jstring substr(const index_type startIndex = 0, const index_type length = -1) const
        {
            if (length == 0)
            {
                return jstring();
            }
            return jstring(internalString.substr(startIndex, length > 0 ? length : internal_type::npos));
        }

        index_type compare(const character_type* const str) const { return internalString.compare(str); }
        index_type compare(const jstring& str) const { return internalString.compare(*str); }
        
        void reserve(const index_type size) { internalString.reserve(math::max(size, 0)); }
        void resize(const index_type size, const character_type character = character_type()) { internalString.resize(math::max(size, 0), character); }

        jstring& assign(const character_type character)
        {
            internalString = character;
            return *this;
        }
        jstring& assign(const character_type* const str)
        {
            internalString = str;
            return *this;
        }
        jstring& assign(const character_type* const str, const int32 strLength)
        {
            if (strLength <= 0)
            {
                clear();
            }
            else
            {
                internalString.assign(str, strLength);
            }
            return *this;
        }
        jstring& assign(const internal_type& str)
        {
            internalString = str;
            return *this;
        }
        jstring& assign(internal_type&& str)
        {
            internalString = std::move(str);
            return *this;
        }
        jstring& assign(const jstring& str) { return assign(str.internalString); }
        jstring& assign(jstring&& str) { return assign(std::move(str.internalString)); }

        jstring& add(const character_type character)
        {
            internalString += character;
            return *this;
        }
        jstring& add(const character_type* const str)
        {
            internalString += str;
            return *this;
        }
        jstring& add(const character_type* const str, const index_type strLength)
        {
            if (strLength > 0)
            {
                internalString.append(str, strLength);
            }
            return *this;
        }
        jstring& add(const jstring& str)
        {
            internalString += str.internalString;
            return *this;
        }

        jstring& addAt(const index_type index, const character_type character)
        {
            _checkIfOutOfRange(index);
            internalString.insert(index, 1, character);
            return *this;
        }
        jstring& addAt(const index_type index, const character_type* const str)
        {
            _checkIfOutOfRange(index);
            internalString.insert(index, str);
            return *this;
        }
        jstring& addAt(const index_type index, const character_type* const str, const index_type strLength)
        {
            _checkIfOutOfRange(index);
            if (strLength > 0)
            {
                internalString.insert(index, str, strLength);
            }
            return *this;
        }
        jstring& addAt(const index_type index, const jstring& str)
        {
            _checkIfOutOfRange(index);
            internalString.insert(index, str.internalString);
            return *this;
        }
        
        void removeAt(const index_type index, const index_type count = 1)
        {
            if (isValidIndex(index) && (count > 0))
            {
                internalString.erase(index);
            }
        }

        void clear() { internalString.clear(); }
        
        constexpr uint64 hash() const { return math::hash::crc64(getString(), getSize()); }

        jstring& operator+=(const character_type character) { return add(character); }
        jstring& operator+=(const character_type* const str) { return add(str); }
        jstring& operator+=(const jstring& str) { return add(str); }

    private:

        internal_type internalString;


        void _checkIfOutOfRange(const index_type index) const
        {
            if (!isValidIndex(index))
            {
                throw std::out_of_range("Invalid jstring index!");
            }
        }
    };

    inline std::basic_istream<jstring::character_type, jstring::internal_type::traits_type>& readStreamLine(
        std::basic_istream<jstring::character_type, jstring::internal_type::traits_type>& stream, jstring& outString)
    {
        std::string str;
        auto& result = std::getline(stream, str);
        outString = std::move(str);
        return result;
    }

    template<typename T>
    jstring to_jstring(T value) { return jstring(std::to_string(value)); }

    inline jstring operator+(const jstring& str1, const jstring& str2) { return jstring(str1) += str2; }
    inline jstring operator+(const jstring& str1, const jstring::character_type* str2) { return jstring(str1) += str2; }
    inline jstring operator+(const jstring::character_type* const str1, const jstring& str2) { return jstring(str1) += str2; }

    inline bool operator==(const jstring& str1, const jstring& str2) { return str1.compare(str2) == 0; }
    inline bool operator==(const jstring& str1, const jstring::character_type* const str2) { return str1.compare(str2) == 0; }
    inline bool operator==(const jstring::character_type* const str1, const jstring& str2) { return str2 == str1; }

    inline bool operator!=(const jstring& str1, const jstring& str2) { return !(str1 == str2); }
    inline bool operator!=(const jstring& str1, const jstring::character_type* const str2) { return !(str1 == str2); }
    inline bool operator!=(const jstring::character_type* const str1, const jstring& str2) { return !(str1 == str2); }

    inline bool operator<(const jstring& str1, const jstring& str2) { return str1.compare(str2) < 0; }
    inline bool operator<(const jstring& str1, const jstring::character_type* const str2) { return str1.compare(str2) < 0; }
    inline bool operator<(const jstring::character_type* const str1, const jstring& str2) { return str2.compare(str1) > 0; }
    
    inline bool operator>(const jstring& str1, const jstring& str2) { return str1.compare(str2) > 0; }
    inline bool operator>(const jstring& str1, const jstring::character_type* const str2) { return str1.compare(str2) > 0; }
    inline bool operator>(const jstring::character_type* const str1, const jstring& str2) { return str2.compare(str1) < 0; }

    inline std::basic_ostream<jstring::character_type, jstring::internal_type::traits_type>& operator<<(
        std::basic_ostream<jstring::character_type, jstring::internal_type::traits_type>& stream, const jstring& str)
    {
        return stream << *str;
    }
}

#define JSTR(str) u8 ## str
#define TO_JSTR(value) jutils::to_jstring(value)
