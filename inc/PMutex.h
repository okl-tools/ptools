#pragma once

#include "PFormat.h"

#if defined(PLATFORM_64BIT)
    #include <mutex>
#elif defined(H7_FREE_RTOS)
    #include <cmsis_os.h> // osMutexId_t
#elif defined(CORTEX_M7_FREE_RTOS)
    #include <cmsis_os.h>
#elif defined(THREADX)
    #include <tx_api.h> // TX_MUTEX
#else
    #error "Platform not defined: Must define PLATFORM_64BIT, H7_FREE_RTOS, or THREADX"
#endif

namespace ptools
{
    struct PMutex
    {
#if defined(PLATFORM_64BIT)
        std::mutex mutex;
#elif defined(H7_FREE_RTOS)  // free rtos v 10
        osMutexId_t mutex;
        osMutexAttr_t attr;
#elif defined(CORTEX_M7_FREE_RTOS) // free rtos v 8
        SemaphoreHandle_t handle = nullptr;
#elif defined(THREADX)
        TX_MUTEX mutex;
#endif

        PMutex();
        ~PMutex();
    };

    struct PMutexLocker
    {
        PMutexLocker(PMutex& m);
        ~PMutexLocker();

    private:
        PMutex& mutex;
    };
}


