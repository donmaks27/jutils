// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "juid.h"

namespace jutils
{
    template<typename T, typename UIDType>
    class jpool;

    template<typename UIDType = uint32>
    class jpool_pointer
    {
        template<typename T>
        friend class jpool<T, UIDType>;

    public:
        using uid_type = UIDType;
    private:
        constexpr uid_type invalidUID = juid<uid_type>::invalidUID;

    public:
        constexpr jpool_pointer() = default;
        constexpr jpool_pointer(nullptr_t) : jpool_pointer() {}
        constexpr jpool_pointer(const jpool_pointer&) = default;
    private:
        constexpr jpool_pointer(const int32 segment, const uint32 node, const uint32 uid)
            : segmentIndex(segment), nodeIndex(node), UID(uid)
        {}
    public:

        constexpr bool isValid() const { return (UID != invalidUID) && (segmentIndex >= 0); }

        constexpr bool operator==(const jpool_pointer& pointer) const
        {
            if (!isValid())
            {
                return !pointer.isValid();
            }
            return (segmentIndex == pointer.segmentIndex) && (nodeIndex == pointer.nodeIndex) && (UID == pointer.UID);
        }
        constexpr bool operator!=(const jpool_pointer& pointer) const { return !this->operator==(pointer); }

        constexpr bool operator==(nullptr_t) const { return !isValid(); }
        constexpr bool operator!=(nullptr_t) const { return isValid(); }

    private:

        int32 segmentIndex = -1;
        uint32 nodeIndex = 0;
        uid_type UID = invalidUID;
    };
}
