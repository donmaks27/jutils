// Copyright 2021 Leonov Maksim. All Rights Reserved.

#include "jlog.h"

#ifndef JUTILS_LOG_DISABLED

#include <iostream>
#include <mutex>

std::mutex LogMutex;

namespace jutils
{
    namespace jlog
    {
        void writeLog(const char* const prefix, const char* method, const char* const message)
        {
            std::lock_guard lock(LogMutex);
            std::cout << prefix;
            if ((method != nullptr) && (method[0] != '\0'))
            {
                std::cout << method << JSTR(": ");
            }
            if ((message != nullptr) && (message[0] != '\0'))
            {
                std::cout << message;
            }
            std::cout << std::endl;
        }
    }
}

#endif
