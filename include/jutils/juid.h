// Copyright © 2021-2024 Leonov Maksim. All Rights Reserved.

#pragma once

#include "type_traits.h"

#include <limits>

namespace jutils
{
    JUTILS_TEMPLATE_CONDITION(std::is_integral_v<IdType>, typename IdType = uint64)
    class juid
    {
    public:

        using uid_type = IdType;
        static constexpr uid_type invalidUID = std::numeric_limits<uid_type>::min();

        constexpr juid() = default;
        constexpr juid(const juid& value)
            : currentUID(value.currentUID)
        {}

        constexpr juid& operator=(const juid& value) noexcept
        {
            if (this != &value)
            {
                currentUID = value.currentUID;
            }
            return *this;
        }

        [[nodiscard]] constexpr uid_type getCurrentUID() const noexcept { return currentUID; }
        [[nodiscard]] constexpr uid_type getNextUID() const noexcept { return currentUID != maxUID ? currentUID + 1 : minUID; }
        constexpr uid_type generateUID() noexcept
        {
            currentUID = getNextUID();
            return getCurrentUID();
        }

        constexpr void reset() noexcept
        {
            currentUID = minUID;
        }

        [[nodiscard]] explicit constexpr operator uid_type() const noexcept { return getCurrentUID(); }

        [[nodiscard]] constexpr bool operator==(const juid& uid) const noexcept { return getCurrentUID() == uid.getCurrentUID(); }
        [[nodiscard]] constexpr bool operator!=(const juid& uid) const noexcept { return getCurrentUID() != uid.getCurrentUID(); }
        [[nodiscard]] constexpr bool operator==(const uid_type uid) const noexcept { return getCurrentUID() == uid; }
        [[nodiscard]] constexpr bool operator!=(const uid_type uid) const noexcept { return getCurrentUID() != uid; }

    private:

        static constexpr uid_type minUID = invalidUID + 1;
        static constexpr uid_type maxUID = std::numeric_limits<uid_type>::max();

        uid_type currentUID = minUID;
    };
}
