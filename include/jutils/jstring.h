// Copyright © 2021-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "type_defines.h"
#include "math/math.h"
#include "math/hash.h"

#include <format>
#include <string>

namespace jutils
{
    class jstring : private std::string
    {
    public:
        
        using base_type = std::string;
        using character_type = base_type::value_type;
        using index_type = std::make_signed_t<base_type::size_type>;

        static constexpr index_type invalidIndex = -1;
        static constexpr index_type maxSize = std::numeric_limits<index_type>::max();
        
        using const_iterator = base_type::const_iterator;
        using iterator = base_type::iterator;

        constexpr jstring() noexcept = default;
        constexpr jstring(const character_type character)
            : base_type(1, character)
        {}
        constexpr jstring(const character_type* const str)
            : base_type(str)
        {
            _correctSize();
        }
        constexpr jstring(const character_type* const str, const index_type count)
            : base_type(str, jutils::math::max(count, 0))
        {}
        constexpr jstring(const int32 count, const character_type character)
            : base_type(jutils::math::max(count, 0), character)
        {}
        constexpr jstring(const base_type& str)
            : base_type(str)
        {
            _correctSize();
        }
        constexpr jstring(base_type&& str) noexcept
            : base_type(std::move(str))
        {
            _correctSize();
        }
        constexpr jstring(const jstring& str) = default;
        constexpr jstring(jstring&& str) noexcept = default;
        constexpr ~jstring() = default;
        
        constexpr jstring& operator=(const character_type character)
        {
            base_type::operator=(character);
            return *this;
        }
        constexpr jstring& operator=(const character_type* const str)
        {
            base_type::operator=(str);
            _correctSize();
            return *this;
        }
        constexpr jstring& operator=(const base_type& str)
        {
            base_type::operator=(str);
            _correctSize();
            return *this;
        }
        constexpr jstring& operator=(base_type&& str) noexcept
        {
            base_type::operator=(std::move(str));
            _correctSize();
            return *this;
        }
        constexpr jstring& operator=(const jstring& str)
        {
            base_type::operator=(str);
            return *this;
        }
        constexpr jstring& operator=(jstring&& str) noexcept
        {
            base_type::operator=(std::move(static_cast<base_type&>(str)));
            return *this;
        }

        constexpr const base_type& toBase() const noexcept { return *this; }

        constexpr index_type getSize() const noexcept { return static_cast<index_type>(jutils::math::min(base_type::size(), maxSize)); }
        constexpr bool isEmpty() const noexcept { return base_type::empty(); }
        constexpr bool isValidIndex(const index_type index) const noexcept
        {
            return (index >= 0) && (static_cast<base_type::size_type>(index) < base_type::size());
        }

        constexpr iterator begin() noexcept { return base_type::begin(); }
        constexpr iterator end() noexcept { return base_type::end(); }
        
        constexpr const_iterator begin() const noexcept { return base_type::begin(); }
        constexpr const_iterator end() const noexcept { return base_type::end(); }

        constexpr character_type* getRawString() noexcept { return base_type::data(); }
        constexpr const character_type* getRawString() const noexcept { return base_type::c_str(); }

        constexpr character_type* operator*() noexcept { return getRawString(); }
        constexpr const character_type* operator*() const noexcept { return getRawString(); }

        constexpr character_type& get(const index_type index) noexcept
        {
            return base_type::operator[](index < 0 ? base_type::npos : index);
        }
        constexpr const character_type& get(const index_type index) const noexcept
        {
            return base_type::operator[](index < 0 ? base_type::npos : index);
        }
        constexpr character_type& operator[](const index_type index) noexcept { return get(index); }
        constexpr const character_type& operator[](const index_type index) const noexcept { return get(index); }

