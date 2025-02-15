// Copyright © 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "core.h"

#include "base_types.h"
#include "macro/template_condition.h"
#include <limits>

namespace jutils
{
    JUTILS_TEMPLATE_CONDITION(std::is_integral_v<IdType>, typename IdType = uint64)
    class uid
    {
    public:

        using uid_type = IdType;
        static constexpr uid_type invalidUID = std::numeric_limits<uid_type>::min();

        constexpr uid() noexcept = default;
        constexpr uid(const uid& value) noexcept
            : currentUID(value.currentUID)
        {}
        constexpr uid(uid&&) noexcept = default;
        constexpr ~uid() = default;

        constexpr uid& operator=(const uid& value) noexcept
        {
            if (this != &value)
            {
                currentUID = value.currentUID;
            }
            return *this;
        }
        constexpr uid& operator=(uid&&) noexcept = default;

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

    private:

        static constexpr uid_type minUID = invalidUID + 1;
        static constexpr uid_type maxUID = std::numeric_limits<uid_type>::max();

        uid_type currentUID = minUID;
    };
}
