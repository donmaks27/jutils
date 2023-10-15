// Copyright © 2021-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "base_types.h"
#include "math/hash.h"
#include "jarray.h"

#if defined(JUTILS_USE_FMT)
    #include <fmt/format.h>
#else
    #include <format>
#endif
#include <string>

namespace jutils
{
    class jstring
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
            : _internalString(1, character)
        {}
        constexpr jstring(const character_type* str)
            : _internalString(str)
        {
            _correctSize();
        }
        constexpr jstring(const character_type* const str, const index_type count)
            : _internalString(str, jutils::math::max(count, 0))
        {}
        constexpr jstring(const int32 count, const character_type character)
            : _internalString(jutils::math::max(count, 0), character)
        {}
        constexpr jstring(const base_type& str)
            : _internalString(str)
        {
            _correctSize();
        }
        constexpr jstring(base_type&& str) noexcept
            : _internalString(std::move(str))
        {
            _correctSize();
        }
        constexpr jstring(const jstring& str) = default;
        constexpr jstring(jstring&& str) noexcept = default;
        constexpr ~jstring() = default;
        
        constexpr jstring& operator=(const character_type character)
        {
            _internalString = character;
            return *this;
        }
        constexpr jstring& operator=(const character_type* const str)
        {
            _internalString = str;
            _correctSize();
            return *this;
        }
        constexpr jstring& operator=(const base_type& str)
        {
            _internalString = str;
            _correctSize();
            return *this;
        }
        constexpr jstring& operator=(base_type&& str) noexcept
        {
            _internalString = std::move(str);
            _correctSize();
            return *this;
        }
        constexpr jstring& operator=(const jstring&) = default;
        constexpr jstring& operator=(jstring&&) noexcept = default;

        constexpr const base_type& toBase() const noexcept { return _internalString; }

        constexpr index_type getSize() const noexcept { return static_cast<index_type>(jutils::math::min(_internalString.size(), maxSize)); }
        constexpr bool isEmpty() const noexcept { return _internalString.empty(); }
        constexpr bool isValidIndex(const index_type index) const noexcept
        {
            return (index >= 0) && (static_cast<base_type::size_type>(index) < _internalString.size());
        }

        constexpr iterator begin() noexcept { return _internalString.begin(); }
        constexpr iterator end() noexcept { return _internalString.end(); }
        
        constexpr const_iterator begin() const noexcept { return _internalString.begin(); }
        constexpr const_iterator end() const noexcept { return _internalString.end(); }

        constexpr character_type* getData() noexcept { return _internalString.data(); }
        constexpr const character_type* getData() const noexcept { return _internalString.c_str(); }

        constexpr character_type* operator*() noexcept { return getData(); }
        constexpr const character_type* operator*() const noexcept { return getData(); }

        constexpr character_type& get(const index_type index) noexcept
        {
            return _internalString[index < 0 ? base_type::npos : index];
        }
        constexpr const character_type& get(const index_type index) const noexcept
        {
            return _internalString[index < 0 ? base_type::npos : index];
        }
        constexpr character_type& operator[](const index_type index) noexcept { return get(index); }
        constexpr const character_type& operator[](const index_type index) const noexcept { return get(index); }

        constexpr index_type indexOf(const character_type character, const index_type startIndex = 0, 
            const index_type finishIndex = invalidIndex) const noexcept
        {
            const base_type::size_type index = _internalString.find(character, jutils::math::max(startIndex, 0));
            if (index > static_cast<base_type::size_type>(maxSize))
            {
                return invalidIndex;
            }
            if ((finishIndex >= 0) && (static_cast<base_type::size_type>(finishIndex) < index))
            {
                return invalidIndex;
            }
            return static_cast<index_type>(index);
        }
        constexpr index_type indexOf(const character_type* const str, const index_type startIndex = 0, 
            const index_type finishIndex = invalidIndex) const noexcept
        {
            const base_type::size_type index = _internalString.find(str, jutils::math::max(startIndex, 0));
            if (index > static_cast<base_type::size_type>(maxSize))
            {
                return invalidIndex;
            }
            if ((finishIndex >= 0) && (static_cast<base_type::size_type>(finishIndex) < index))
            {
                return invalidIndex;
            }
            return static_cast<index_type>(index);
        }
        constexpr index_type indexOf(const character_type* const str, const index_type strLength, const index_type startIndex, 
            const index_type finishIndex) const noexcept
        {
            const base_type::size_type index = _internalString.find(str, jutils::math::max(0, startIndex), jutils::math::max(strLength, 0));
            if (index > static_cast<base_type::size_type>(maxSize))
            {
                return invalidIndex;
            }
            if ((finishIndex >= 0) && (static_cast<base_type::size_type>(finishIndex) < index))
            {
                return invalidIndex;
            }
            return static_cast<index_type>(index);
        }
        constexpr index_type indexOf(const base_type& str, const index_type startIndex = 0, 
            const index_type finishIndex = invalidIndex) const noexcept
        {
            return indexOf(str.c_str(), jutils::math::min(static_cast<index_type>(str.size()), maxSize), startIndex, finishIndex);
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
            return indexOf(str.c_str(), jutils::math::min(static_cast<index_type>(str.size()), maxSize), startIndex, finishIndex) != invalidIndex;
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
            return _internalString.substr(offset, count);
        }

