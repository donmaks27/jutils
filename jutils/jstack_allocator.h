// Copyright 2021 Leonov Maksim. All Rights Reserved.

#pragma once

#include "type_defines.h"
#include "math/jmath.h"

namespace jutils
{
    class jstack_allocator
    {
    public:

        jstack_allocator() = default;
        jstack_allocator(const uint32 stackSize)
        {
            size = jutils::math::min(stackSize, std::numeric_limits<uint32>::max() - 1);
            if (size > 0)
            {
                data = new uint8[size];
            }
        }
        ~jstack_allocator()
        {
            delete[] data;
        }

        template<typename T, typename... Args>
        T* createObject(Args&&... args)
        {
            T* object = allocateObject<T>();
            if (object != nullptr)
            {
                ::new (static_cast<void*>(object)) T(std::forward<Args>(args)...);
            }
            return object;
        }
        template<typename T>
        void destroyObject(const T* object)
        {
            if ((object != nullptr) && (object == getLastObject()))
            {
                object->~T();
                deallocateObject(object);
            }
        }

    private:

        using index_type = uint32;
        using object_size_type = uint16;

        uint8* data = nullptr;
        index_type size = 0;
        index_type headIndex = 0;


        template<typename T>
        static constexpr bool isTypeAllowed = (alignof(T) <= 256) && (sizeof(T) <= std::numeric_limits<object_size_type>::max());
        template<typename T>
        T* allocateObject()
        {
            if (!isTypeAllowed<T>)
            {
                return nullptr;
            }

            const index_type objectIndex = (headIndex + alignof(T)) & ~(alignof(T) - 1);
            if (objectIndex <= headIndex)
            {
                // Overflow
                return nullptr;
            }

            const object_size_type objectSize = sizeof(T);
            const index_type nodeSize = objectIndex - headIndex + objectSize + sizeof(object_size_type);
            if ((size + 1 - headIndex) < nodeSize)
            {
                // Not enough free space
                return nullptr;
            }

            const uint8 offset = static_cast<uint8>(objectIndex - headIndex);
            headIndex += nodeSize;

            data[objectIndex - 1] = offset != 256 ? offset : 0;
            memcpy(data + objectIndex + objectSize, &objectSize, sizeof(object_size_type));
            return reinterpret_cast<T*>(data + objectIndex);
        }
        void* getLastObject() const
        {
            if (headIndex == 0)
            {
                return nullptr;
            }

            const index_type objectSizeIndex = headIndex - sizeof(object_size_type);
            object_size_type objectSize = 0;
            memcpy(&objectSize, data + objectSizeIndex, sizeof(object_size_type));
            return data + objectSizeIndex - objectSize;
        }
        void deallocateObject(const void* object)
        {
            const index_type objectIndex = static_cast<index_type>(static_cast<const uint8*>(object) - data);
            const uint8 offset = data[objectIndex - 1];
            headIndex = objectIndex - (offset == 0 ? 256 : offset);
        }
    };
}
