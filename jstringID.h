// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jstring_hash_table.h"

namespace jutils
{
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

        static jstringID NONE;

        bool isValid() const { return jstring_hash_table::GetInstanse()->contains(pointerIndex); }
        jstring toString() const { return jstring_hash_table::GetInstanse()->get(pointerIndex); }

        bool operator==(const jstringID& strID) const { return isValid() && (pointerIndex == strID.pointerIndex); }
        bool operator!=(const jstringID& strID) const { return !operator==(strID); }

    private:

        jstring_hash_table::index_type pointerIndex = -1;
    };

    jstringID jstringID::NONE = jstringID();
}
