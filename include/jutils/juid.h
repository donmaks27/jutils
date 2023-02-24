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

        using id_type = IdType;
        static constexpr id_type invalidUID = std::numeric_limits<id_type>::min();
        static constexpr id_type minUID = invalidUID + 1;
        static constexpr id_type maxUID = std::numeric_limits<id_type>::max();

        juid() = default;
        juid(const juid& value)
            : currentUID(value.currentUID)
        {}

        juid& operator=(const juid& value)
        {
            if (this != &value)
            {
                currentUID = value.currentUID;
            }
            return *this;
        }

        id_type getUID() const { return currentUID; }
        id_type getNextUID() const { return currentUID != maxUID ? currentUID + 1 : invalidUID; }
        id_type generateUID()
        {
            currentUID = getNextUID();
            return getUID();
        }

        void reset()
        {
            currentUID = minUID;
        }

    private:

        id_type currentUID = minUID;
    };
}
