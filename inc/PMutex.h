#pragma once

#include "PFormat.h"

#if defined(PLATFORM_64BIT)
#include <mutex>
#elif defined(H7_FREE_RTOS)
#include <cmsis_os.h> // Für osMutexId_t
#elif defined(THREADX)
#include <tx_api.h> // Für TX_MUTEX
#else
#error "Platform not defined: Must define PLATFORM_64BIT, H7_FREE_RTOS, or THREADX"
#endif

namespace ptools
{
    struct PMutex
    {
#if defined(PLATFORM_64BIT)
        std::mutex mutex;
#elif defined(H7_FREE_RTOS)
        osMutexId_t mutex;
        osMutexAttr_t attr;
#elif defined(THREADX)
        TX_MUTEX mutex;
#endif

        PMutex()
        {
#if defined(PLATFORM_64BIT)
            // std::mutex benötigt keine explizite Initialisierung
#elif defined(H7_FREE_RTOS)
            attr.name = nullptr;
            attr.attr_bits = 0;
            attr.cb_mem = nullptr;
            attr.cb_size = 0;
            mutex = osMutexNew(&attr);
            if (!mutex)
            {
                // error treatment ...
            }
#elif defined(THREADX)
            UINT status = tx_mutex_create(&mutex, nullptr, TX_NO_INHERIT);
            if (status != TX_SUCCESS)
            {
                // error treatment ...
            }
#endif
        }

        ~PMutex()
        {
#if defined(PLATFORM_64BIT)
            // std::mutex Destruktor ist ausreichend
#elif defined(H7_FREE_RTOS)
            if (mutex)
            {
                osMutexDelete(mutex);
            }
#elif defined(THREADX)
            tx_mutex_delete(&mutex);
#endif
        }
    };

    struct PMutexLocker
    {
        PMutexLocker(PMutex& m) : mutex(m)
        {
#if defined(PLATFORM_64BIT)
            mutex.mutex.lock();
#elif defined(H7_FREE_RTOS)
            osMutexAcquire(mutex.mutex, osWaitForever);
#elif defined(THREADX)
            tx_mutex_get(&mutex.mutex, TX_WAIT_FOREVER);
#endif
        }

        ~PMutexLocker()
        {
#if defined(PLATFORM_64BIT)
            mutex.mutex.unlock();
#elif defined(H7_FREE_RTOS)
            osMutexRelease(mutex.mutex);
#elif defined(THREADX)
            tx_mutex_put(&mutex.mutex);
#endif
        }

    private:
        PMutex& mutex;
    };
}


