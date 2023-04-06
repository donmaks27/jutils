// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"
#include "jlist.h"

namespace jutils
{
    template<typename T>
    class jpool_simple;

    class jpool_simple_object
    {
        template<typename T>
        friend class jpool_simple;

    protected:
        jpool_simple_object() = default;
    public:
        virtual ~jpool_simple_object() = default;

    protected:

        virtual void initPoolObject() {}
        virtual void clearPoolObject() {}
    };

    template<typename T>
    class jpool_simple
    {
    public:

        using type = T;

        jpool_simple() = default;
        jpool_simple(const jpool_simple&) = delete;
        jpool_simple(jpool_simple&& pool) noexcept
            : objectsPool(std::move(pool.objectsPool)), unusedObjects(pool.unusedObjects)
        {}
        ~jpool_simple() = default;

        jpool_simple& operator=(const jpool_simple&) = delete;
        jpool_simple& operator=(jpool_simple&& pool) noexcept;

        type* getPoolObject();
        void returnPoolObject(type* object);
        void clear();
        
    private:

        jlist<type> objectsPool;
        jarray<type*> unusedObjects;


        static void _initDefaultPoolObject(jpool_simple_object* object) { object->initPoolObject(); }
        static void _clearDefaultPoolObject(jpool_simple_object* object) { object->clearPoolObject(); }

        template<bool>
        void _initPoolObject(type* object) {}
        template<>
        void _initPoolObject<true>(type* object) { jpool_simple::_initDefaultPoolObject(object); }
        void _initPoolObject(type* object) { this->_initPoolObject<jutils::is_base<jpool_simple_object, type>>(object); }
        
        template<bool>
        void _clearPoolObject(type* object) {}
        template<>
        void _clearPoolObject<true>(type* object) { jpool_simple::_clearDefaultPoolObject(object); }
        void _clearPoolObject(type* object) { this->_clearPoolObject<jutils::is_base<jpool_simple_object, type>>(object); }
    };

    template<typename T>
    jpool_simple<T>& jpool_simple<T>::operator=(jpool_simple&& pool) noexcept
    {
        clear();
        objectsPool = std::move(pool.objectsPool);
        unusedObjects = std::move(pool.unusedObjects);
        return *this;
    }

    template<typename T>
    typename jpool_simple<T>::type* jpool_simple<T>::getPoolObject()
    {
        type* object;
        if (!unusedObjects.isEmpty())
        {
            object = unusedObjects.getLast();
            unusedObjects.removeLast();
        }
        else
        {
            object = &objectsPool.addDefault();
        }
        this->_initPoolObject(object);
        return object;
    }
    template<typename T>
    void jpool_simple<T>::returnPoolObject(type* object)
    {
        if (object != nullptr)
        {
            this->_clearPoolObject(object);
            unusedObjects.add(object);
        }
    }
    template<typename T>
    void jpool_simple<T>::clear()
    {
        for (auto& object : objectsPool)
        {
            if (!unusedObjects.contains(&object))
            {
                this->_clearPoolObject(&object);
            }
        }
        unusedObjects.clear();
        objectsPool.clear();
    }
}
