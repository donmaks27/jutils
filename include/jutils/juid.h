// Copyright 2021 Leonov Maksim. All Rights Reserved.

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
        static constexpr id_type invalidUID = 0;
        static constexpr id_type minUID = 1;
        static constexpr id_type maxUID = std::numeric_limits<id_type>::max();

        juid()
            : nextUID(minUID)
        {}
        juid(const juid& value)
            : nextUID(value.nextUID)
        {}

        juid& operator=(const juid& value)
        {
            if (this != &value)
            {
                nextUID = value.nextUID;
            }
            return *this;
        }

        id_type getNextUID() const { return nextUID; }
        id_type getUID()
        {
            if (nextUID == invalidUID)
            {
                return invalidUID;
            }
            if (nextUID == maxUID)
            {
                nextUID = invalidUID;
                return maxUID;
            }
            return nextUID++;
        }

        void reset()
        {
            nextUID = minUID;
        }

    private:

        id_type nextUID;
    };
}