        constexpr index_type indexOf(const character_type character, const index_type startIndex = 0, 
            const index_type finishIndex = invalidIndex) const noexcept
        {
            const base_type::size_type index = base_type::find(character, jutils::math::max(startIndex, 0));
            if (index > static_cast<base_type::size_type>(maxSize))
            {
                return invalidIndex;
            }
            if ((finishIndex >= 0) && (static_cast<base_type::size_type>(finishIndex) < index))
            {
                return invalidIndex;
            }
            return index;
        }
        constexpr index_type indexOf(const character_type* const str, const index_type startIndex = 0, 
            const index_type finishIndex = invalidIndex) const noexcept
        {
            const base_type::size_type index = base_type::find(str, jutils::math::max(startIndex, 0));
            if (index > static_cast<base_type::size_type>(maxSize))
            {
                return invalidIndex;
            }
            if ((finishIndex >= 0) && (static_cast<base_type::size_type>(finishIndex) < index))
            {
                return invalidIndex;
            }
            return index;
        }
        constexpr index_type indexOf(const character_type* const str, const index_type strLength, const index_type startIndex, 
            const index_type finishIndex) const noexcept
        {
            const base_type::size_type index = base_type::find(str, jutils::math::max(0, startIndex), jutils::math::max(strLength, 0));
            if (index > static_cast<base_type::size_type>(maxSize))
            {
                return invalidIndex;
            }
            if ((finishIndex >= 0) && (static_cast<base_type::size_type>(finishIndex) < index))
            {
                return invalidIndex;
            }
            return index;
        }
        constexpr index_type indexOf(const base_type& str, const index_type startIndex = 0, 
            const index_type finishIndex = invalidIndex) const noexcept
        {
            return indexOf(str.c_str(), jutils::math::min(str.size(), maxSize), startIndex, finishIndex);
        }
        constexpr index_type indexOf(const jstring& str, const index_type startIndex = 0, 
            const index_type finishIndex = invalidIndex) const noexcept
        {
            return indexOf(*str, str.getSize(), startIndex, finishIndex);
        }
        
        constexpr bool contains(const character_type character, const index_type startIndex = 0, const index_type finishIndex = invalidIndex) const noexcept
        {
            return indexOf(character, startIndex, finishIndex) != invalidIndex;
        }
        constexpr bool contains(const character_type* const str, const index_type startIndex = 0, const index_type finishIndex = invalidIndex) const noexcept
        {
            return indexOf(str, startIndex, finishIndex) != invalidIndex;
        }
        constexpr bool contains(const character_type* const str, const index_type strLength, const index_type startIndex, const int32 finishIndex) const noexcept
        {
            return indexOf(str, strLength, startIndex, finishIndex) != invalidIndex;
        }
        constexpr bool contains(const std::string& str, const index_type startIndex = 0, const index_type finishIndex = invalidIndex) const noexcept
        {
            return indexOf(str.c_str(), jutils::math::min(str.size(), maxSize), startIndex, finishIndex) != invalidIndex;
        }
        constexpr bool contains(const jstring& str, const index_type startIndex = 0, const index_type finishIndex = invalidIndex) const noexcept
        {
            return indexOf(*str, str.getSize(), startIndex, finishIndex) != invalidIndex;
        }
        
        constexpr jstring substr(const index_type startIndex = 0, const index_type length = invalidIndex) const noexcept
        {
            if (length == 0)
            {
                return {};
            }
            const index_type size = getSize();
            const index_type offset = jutils::math::max(startIndex, 0);
            if (offset >= size)
            {
                return {};
            }
            const index_type count = length < 0 ? size - offset : jutils::math::min(size - offset, length);
            return base_type::substr(offset, count);
        }

        constexpr int compare(const character_type* const str) const noexcept { return base_type::compare(str); }
        constexpr int compare(const std::string& str) const noexcept { return base_type::compare(str); }
        constexpr int compare(const jstring& str) const noexcept { return base_type::compare(*str); }
        
        constexpr void reserve(const index_type size) { base_type::reserve(jutils::math::max(size, 0)); }
        constexpr void resize(const index_type size, const character_type character = character_type()) { base_type::resize(jutils::math::max(size, 0), character); }

        constexpr jstring& assign(const character_type character) { return this->operator=(character); }
        constexpr jstring& assign(const character_type* const str) { return this->operator=(str); }
        constexpr jstring& assign(const character_type* const str, const index_type strLength)
        {
            if (strLength <= 0)
            {
                clear();
            }
            else
            {
                base_type::assign(str, strLength);
                _correctSize();
            }
            return *this;
        }
        constexpr jstring& assign(const base_type& str) { return this->operator=(str); }
        constexpr jstring& assign(base_type&& str) noexcept { return this->operator=(std::move(str)); }
        constexpr jstring& assign(const jstring& str) { return this->operator=(str); }
        constexpr jstring& assign(jstring&& str) noexcept { return this->operator=(std::move(str)); }

