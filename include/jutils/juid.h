// Copyright © 2021-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "type_defines.h"
#include "type_checks.h"

#include <limits>

namespace jutils
{
    template<typename IdType = uint64, TEMPLATE_ENABLE(std::is_integral_v<IdType>)>
    class juid
    {
    public:

        using uid_type = IdType;
        static constexpr uid_type invalidUID = std::numeric_limits<uid_type>::min();
        static constexpr uid_type minUID = invalidUID + 1;
        static constexpr uid_type maxUID = std::numeric_limits<uid_type>::max();

        constexpr juid() = default;
        constexpr juid(const juid& value)
            : currentUID(value.currentUID)
        {}

        constexpr juid& operator=(const juid& value)
        {
            if (this != &value)
            {
                currentUID = value.currentUID;
            }
            return *this;
        }

        constexpr uid_type getUID() const { return currentUID; }
        constexpr uid_type getNextUID() const { return currentUID != maxUID ? currentUID + 1 : invalidUID; }
        constexpr uid_type generateUID()
        {
            currentUID = getNextUID();
            return getUID();
        }

        constexpr void reset()
        {
            currentUID = minUID;
        }

        constexpr operator uid_type() const { return getUID(); }

        constexpr bool operator==(const juid& uid) const { return getUID() == uid.getUID(); }
        constexpr bool operator!=(const juid& uid) const { return getUID() != uid.getUID(); }
        constexpr bool operator==(const uid_type uid) const { return getUID() == uid; }
        constexpr bool operator!=(const uid_type uid) const { return getUID() != uid; }

    private:

        uid_type currentUID = minUID;
    };
}
