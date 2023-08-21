// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jset_hash.h"
#include "jstring.h"

#include <mutex>
#include <shared_mutex>

namespace jutils
{
    class jstring_hash_table
    {
    public:

        using index_type = jarray_index_type;

        static void CreateInstance() noexcept
        {
            if (Instance == nullptr)
            {
                Instance = new jstring_hash_table();
            }
        }
        static jstring_hash_table* GetInstanse() noexcept
        {
            CreateInstance();
            return Instance;
        }
        static void ClearInstance() noexcept
        {
            if (Instance != nullptr)
            {
                delete Instance;
                Instance = nullptr;
            }
        }

        index_type addOrFind(const jstring& str)
        {
            if (str.isEmpty())
            {
                return jarrayInvalidIndex;
            }

            index_type strIndex;
            {
                strings_table_entry entry = { str };
                std::scoped_lock lock(rwMutex);
                const strings_table_entry* entryPtr = stringsTable.find(entry);
                if (entryPtr != nullptr)
                {
                    strIndex = entryPtr->pointerIndex;
                }
                else
                {
                    strIndex = entry.pointerIndex = stringPointers.getSize();
                    stringPointers.add( &stringsTable.add(std::move(entry)) );
                }
            }
            return strIndex;
        }
        bool contains(const index_type index) const noexcept
        {
            std::shared_lock lock(rwMutex);
            return stringPointers.isValidIndex(index);
        }
        jstring get(const index_type index) const noexcept
        {
            {
                std::shared_lock lock(rwMutex);
                if (stringPointers.isValidIndex(index))
                {
                    return stringPointers[index]->string;
                }
            }
            return {};
        }

    private:

        inline static jstring_hash_table* Instance = nullptr;

        jstring_hash_table() noexcept = default;
        ~jstring_hash_table() noexcept = default;

        struct strings_table_entry
        {
            constexpr strings_table_entry() noexcept = default;
            constexpr strings_table_entry(const jstring& str)
                : string(str)
            {}
            constexpr strings_table_entry(jstring&& str) noexcept
                : string(std::move(str))
            {}
            constexpr strings_table_entry(const strings_table_entry&) = default;
            constexpr strings_table_entry(strings_table_entry&&) noexcept = default;

            constexpr strings_table_entry& operator=(const strings_table_entry&) = default;
            constexpr strings_table_entry& operator=(strings_table_entry&&) noexcept = default;

            jstring string;
            index_type pointerIndex = -1;

            constexpr uint64 hash() const noexcept { return math::hash::getHash(string); }

            constexpr bool operator==(const jstring& str) const noexcept { return string == str; }
            constexpr bool operator==(const strings_table_entry& entry) const noexcept { return operator==(entry.string); }

            constexpr bool operator!=(const jstring& str) const noexcept { return !operator==(str); }
            constexpr bool operator!=(const strings_table_entry& entry) const noexcept { return operator!=(entry.string); }
        };
        
        jset_hash<strings_table_entry> stringsTable;
        jarray<const strings_table_entry*> stringPointers;
        mutable std::shared_mutex rwMutex;
    };

    class jstringID
    {
    public:
        constexpr jstringID() noexcept = default;
        jstringID(const jstring::character_type* const str)
            : jstringID(jstring(str))
        {}
        jstringID(const jstring& str)
        {
            pointerIndex = jstring_hash_table::GetInstanse()->addOrFind(str);
        }
        constexpr jstringID(const jstringID&) noexcept = default;

        constexpr jstringID& operator=(const jstringID&) noexcept = default;

        constexpr bool isValid() const noexcept { return pointerIndex >= 0; }
        jstring toString() const noexcept { return jstring_hash_table::GetInstanse()->get(pointerIndex); }

        constexpr bool operator==(const jstringID& strID) const noexcept { return isValid() && (pointerIndex == strID.pointerIndex); }
        constexpr bool operator!=(const jstringID& strID) const noexcept { return !operator==(strID); }

        constexpr bool operator<(const jstringID& strID) const noexcept { return isValid() && (pointerIndex < strID.pointerIndex); }
        constexpr bool operator>(const jstringID& strID) const noexcept { return isValid() && (pointerIndex > strID.pointerIndex); }

    private:

        jstring_hash_table::index_type pointerIndex = -1;
    };

    constexpr jstringID jstringID_NONE = jstringID();
}

template<>
struct jutils::string::formatter<std::remove_cvref_t< jutils::jstringID >> : std::true_type
{
    static jutils::jstring format(const jutils::jstringID& value) noexcept { return value.toString(); }
};
#if defined(JUTILS_USE_FMT)
    template<>
    struct fmt::formatter<jutils::jstringID> : fmt::formatter<const char*>
    {
        template<typename FormatContext>
        auto format(const jutils::jstringID& value, FormatContext& ctx) const
        {
            return fmt::formatter<const char*>::format(*value.toString(), ctx);
        }
    };
#else
    template<>
    struct std::formatter<jutils::jstringID> : std::formatter<const char*>
    {
        template<typename FormatContext>
        auto format(const jutils::jstringID& value, FormatContext& ctx) const
        {
            return std::formatter<const char*>::format(*value.toString(), ctx);
        }
    };
#endif