        constexpr jstring& operator+=(const character_type character)
        {
            if (getSize() != maxSize)
            {
                base_type::operator+=(character);
            }
            return *this;
        }
        constexpr jstring& operator+=(const character_type* const str)
        {
            base_type::operator+=(str);
            _correctSize();
            return *this;
        }
        constexpr jstring& operator+=(const std::string& str)
        {
            base_type::operator+=(str);
            _correctSize();
            return *this;
        }
        constexpr jstring& operator+=(const jstring& str) { return this->operator+=(str.toBase()); }

        constexpr jstring& add(const character_type character) { return this->operator+=(character); }
        constexpr jstring& add(const character_type* const str) { return this->operator+=(str); }
        constexpr jstring& add(const character_type* const str, const index_type strLength)
        {
            if (strLength > 0)
            {
                base_type::append(str, strLength);
                _correctSize();
            }
            return *this;
        }
        constexpr jstring& add(const std::string& str) { return this->operator+=(str); }
        constexpr jstring& add(const jstring& str) { return this->operator+=(str); }

        constexpr jstring& addAt(const index_type index, const character_type character)
        {
            if ((getSize() < maxSize) && jutils::math::isWithin(index, 0, getSize()))
            {
                base_type::insert(index, 1, character);
                _correctSize();
            }
            return *this;
        }
        constexpr jstring& addAt(const index_type index, const character_type* const str)
        {
            if (jutils::math::isWithin(index, 0, getSize()))
            {
                base_type::insert(index, str);
                _correctSize();
            }
            return *this;
        }
        constexpr jstring& addAt(const index_type index, const character_type* const str, const index_type strLength)
        {
            if (jutils::math::isWithin(index, 0, getSize()))
            {
                base_type::insert(index, str, strLength);
                _correctSize();
            }
            return *this;
        }
        constexpr jstring& addAt(const index_type index, const std::string& str)
        {
            if (jutils::math::isWithin(index, 0, getSize()))
            {
                base_type::insert(index, str);
                _correctSize();
            }
            return *this;
        }
        constexpr jstring& addAt(const index_type index, const jstring& str) { return addAt(index, str.toBase()); }
        
        constexpr void removeAt(const index_type index, const index_type count = 1) noexcept
        {
            if (isValidIndex(index) && (count > 0))
            {
                base_type::erase(index, count);
            }
        }
        constexpr void clear() noexcept { base_type::clear(); }
        
        constexpr uint64 hash() const noexcept { return jutils::math::hash::crc64(getRawString(), getSize()); }
        
    private:

        constexpr void _correctSize() noexcept
        {
            if (base_type::size() > static_cast<base_type::size_type>(maxSize))
            {
                base_type::resize(maxSize);
            }
        }
    };

    namespace string
    {
        template<typename T>
        struct formatter : std::bool_constant<std::is_arithmetic_v<std::remove_cvref_t<T>>>
        {
            static jstring format(T value) noexcept { return std::to_string(value); }
        };
        template<typename T>
        using has_jstring_formatter = std::bool_constant<formatter<T>::value>;
        template<typename T>
        constexpr bool has_jstring_formatter_v = has_jstring_formatter<T>::value;

        template<typename T> requires has_jstring_formatter_v<T>
        constexpr jstring toString(T value) noexcept { return formatter<T>::format(value); }

        template<typename T, typename Ctx = std::format_context>
        struct has_formatter
        {
        private:
            template<typename T1, typename Ctx1> requires requires(T1& val, Ctx1& ctx)
            {
                std::declval<typename Ctx1::template formatter_type<std::remove_cvref_t<T1>>>().format(val, ctx);
            }
            static constexpr bool _has_formatter(int32) noexcept { return true; }
            template<typename, typename>
            static constexpr bool _has_formatter(int8) noexcept { return false; }
        public:
            static constexpr bool value = _has_formatter<T, Ctx>(0);
        };
        template<typename T, typename Ctx = std::format_context>
        constexpr bool has_formatter_v = has_formatter<T, Ctx>::value;

        inline jstring format(const char* str) { return str; }
        template<typename... Args> requires (has_formatter_v<Args> && ...)
        jstring format(const std::format_string<Args...> formatStr, Args&&... args)
        {
            return std::format(formatStr, std::forward<Args>(args)...);
        }
    }
    
