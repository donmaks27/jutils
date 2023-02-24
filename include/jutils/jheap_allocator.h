// Copyright © 2021-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"
#include "juid.h"

namespace jutils
{
    class jheap_allocator
    {
    public:

        using index_type = int32;
        using uid_type = uint32;
        using uid_generator_type = juid<uid_type>;

        class pointer
        {
            friend jheap_allocator;

        public:
            pointer() = default;
            pointer(const pointer&) = default;

        private:

            index_type objectIndex = -1;
            uid_type UID = uid_generator_type::invalidUID;
        };

        jheap_allocator() = default;
        explicit jheap_allocator(const index_type heapSize, const index_type defaultObjectsSize = 100)
        {
            if (heapSize > 0)
            {
                data = new uint8[heapSize];
                size = heapSize;

                objects.reserve(100);
            }
        }
        ~jheap_allocator() { clear(); }

        template<typename T, typename... Args>
        pointer createObject(Args&&... args)
        {
            if (data == nullptr)
            {
                return pointer();
            }

            const pointer result = allocateObjectNode<T>();
            if (isObjectValid(result))
            {
                jutils::memory::construct(static_cast<T*>(getObjectNode(result)), std::forward<Args>(args)...);
            }
            return result;
        }
        bool isObjectValid(const pointer& pointer) const
        {
            return (data != nullptr) && (pointer.UID != uid_generator_type::invalidUID) && 
                objects.isValidIndex(pointer.objectIndex) &&
                (pointer.UID == objects[pointer.objectIndex].UID);
        }
        template<typename T>
        void destroyObject(const pointer& pointer)
        {
            if ((data != nullptr) && isObjectValid(pointer))
            {
                jutils::memory::destruct(static_cast<T*>(getObjectNode(pointer)));
                deallocateObjectNode(pointer);
            }
        }

        void* getObject(const pointer& pointer) const { return isObjectValid(pointer) ? getObjectNode(pointer) : nullptr; }
        template<typename T>
        T* getObject(const pointer& pointer) const { return static_cast<T*>(getObject(pointer)); }

        void defragmentMemory(const index_type defragmentNodesLimit = 3)
        {
            if (data != nullptr)
            {
                defragmentObjectNodes(defragmentNodesLimit);
            }
        }

        void clear()
        {
            if (data != nullptr)
            {
                delete[] data;
                size = 0;
                objects.clear();
            }
        }

    private:
        
        struct object_description
        {
            index_type nextObjectIndex = -1;

            index_type indexFirst = -1;
            index_type indexLast = -1;
            uid_type UID = uid_generator_type::invalidUID;

            uint8 alignPow = 0;
        };

        uint8* data = nullptr;
        index_type size = 0;

        jarray<object_description> objects;
        index_type firstObjectIndex = -1;
        index_type firstEmptyObjectIndex = -1;
        bool isDataDefragmented = true;

        uid_generator_type nodeUIDs;


        template<typename T>
        static constexpr uint8 getPowForAlign()
        {
            uint8 pow = 0;
            uint16 align = alignof(T);
            while (align > 1)
            {
                align >>= 1;
                pow++;
            }
            return pow;
        }

        index_type getAlignedIndex(const index_type index, const uint16 align) const
        {
            const uintptr_t dataPtr = reinterpret_cast<uintptr_t>(data);
            return static_cast<index_type>(((dataPtr + index + align - 1) & ~(align - 1)) - dataPtr);
        }

