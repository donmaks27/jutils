// Copyright © 2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include "jarray.h"
#include "jlist.h"

#include <functional>
#include <mutex>
#include <thread>

namespace jutils
{
    template<typename TaskType = std::function<void()>>
    class jtask_queue
    {
    public:

        using task_type = TaskType;

        jtask_queue() = default;
        jtask_queue(const jtask_queue&) = delete;
        jtask_queue(jtask_queue&&) noexcept = delete;
        ~jtask_queue() { stop(); }

        jtask_queue& operator=(const jtask_queue&) = delete;
        jtask_queue& operator=(jtask_queue&&) noexcept = delete;

        bool start(int32 workersCount, std::function<bool(int32 workerIndex)> workerInitCallback = nullptr, 
            std::function<void(int32 workerIndex)> workerDestroyCallback = nullptr);
        void stop();

        template<typename... Args>
        bool addTask(Args&&... args);
        void clearTasks();

    private:

        struct workerDescription
        {
            std::thread workerThread = std::thread();
            std::atomic_bool shouldStop = false;
        };
        
        workerDescription* workers = nullptr;
        int32 workersArraySize = 0;
        std::function<bool(int32 workerIndex)> workerInitFunction = nullptr;
        std::function<void(int32 workerIndex)> workerDestroyFunction = nullptr;

        jlist<task_type> plannedTasks;
        std::mutex tasksMutex;
        std::condition_variable taskAvailable;


        void _workerFunction(int32 workerIndex, workerDescription* worker);
    };

    template<typename TaskType>
    bool jtask_queue<TaskType>::start(const int32 workersCount, std::function<bool(int32 workerIndex)> workerInitCallback,
        std::function<void(int32 workerIndex)> workerDestroyCallback)
    {
        if ((workersCount <= 0) || (workersArraySize != 0))
        {
            return false;
        }
        workerInitFunction = std::move(workerInitCallback);
        workerDestroyFunction = std::move(workerDestroyCallback);
        workers = new workerDescription[workersCount];
        workersArraySize = workersCount;
        for (int32 index = 0; index < workersCount; index++)
        {
            workers[index].workerThread = std::thread(&jtask_queue::_workerFunction, this, index, &workers[index]);
        }
        return true;
    }
    template<typename TaskType>
    void jtask_queue<TaskType>::stop()
    {
        if (workersArraySize == 0)
        {
            return;
        }

        for (int32 index = 0; index < workersArraySize; index++)
        {
            workers[index].shouldStop = true;
        }
        taskAvailable.notify_all();
        for (int32 index = 0; index < workersArraySize; index++)
        {
            workers[index].workerThread.join();
        }
        delete[] workers;
        workersArraySize = 0;
        workerInitFunction = nullptr;
        workerDestroyFunction = nullptr;

        plannedTasks.clear();
    }

    template<typename TaskType>
    void jtask_queue<TaskType>::_workerFunction(const int32 workerIndex, workerDescription* worker)
    {
        if (workerInitFunction)
        {
            if (!workerInitFunction(workerIndex))
            {
                return;
            }
        }

        while (true)
        {
            std::unique_lock lock(tasksMutex);
            if (worker->shouldStop)
            {
                lock.unlock();
                break;
            }
            if (plannedTasks.isEmpty())
            {
                taskAvailable.wait(lock);
                if (worker->shouldStop)
                {
                    lock.unlock();
                    break;
                }
            }
            task_type task = std::move(plannedTasks.getFirst());
            plannedTasks.removeFirst();
            lock.unlock();

            task();
            if (worker->shouldStop)
            {
                break;
            }
        }

        if (workerDestroyFunction)
        {
            workerDestroyFunction(workerIndex);
        }
    }

    template<typename TaskType>
    template<typename... Args>
    bool jtask_queue<TaskType>::addTask(Args&&... args)
    {
        tasksMutex.lock();
        if (workersArraySize == 0)
        {
            tasksMutex.unlock();
            return false;
        }
        plannedTasks.add(task_type(std::forward<Args>(args)...));
        tasksMutex.unlock();

        taskAvailable.notify_one();
        return true;
    }
    template<typename TaskType>
    void jtask_queue<TaskType>::clearTasks()
    {
        tasksMutex.lock();
        plannedTasks.clear();
        tasksMutex.unlock();
    }
}
