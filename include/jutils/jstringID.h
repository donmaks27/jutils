// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jhash_table_chain.h"
#include "jstring.h"

#include <shared_mutex>

namespace jutils
{
    class jstring_hash_table
    {
    public:

        using index_type = int32;

        static void CreateInstance()
        {
            if (Instance == nullptr)
            {
                Instance = new jstring_hash_table();
            }
        }
        static jstring_hash_table* GetInstanse()
        {
            CreateInstance();
            return Instance;
        }
        static void ClearInstance()
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
                return -1;
            }

            rwMutex.lock();
            hash_table_entry& entry = hashTable.put(str, str);
            if (!pointers.isValidIndex(entry.pointerIndex))
            {
                entry.pointerIndex = pointers.getSize();
                pointers.add(&entry);
            }
            rwMutex.unlock();
            return entry.pointerIndex;
        }
        bool contains(const index_type index) const
        {
            std::shared_lock lock(rwMutex);
            return pointers.isValidIndex(index);
        }
        jstring get(const index_type index) const
        {
            std::shared_lock lock(rwMutex);
            return contains(index) ? pointers[index]->string : jstring();
        }

    private:

        inline static jstring_hash_table* Instance = nullptr;

        jstring_hash_table() = default;
        ~jstring_hash_table() = default;

        struct hash_table_entry
        {
            hash_table_entry()
                : string(), pointerIndex(-1)
            {}
            hash_table_entry(const jstring& str)
                : string(str)
            {}
            hash_table_entry(jstring&& str)
                : string(std::move(str))
            {}
            hash_table_entry(const hash_table_entry&) = default;
            hash_table_entry(hash_table_entry&&) noexcept = default;

            hash_table_entry& operator=(const hash_table_entry&) = default;
            hash_table_entry& operator=(hash_table_entry&&) noexcept = default;

            jstring string;
            index_type pointerIndex = -1;

            uint64 hash() const { return math::hash::getHash(string); }

            bool operator==(const jstring& str) const { return string == str; }
            bool operator==(const hash_table_entry& entry) const { return operator==(entry.string); }

            bool operator!=(const jstring& str) const { return !operator==(str); }
            bool operator!=(const hash_table_entry& entry) const { return operator!=(entry.string); }
        };

        jhash_table_chain<hash_table_entry> hashTable = jhash_table_chain<hash_table_entry>(65536);
        jarray<hash_table_entry*> pointers;
        mutable std::shared_mutex rwMutex;
    };

    class jstringID
    {
    public:
        constexpr jstringID() = default;
        jstringID(const jstring::character_type* const str)
            : jstringID(jstring(str))
        {}
        jstringID(const jstring& str)
        {
            pointerIndex = jstring_hash_table::GetInstanse()->addOrFind(str);
        }
        constexpr jstringID(const jstringID&) = default;

        jstringID& operator=(const jstringID&) = default;

        bool isValid() const { return pointerIndex >= 0; }
        jstring toString() const { return jstring_hash_table::GetInstanse()->get(pointerIndex); }

        bool operator==(const jstringID& strID) const { return isValid() && (pointerIndex == strID.pointerIndex); }
        bool operator!=(const jstringID& strID) const { return !operator==(strID); }

        bool operator<(const jstringID& strID) const { return isValid() && (pointerIndex < strID.pointerIndex); }
        bool operator>(const jstringID& strID) const { return isValid() && (pointerIndex > strID.pointerIndex); }

    private:

        jstring_hash_table::index_type pointerIndex = -1;
    };

    constexpr jstringID jstringID_NONE = jstringID();

    inline jstring jstringID_to_jstring(const jstringID& stringID) { return stringID.toString(); }
}

JUTILS_STRING_FORMATTER(jutils::jstringID, jutils::jstringID_to_jstring);