        constexpr jarray<jstring> split(const character_type delimiter) const { return _split(delimiter, 1); }
        constexpr jarray<jstring> split(const character_type* delimiter) const
        {
            return _split(delimiter, static_cast<index_type>(std::char_traits<character_type>::length(delimiter)));
        }
        constexpr jarray<jstring> split(const character_type* delimiter, const index_type delimiterLength) const
        {
            return _split(delimiter, delimiterLength);
        }
        constexpr jarray<jstring> split(const std::string& delimiter) const
        {
            return _split(delimiter, static_cast<index_type>(delimiter.size()));
        }
        constexpr jarray<jstring> split(const jstring& delimiter) const { return _split(delimiter, delimiter.getSize()); }

        constexpr int compare(const character_type* const str) const noexcept { return _internalString.compare(str); }
        constexpr int compare(const std::string& str) const noexcept { return _internalString.compare(str); }
        constexpr int compare(const jstring& str) const noexcept { return _internalString.compare(*str); }
        
        constexpr void reserve(const index_type size) { _internalString.reserve(jutils::math::max(size, 0)); }
        constexpr void resize(const index_type size, const character_type character = character_type())
        {
            _internalString.resize(jutils::math::max(size, 0), character);
        }

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
                _internalString.assign(str, strLength);
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
                _internalString += character;
            }
            return *this;
        }
        constexpr jstring& operator+=(const character_type* const str)
        {
            _internalString += str;
            _correctSize();
            return *this;
        }
        constexpr jstring& operator+=(const std::string& str)
        {
            _internalString += str;
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
                _internalString.append(str, strLength);
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
                _internalString.insert(index, 1, character);
                _correctSize();
            }
            return *this;
        }
        constexpr jstring& addAt(const index_type index, const character_type* const str)
        {
            if (jutils::math::isWithin(index, 0, getSize()))
            {
                _internalString.insert(index, str);
                _correctSize();
            }
            return *this;
        }
        constexpr jstring& addAt(const index_type index, const character_type* const str, const index_type strLength)
        {
            if (jutils::math::isWithin(index, 0, getSize()))
            {
                _internalString.insert(index, str, strLength);
                _correctSize();
            }
            return *this;
        }
        constexpr jstring& addAt(const index_type index, const std::string& str)
        {
            if (jutils::math::isWithin(index, 0, getSize()))
            {
                _internalString.insert(index, str);
                _correctSize();
            }
            return *this;
        }
        constexpr jstring& addAt(const index_type index, const jstring& str) { return addAt(index, str.toBase()); }
        
        constexpr void removeAt(const index_type index, const index_type count = 1) noexcept
        {
            if (isValidIndex(index) && (count > 0))
            {
                _internalString.erase(index, count);
            }
        }
        constexpr void clear() noexcept { _internalString.clear(); }
        
        constexpr uint64 hash() const noexcept { return jutils::math::hash::crc64(getData(), getSize()); }
        
    private:

        std::string _internalString;

        constexpr std::string& _base() { return _internalString; }

        constexpr void _correctSize() noexcept
        {
            if (_internalString.size() > static_cast<base_type::size_type>(maxSize))
            {
                _internalString.resize(maxSize);
            }
        }

        template<typename T>
        constexpr jarray<jstring> _split(T delimiter, const index_type delimiterSize) const
        {
            if (isEmpty())
            {
                return {};
            }
            jarray<jstring> result;
            jstring restStr = *this;
            while (true)
            {
                const index_type index = restStr.indexOf(delimiter);
                if (index != invalidIndex)
                {
                    if (index > 0)
                    {
                        result.add(restStr.substr(0, index));
                    }
                    restStr = restStr.substr(index + delimiterSize);
                }
                else
                {
                    result.add(std::move(restStr));
                    return result;
                }
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

        template<typename T> requires has_jstring_formatter_v<T>
        constexpr jstring join(const jarray<T>& values, const jstring& separator = "")
        {
            if (values.isEmpty())
            {
                return "";
            }
            jstring result = toString(values[0]);
            for (typename jarray<T>::size_type index = 1; index < values.getSize(); index++)
            {
                result += separator + toString(values[index]);
            }
            return result;
        }

#if defined(JUTILS_USE_FMT)
        template<typename T, typename Ctx = fmt::format_context>
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
        template<typename T, typename Ctx = fmt::format_context>
        constexpr bool has_formatter_v = has_formatter<T, Ctx>::value;

        inline jstring format(const char* str) { return str; }
        template<typename... Args> requires (has_formatter_v<Args> && ...)
        jstring format(const fmt::format_string<Args...> formatStr, Args&&... args)
        {
            return fmt::format(formatStr, std::forward<Args>(args)...);
        }
#else
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
#endif
    }
    
    constexpr jstring operator+(const jstring& str1, const jstring::character_type character) { return str1.toBase() + character; }
    constexpr jstring operator+(jstring&& str1, const jstring::character_type character) { return std::move(str1 += character); }
    constexpr jstring operator+(const jstring::character_type character, const jstring& str1) { return character + str1.toBase(); }
    constexpr jstring operator+(const jstring::character_type character, jstring&& str1) { return std::move(str1.addAt(0, character)); }
    constexpr jstring operator+(const jstring& str1, const jstring& str2) { return str1.toBase() + str2.toBase(); }
    constexpr jstring operator+(const jstring& str1, jstring&& str2) { return std::move(str2.addAt(0, str1)); }
    constexpr jstring operator+(jstring&& str1, const jstring& str2) { return std::move(str1 += str2); }
    constexpr jstring operator+(const jstring& str1, const jstring::character_type* str2) { return str1.toBase() + str2; }
    constexpr jstring operator+(jstring&& str1, const jstring::character_type* str2) { return std::move(str1 += str2); }
    constexpr jstring operator+(const jstring::character_type* const str1, const jstring& str2) { return str1 + str2.toBase(); }
    constexpr jstring operator+(const jstring::character_type* const str1, jstring&& str2) { return std::move(str2.addAt(0, str1)); }

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

#if defined(JUTILS_USE_FMT)
    template<>
    struct fmt::formatter<jutils::jstring> : fmt::formatter<const jutils::jstring::character_type*>
    {
        template<typename FormatContext>
        auto format(const jutils::jstring& str, FormatContext& ctx) const
        {
            return fmt::formatter<const jutils::jstring::character_type*>::format(*str, ctx);
        }
    };
    #define JUTILS_STRING_FORMATTER(type, funcName)                                                 \
    template<> struct jutils::string::formatter<std::remove_cvref_t< type >> : std::true_type       \
        { static constexpr jutils::jstring format(const type& value) { return funcName(value); } }; \
    template<>                                                                                      \
    struct fmt::formatter<type> : fmt::formatter<decltype(funcName(type()))>                        \
    {                                                                                               \
        template<typename FormatContext>                                                            \
        auto format(const type& value, FormatContext& ctx) const                                    \
        {                                                                                           \
            return fmt::formatter<decltype(funcName(type()))>::format(funcName(value), ctx);        \
        }                                                                                           \
    };
#else
    template<>
    struct std::formatter<jutils::jstring> : std::formatter<const jutils::jstring::character_type*>
    {
        template<typename FormatContext>
        auto format(const jutils::jstring& str, FormatContext& ctx) const
        {
            return std::formatter<const jutils::jstring::character_type*>::format(*str, ctx);
        }
    };
    #define JUTILS_STRING_FORMATTER(type, funcName)                                                 \
    template<> struct jutils::string::formatter<std::remove_cvref_t< type >> : std::true_type       \
        { static constexpr jutils::jstring format(const type& value) { return funcName(value); } }; \
    template<>                                                                                      \
    struct std::formatter<type> : std::formatter<decltype(funcName(type()))>                        \
    {                                                                                               \
        template<typename FormatContext>                                                            \
        auto format(const type& value, FormatContext& ctx) const                                    \
        {                                                                                           \
            return std::formatter<decltype(funcName(type()))>::format(funcName(value), ctx);        \
        }                                                                                           \
    };
#endif

#define JSTR(str) str
#define TO_JSTR(value) jutils::string::toString(value)
#define JSTR_FORMAT(formatStr, ...) jutils::string::format(formatStr __VA_OPT__(,) __VA_ARGS__)
