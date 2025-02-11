// Copyright © 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "hash_set.h"
#include "format.h"
#include <mutex>
#include <shared_mutex>

namespace jutils
{
    class jstring_hash_table
    {
    public:

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

        index_type addOrFind(const string& str)
        {
            if (str.isEmpty())
            {
                return index_invalid;
            }

            index_type strIndex;
            {
                entry e = { str };
                std::scoped_lock lock(rwMutex);
                const entry* entryPtr = stringsTable.find(e);
                if (entryPtr != nullptr)
                {
                    strIndex = entryPtr->pointerIndex;
                }
                else
                {
                    strIndex = e.pointerIndex = stringPointers.getSize();
                    stringPointers.add( &stringsTable.add(std::move(e)) );
                }
            }
            return strIndex;
        }
        bool contains(const index_type index) const noexcept
        {
            std::shared_lock lock(rwMutex);
            return stringPointers.isValidIndex(index);
        }
        string get(const index_type index) const noexcept
        {
            {
                std::shared_lock lock(rwMutex);
                if (stringPointers.isValidIndex(index))
                {
                    return stringPointers[index]->stringValue;
                }
            }
            return {};
        }

    private:

        inline static jstring_hash_table* Instance = nullptr;

        jstring_hash_table() noexcept = default;
        ~jstring_hash_table() noexcept = default;
        
        class entry
        {
        public:
            entry() noexcept = default;
            entry(const string& str)
                : stringValue(str)
            {}
            entry(string&& str) noexcept
                : stringValue(std::move(str))
            {}
            entry(const entry&) = default;
            entry(entry&&) noexcept = default;
            ~entry() = default;

            entry& operator=(const entry&) = default;
            entry& operator=(entry&&) noexcept = default;

            string stringValue;
            index_type pointerIndex = index_invalid;

            [[nodiscard]] bool operator==(const string& str) const noexcept { return stringValue == str; }
            [[nodiscard]] bool operator==(const entry& entry) const noexcept { return operator==(entry.stringValue); }

            [[nodiscard]] bool operator!=(const string& str) const noexcept { return !operator==(str); }
            [[nodiscard]] bool operator!=(const entry& entry) const noexcept { return operator!=(entry.stringValue); }

            struct hash
            {
                [[nodiscard]] jutils::math::hash_t operator()(const entry& entry) const noexcept
                    { return jutils::math::hash<string>{}(entry.stringValue); }
            };
        };
        
        hash_set<entry, entry::hash> stringsTable;
        vector<const entry*> stringPointers;
        mutable std::shared_mutex rwMutex;
    };

    class stringID
    {
    public:
        constexpr stringID() noexcept = default;
        stringID(const string::char_type* const str)
            : stringID(string(str))
        {}
        stringID(const string& str)
            : pointerIndex(jstring_hash_table::GetInstanse()->addOrFind(str))
        {}
        constexpr stringID(stringID&&) noexcept = default;
        constexpr stringID(const stringID&) noexcept = default;
        constexpr ~stringID() = default;

        constexpr stringID& operator=(stringID&&) noexcept = default;
        constexpr stringID& operator=(const stringID&) noexcept = default;

        [[nodiscard]] constexpr bool isValid() const noexcept { return pointerIndex != index_invalid; }
        [[nodiscard]] string toString() const noexcept { return jstring_hash_table::GetInstanse()->get(pointerIndex); }

        [[nodiscard]] constexpr bool operator==(const stringID& strID) const noexcept { return isValid() && (pointerIndex == strID.pointerIndex); }
        [[nodiscard]] constexpr bool operator!=(const stringID& strID) const noexcept { return !operator==(strID); }

        [[nodiscard]] constexpr bool operator<(const stringID& strID) const noexcept { return isValid() && (pointerIndex < strID.pointerIndex); }
        [[nodiscard]] constexpr bool operator>(const stringID& strID) const noexcept { return isValid() && (pointerIndex > strID.pointerIndex); }

    private:

        index_type pointerIndex = index_invalid;
    };

    template<>
    struct string_formatter<jutils::remove_cvref_t< stringID >> : std::true_type
    {
        [[nodiscard]] static string format(const stringID& value) noexcept { return value.toString(); }
    };
}

namespace JUTILS_FORMAT_NAMESPACE
{
    template<>
    struct formatter<jutils::stringID> : formatter<jutils::string>
    {
        template<typename FormatContext>
        auto format(const jutils::stringID& value, FormatContext& ctx) const
        {
            return formatter<jutils::string>::format(value.toString(), ctx);
        }
    };
}

