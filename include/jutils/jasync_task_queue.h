// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"
#include "jlist.h"

#include <condition_variable>
#include <functional>
#include <mutex>

namespace jutils
{
    class jasync_task
    {
    protected:
        jasync_task() = default;
    public:
        virtual ~jasync_task() = default;

        virtual void run() = 0;
        virtual bool shouldDeleteAfterExecution() const { return true; }
    };
    class jasync_task_default : public jasync_task
    {
    public:
        jasync_task_default() = default;
        jasync_task_default(const std::function<void()>& func) : taskFunction(func) {}
        jasync_task_default(std::function<void()>&& func) noexcept : taskFunction(std::move(func)) {}
        virtual ~jasync_task_default() override = default;

        virtual void run() override
        {
            if (taskFunction != nullptr)
            {
                taskFunction();
            }
        }

    private:

        std::function<void()> taskFunction = nullptr;
    };

    class jasync_task_queue_base
    {
    protected:
        jasync_task_queue_base() = default;
    public:

        bool isValid() const { return asyncWorkerCount != 0; }

        inline bool addTask(jasync_task* task);
        inline void addTasks(const jarray<jasync_task*>& tasks);
        inline void clearTasks();

    protected:

        struct task_description
        {
            jasync_task* task = nullptr;

            void clear() const
            {
                if (task->shouldDeleteAfterExecution())
                {
                    delete task;
                }
            }
        };

        std::mutex tasksQueueMutex;
        std::condition_variable taskAvailableCondition;
        jlist<task_description> tasksQueue;

        int32 asyncWorkerCount = 0;
    };

    class jasync_worker;
    template<typename WorkerType = jasync_worker>
    class jasync_task_queue;

    class jasync_worker
    {
        template<typename T>
        friend class jasync_task_queue;

    public:
        jasync_worker() = default;

        bool onStart_MainThread() const { return true; }
        bool onStart_WorkerThread() const { return true; }
        void onStop_WorkerThread() const {}
        void onStop_MainThread() const {}

        int32 getWorkerIndex() const { return workerIndex; }

    private:

        std::thread workerThread;
        int32 workerIndex = -1;
        std::atomic_bool shouldStop = false;
    };

    template<typename WorkerType>
    class jasync_task_queue : public jasync_task_queue_base
    {
    public:

        using worker_type = WorkerType;

        jasync_task_queue() = default;
        jasync_task_queue(const jasync_task_queue&) = delete;
        jasync_task_queue(jasync_task_queue&&) noexcept = delete;
        ~jasync_task_queue() { stop(); }

        jasync_task_queue& operator=(const jasync_task_queue&) = delete;
        jasync_task_queue& operator=(jasync_task_queue&&) noexcept = delete;

        template<typename... Args>
        bool init(int32 workerCount, Args&&... args);
        void stop();
        
    private:

        worker_type* asyncWorkers = nullptr;


        void _workerThreadFunction(worker_type* worker);
    };
    
    inline bool jasync_task_queue_base::addTask(jasync_task* task)
    {
        if ((asyncWorkerCount == 0) || (task == nullptr))
        {
            return false;
        }

        tasksQueueMutex.lock();
        tasksQueue.add({ task });
        tasksQueueMutex.unlock();

        taskAvailableCondition.notify_one();
        return true;
    }
    inline void jasync_task_queue_base::addTasks(const jarray<jasync_task*>& tasks)
    {
        if ((asyncWorkerCount == 0) || tasks.isEmpty())
        {
            return;
        }

        tasksQueueMutex.lock();
        for (const auto& task : tasks)
        {
            tasksQueue.add({ task });
        }
        tasksQueueMutex.unlock();

        taskAvailableCondition.notify_all();
    }
    inline void jasync_task_queue_base::clearTasks()
    {
        if (asyncWorkerCount == 0)
        {
            return;
        }

        std::lock_guard lock(tasksQueueMutex);
        for (const auto& task : tasksQueue)
        {
            task.clear();
        }
        tasksQueue.clear();
    }

    template<typename WorkerType>
    template<typename... Args>
    bool jasync_task_queue<WorkerType>::init(const int32 workerCount, Args&&... args)
    {
        if ((asyncWorkerCount != 0) || (workerCount <= 0))
        {
            return false;
        }
        
        asyncWorkers = memory::allocate<worker_type>(workerCount);
        asyncWorkerCount = workerCount;
        for (int32 index = 0; index < workerCount; index++)
        {
            memory::construct(asyncWorkers + index, std::forward<Args>(args)...);

            asyncWorkers[index].workerIndex = index;
            if (!asyncWorkers[index].onStart_MainThread())
            {
                memory::destruct(asyncWorkers + index);
                for (int32 index1 = index - 1; index1 >= 0; index1--)
                {
                    asyncWorkers[index1].onStop_MainThread();
                    memory::destruct(asyncWorkers + index1);
                }

                memory::deallocate(asyncWorkers, asyncWorkerCount);
                asyncWorkers = nullptr;
                asyncWorkerCount = 0;
                return false;
            }
        }
        for (int32 index = 0; index < workerCount; index++)
        {
            asyncWorkers[index].workerThread = std::thread(&jasync_task_queue<WorkerType>::_workerThreadFunction, this, &asyncWorkers[index]);
        }
        return true;
    }
    template<typename WorkerType>
    void jasync_task_queue<WorkerType>::stop()
    {
        if (asyncWorkerCount == 0)
        {
            return;
        }

        for (int32 index = 0; index < asyncWorkerCount; index++)
        {
            asyncWorkers[index].shouldStop = true;
        }
        taskAvailableCondition.notify_all();
        for (int32 index = 0; index < asyncWorkerCount; index++)
        {
            asyncWorkers[index].workerThread.join();
            asyncWorkers[index].onStop_MainThread();
            memory::destruct(asyncWorkers + index);
        }

        memory::deallocate(asyncWorkers, asyncWorkerCount);
        asyncWorkers = nullptr;
        asyncWorkerCount = 0;

        for (const auto& task : tasksQueue)
        {
            task.clear();
        }
        tasksQueue.clear();
    }

    template<typename WorkerType>
    void jasync_task_queue<WorkerType>::_workerThreadFunction(worker_type* worker)
    {
        if (!worker->onStart_WorkerThread())
        {
            return;
        }

        while (true)
        {
            std::unique_lock lock(tasksQueueMutex);
            if (worker->shouldStop)
            {
                lock.unlock();
                break;
            }
            if (tasksQueue.isEmpty())
            {
                taskAvailableCondition.wait(lock);
                if (worker->shouldStop)
                {
                    lock.unlock();
                    break;
                }
            }
            const task_description task = tasksQueue.getFirst();
            tasksQueue.removeFirst();
            lock.unlock();

            if (task.task != nullptr)
            {
                task.task->run();
                task.clear();
            }
            if (worker->shouldStop)
            {
                break;
            }
        }

        worker->onStop_WorkerThread();
    }
}
