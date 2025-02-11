// Copyright © 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "math/hash.h"
#include "format.h"
#include <unordered_set>
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

        std::size_t addOrFind(const std::string& str)
        {
            if (str.empty())
            {
                return index_invalid;
            }

            std::size_t strIndex;
            {
                entry e = { str };
                std::scoped_lock lock(rwMutex);
                const auto entryPtr = stringsTable.find(e);
                if (entryPtr != stringsTable.end())
                {
                    strIndex = entryPtr->pointerIndex;
                }
                else
                {
                    strIndex = e.pointerIndex = stringPointers.size();
                    stringPointers.push_back( &*stringsTable.insert(std::move(e)).first );
                }
            }
            return strIndex;
        }
        bool contains(const std::size_t index) const noexcept
        {
            std::shared_lock lock(rwMutex);
            return index < stringPointers.size();
        }
        std::string get(const std::size_t index) const noexcept
        {
            {
                std::shared_lock lock(rwMutex);
                if (index < stringPointers.size())
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
            entry(const std::string& str)
                : stringValue(str)
            {}
            entry(std::string&& str) noexcept
                : stringValue(std::move(str))
            {}
            entry(const entry&) = default;
            entry(entry&&) noexcept = default;
            ~entry() = default;

            entry& operator=(const entry&) = default;
            entry& operator=(entry&&) noexcept = default;

            std::string stringValue;
            std::size_t pointerIndex = index_invalid;

            [[nodiscard]] bool operator==(const std::string& str) const noexcept { return stringValue == str; }
            [[nodiscard]] bool operator==(const entry& entry) const noexcept { return operator==(entry.stringValue); }

            [[nodiscard]] bool operator!=(const std::string& str) const noexcept { return !operator==(str); }
            [[nodiscard]] bool operator!=(const entry& entry) const noexcept { return operator!=(entry.stringValue); }

            struct hash
            {
                [[nodiscard]] jutils::math::hash_t operator()(const entry& entry) const noexcept
                    { return jutils::math::hash<std::string>{}(entry.stringValue); }
            };
        };
        
        std::unordered_set<entry, entry::hash> stringsTable;
        std::vector<const entry*> stringPointers;
        mutable std::shared_mutex rwMutex;
    };

    class stringID
    {
    public:
        constexpr stringID() noexcept = default;
        stringID(const char* const str)
            : stringID(std::string(str))
        {}
        stringID(const std::string& str)
            : pointerIndex(jstring_hash_table::GetInstanse()->addOrFind(str))
        {}
        constexpr stringID(stringID&&) noexcept = default;
        constexpr stringID(const stringID&) noexcept = default;
        constexpr ~stringID() = default;

        constexpr stringID& operator=(stringID&&) noexcept = default;
        constexpr stringID& operator=(const stringID&) noexcept = default;

        [[nodiscard]] constexpr bool isValid() const noexcept { return pointerIndex != index_invalid; }
        [[nodiscard]] std::string toString() const noexcept { return jstring_hash_table::GetInstanse()->get(pointerIndex); }

        [[nodiscard]] constexpr bool operator==(const stringID& strID) const noexcept { return isValid() && (pointerIndex == strID.pointerIndex); }
        [[nodiscard]] constexpr bool operator!=(const stringID& strID) const noexcept { return !operator==(strID); }

        [[nodiscard]] constexpr bool operator<(const stringID& strID) const noexcept { return isValid() && (pointerIndex < strID.pointerIndex); }
        [[nodiscard]] constexpr bool operator>(const stringID& strID) const noexcept { return isValid() && (pointerIndex > strID.pointerIndex); }

    private:

        std::size_t pointerIndex = index_invalid;
    };

    template<>
    struct string_formatter<jutils::remove_cvref_t< stringID >> : std::true_type
    {
        [[nodiscard]] static std::string format(const stringID& value) noexcept { return value.toString(); }
    };
}

namespace JUTILS_FORMAT_NAMESPACE
{
    template<>
    struct formatter<jutils::stringID> : formatter<std::string>
    {
        template<typename FormatContext>
        auto format(const jutils::stringID& value, FormatContext& ctx) const
        {
            return formatter<std::string>::format(value.toString(), ctx);
        }
    };
}

