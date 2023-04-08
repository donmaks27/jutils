// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"
#include "jlist.h"

#include <mutex>

namespace jutils
{
    template<typename T, bool SyncAccess = false>
    class jpool_simple;

    class jpool_simple_object
    {
        template<typename T, bool SyncAccess>
        friend class jpool_simple;

    protected:
        jpool_simple_object() = default;
        jpool_simple_object(const jpool_simple_object&) = default;
    public:
        virtual ~jpool_simple_object() = default;

    protected:

        virtual void initPoolObject() {}
        virtual void clearPoolObject() {}
    };

    template<typename T, bool SyncAccess>
    class jpool_simple
    {
    public:

        using type = T;
        static constexpr bool shouldSyncAccess = SyncAccess;

        jpool_simple() = default;
        jpool_simple(const jpool_simple&) = delete;
        jpool_simple(jpool_simple&& pool) noexcept = delete;
        ~jpool_simple() = default;

        jpool_simple& operator=(const jpool_simple&) = delete;
        jpool_simple& operator=(jpool_simple&& pool) noexcept = delete;

        type* getPoolObject();
        void returnPoolObject(type* object);
        void clear();
        
    private:

        struct empty_handler
        {
            void startSync() {}
            void stopSync() {}
        };
        struct sync_handler
        {
            std::mutex mutex;
            void startSync() { mutex.lock(); }
            void stopSync() { mutex.unlock(); }
        };
        using sync_handler_type = std::conditional_t<shouldSyncAccess, sync_handler, empty_handler>;

        jlist<type> objectsPool;
        jarray<type*> unusedObjects;
        sync_handler_type handler;

        
        static void _clearDefaultPoolObject(jpool_simple_object* object) { object->clearPoolObject(); }
        static void _initDefaultPoolObject(jpool_simple_object* object) { object->initPoolObject(); }

        template<bool> void _initPoolObject(type* object) {}
        template<bool> void _clearPoolObject(type* object) {}
        template<>     void _initPoolObject<true>(type* object) { jpool_simple::_initDefaultPoolObject(object); }
        template<>     void _clearPoolObject<true>(type* object) { jpool_simple::_clearDefaultPoolObject(object); }

        void _initPoolObject(type* object) { this->_initPoolObject<jutils::is_base<jpool_simple_object, type>>(object); }
        void _clearPoolObject(type* object) { this->_clearPoolObject<jutils::is_base<jpool_simple_object, type>>(object); }
    };
    
    template<typename T, bool SyncAccess>
    typename jpool_simple<T, SyncAccess>::type* jpool_simple<T, SyncAccess>::getPoolObject()
    {
        type* object;
        handler.startSync();
        if (!unusedObjects.isEmpty())
        {
            object = unusedObjects.getLast();
            unusedObjects.removeLast();
        }
        else
        {
            object = &objectsPool.addDefault();
        }
        handler.stopSync();

        this->_initPoolObject(object);
        return object;
    }
    template<typename T, bool SyncAccess>
    void jpool_simple<T, SyncAccess>::returnPoolObject(type* object)
    {
        if (object != nullptr)
        {
            this->_clearPoolObject(object);

            handler.startSync();
            unusedObjects.add(object);
            handler.stopSync();
        }
    }
    template<typename T, bool SyncAccess>
    void jpool_simple<T, SyncAccess>::clear()
    {
        for (auto& object : objectsPool)
        {
            if (!unusedObjects.contains(&object))
            {
                this->_clearPoolObject(&object);
            }
        }

        handler.startSync();
        unusedObjects.clear();
        objectsPool.clear();
        handler.stopSync();
    }

    template<typename T>
    using jpool_simple_async = jpool_simple<T, true>;
}