    constexpr jstring operator+(const jstring& str1, const jstring::character_type character) { return str1.toBase() + character; }
    constexpr jstring operator+(const jstring::character_type character, const jstring& str1) { return character + str1.toBase(); }
    constexpr jstring operator+(const jstring& str1, const jstring& str2) { return str1.toBase() + str2.toBase(); }
    constexpr jstring operator+(const jstring& str1, const jstring::character_type* str2) { return str1.toBase() + str2; }
    constexpr jstring operator+(const jstring::character_type* const str1, const jstring& str2) { return str1 + str2.toBase(); }

    constexpr bool operator==(const jstring& str1, const jstring& str2) noexcept { return str1.compare(str2) == 0; }
    constexpr bool operator==(const jstring& str1, const jstring::character_type* const str2) noexcept { return str1.compare(str2) == 0; }
    constexpr bool operator==(const jstring::character_type* const str1, const jstring& str2) noexcept { return str2 == str1; }

    constexpr bool operator!=(const jstring& str1, const jstring& str2) noexcept { return !(str1 == str2); }
    constexpr bool operator!=(const jstring& str1, const jstring::character_type* const str2) noexcept { return !(str1 == str2); }
    constexpr bool operator!=(const jstring::character_type* const str1, const jstring& str2) noexcept { return !(str1 == str2); }

    constexpr bool operator<(const jstring& str1, const jstring& str2) noexcept { return str1.compare(str2) < 0; }
    constexpr bool operator<(const jstring& str1, const jstring::character_type* const str2) noexcept { return str1.compare(str2) < 0; }
    constexpr bool operator<(const jstring::character_type* const str1, const jstring& str2) noexcept { return str2.compare(str1) > 0; }
    
    constexpr bool operator>(const jstring& str1, const jstring& str2) noexcept { return str1.compare(str2) > 0; }
    constexpr bool operator>(const jstring& str1, const jstring::character_type* const str2) noexcept { return str1.compare(str2) > 0; }
    constexpr bool operator>(const jstring::character_type* const str1, const jstring& str2) noexcept { return str2.compare(str1) < 0; }
}

template<>
struct jutils::string::formatter<const char*> : std::true_type
{
    static constexpr jutils::jstring format(const char* value) noexcept { return value; }
};
template<>
struct jutils::string::formatter<std::string> : std::true_type
{
    static constexpr jutils::jstring format(std::string value) noexcept { return value; }
};
template<>
struct jutils::string::formatter<jutils::jstring> : std::true_type
{
    static constexpr jutils::jstring format(jutils::jstring value) noexcept { return value; }
};

template<typename CharT>
struct std::formatter<jutils::jstring, CharT> : std::formatter<const jutils::jstring::character_type*, CharT>
{
    template<typename FormatContext>
    auto format(const jutils::jstring& str, FormatContext& ctx)
    {
        return std::formatter<const jutils::jstring::character_type*, CharT>::format(*str, ctx);
    }
};

#define JUTILS_JSTRING_FORMATTER(type, funcName)                                                \
template<> struct jutils::string::formatter<std::remove_cvref_t< type >> : std::true_type       \
    { static constexpr jutils::jstring format(const type& value) { return funcName(value); } };
#define JUTILS_JSTRING_FORMATTER_NOT_CONSTEXPR(type, funcName)                               \
template<> struct jutils::string::formatter<std::remove_cvref_t< type >> : std::true_type   \
    { static jutils::jstring format(const type& value) { return funcName(value); } };

#define JUTILS_STD_FORMATTER(type, funcName)                                                    \
template<typename CharT>                                                                        \
struct std::formatter<type, CharT> : std::formatter<decltype(funcName(type())), CharT>          \
{                                                                                               \
    template<typename FormatContext>                                                            \
    auto format(const type& value, FormatContext& ctx)                                          \
    {                                                                                           \
        return std::formatter<decltype(funcName(type())), CharT>::format(funcName(value), ctx); \
    }                                                                                           \
};

#define JUTILS_STRING_FORMATTER(type, funcName) \
    JUTILS_JSTRING_FORMATTER(type, funcName)    \
    JUTILS_STD_FORMATTER(type, funcName)
#define JUTILS_STRING_FORMATTER_NOT_CONSTEXPR(type, funcName)   \
    JUTILS_JSTRING_FORMATTER_NOT_CONSTEXPR(type, funcName)      \
    JUTILS_STD_FORMATTER(type, funcName)

#define JSTR(str) str
#define TO_JSTR(value) jutils::string::toString(value)
#define JSTR_FORMAT(formatStr, ...) jutils::string::format(formatStr, __VA_ARGS__)