        template<typename T>
        pointer allocateObjectNode()
        {
            pointer result;
            index_type prevObjectIndex = -1;
            index_type objectIndex = firstObjectIndex;
            while (objectIndex != -1)
            {
                if (tryAllocateObjectNode<T>(prevObjectIndex, objectIndex, result))
                {
                    return result;
                }

                prevObjectIndex = objectIndex;
                objectIndex = objects[objectIndex].nextObjectIndex;
            }

            if (tryAllocateObjectNode<T>(prevObjectIndex, objectIndex, result))
            {
                return result;
            }
            return pointer();
        }
        template<typename T>
        bool tryAllocateObjectNode(const index_type prevObjectIndex, const index_type nextObjectIndex, pointer& outPointer)
        {
            const index_type holeIndexFirst = prevObjectIndex >= 0 ? objects[prevObjectIndex].indexLast + 1 : 0;
            const index_type holeIndexLast = nextObjectIndex >= 0 ? objects[nextObjectIndex].indexFirst - 1 : size - 1;
            if (holeIndexFirst > holeIndexLast)
            {
                return false;
            }

            const index_type indexFirst = getAlignedIndex(holeIndexFirst, alignof(T));
            const index_type indexLast = indexFirst + sizeof(T) - 1;
            if (indexLast > holeIndexLast)
            {
                return false;
            }

            const index_type objectIndex = getFreeObject();
            if (objectIndex == -1)
            {
                return false;
            }

            object_description& description = objects[objectIndex];
            description.nextObjectIndex = nextObjectIndex;
            description.indexFirst = indexFirst;
            description.indexLast = indexLast;
            description.UID = nodeUIDs.generateUID();
            description.alignPow = getPowForAlign<T>();
            if (prevObjectIndex == -1)
            {
                firstObjectIndex = objectIndex;
            }
            else
            {
                objects[prevObjectIndex].nextObjectIndex = objectIndex;
            }

            outPointer.UID = description.UID;
            outPointer.objectIndex = objectIndex;
            return true;
        }
        index_type getFreeObject()
        {
            if (firstEmptyObjectIndex == -1)
            {
                const index_type oldSize = objects.getSize();
                objects.resize(oldSize + 100);
                const index_type newSize = objects.getSize();
                if (newSize <= oldSize)
                {
                    // Something goes wrong
                    return -1;
                }

                for (index_type i = oldSize + 1; i < newSize; i++)
                {
                    objects[i - 1].nextObjectIndex = i;
                }
                firstEmptyObjectIndex = oldSize;
            }

            const index_type result = firstEmptyObjectIndex;
            firstEmptyObjectIndex = objects[firstEmptyObjectIndex].nextObjectIndex;
            return result;
        }

        void* getObjectNode(const pointer& pointer) const { return data + objects[pointer.objectIndex].indexFirst; }

        void deallocateObjectNode(const pointer& pointer)
        {
            object_description& description = objects[pointer.objectIndex];
            if (pointer.objectIndex == firstObjectIndex)
            {
                firstObjectIndex = description.nextObjectIndex;
            }
            else
            {
                index_type prevObjectIndex = -1;
                index_type objectIndex = firstObjectIndex;
                while (objectIndex != pointer.objectIndex)
                {
                    prevObjectIndex = objectIndex;
                    objectIndex = objects[objectIndex].nextObjectIndex;
                }
                objects[prevObjectIndex].nextObjectIndex = description.nextObjectIndex;
            }

            if (description.nextObjectIndex != -1)
            {
                isDataDefragmented = false;
            }

            description.UID = uid_generator_type::invalidUID;
            description.nextObjectIndex = firstEmptyObjectIndex;
            firstEmptyObjectIndex = pointer.objectIndex;
        }

        void defragmentObjectNodes(const index_type defragmentNodesLimit)
        {
            if (isDataDefragmented || (defragmentNodesLimit <= 0))
            {
                return;
            }

            index_type nodesLeft = defragmentNodesLimit;
            index_type prevObjectIndex = -1;
            index_type objectIndex = firstObjectIndex;
            while ((nodesLeft > 0) && (objectIndex != -1))
            {
                const index_type holeIndexFirst = prevObjectIndex != -1 ? objects[prevObjectIndex].indexLast + 1 : 0;
                object_description& object = objects[objectIndex];

                prevObjectIndex = objectIndex;
                objectIndex = object.nextObjectIndex;

                if (object.indexFirst == holeIndexFirst)
                {
                    continue;
                }

                const index_type indexFirst = getAlignedIndex(holeIndexFirst, static_cast<uint16>(1 << object.alignPow));
                if (object.indexFirst <= indexFirst)
                {
                    continue;
                }

                const index_type objectSize = object.indexLast - object.indexFirst + 1;
                memcpy(data + indexFirst, data + object.indexFirst, objectSize);
                object.indexFirst = indexFirst;
                object.indexLast = indexFirst + objectSize - 1;
                nodesLeft--;
            }

            if (objectIndex == -1)
            {
                isDataDefragmented = true;
            }
        }
    };
}
