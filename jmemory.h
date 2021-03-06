// Copyright 2022 Leonov Maksim. All Rights Reserved.

#pragma once

#include "type_defines.h"

#include <utility>

#ifdef JUTILS_DEBUG_NEW
    #undef new
#endif

namespace jutils
{
    namespace memory
    {
        template<typename Type>
        inline Type* allocate(const int32 size)
        {
            if (size > 0)
            {
                return static_cast<Type*>(::operator new(sizeof(Type) * size, static_cast<std::align_val_t>(alignof(Type))));
            }
            return nullptr;
        }

        template<typename Type>
        inline void deallocate(Type* data, const int32 size)
        {
            if ((data != nullptr) && (size > 0))
            {
                ::operator delete(data, sizeof(Type) * size, static_cast<std::align_val_t>(alignof(Type)));
            }
        }

        template<typename Type, typename... Args>
        inline void construct(Type* object, Args&&... args)
        {
            if (object != nullptr)
            {
                ::new (object) Type(std::forward<Args>(args)...);
            }
        }

        template<typename Type>
        inline void destruct(Type* object)
        {
            if (object != nullptr)
            {
                object->~Type();
            }
        }
    }
}

#ifdef JUTILS_DEBUG_NEW
    #define new JUTILS_DEBUG_NEW
#endif
