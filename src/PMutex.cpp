#include "PMutex.h"
#include "PFormat.h"
#include "PWriter.h"

namespace ptools
{

    PMutex::PMutex ()
    {
#if defined(PLATFORM_64BIT)
        // does not need explicite ini
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
#elif defined(CORTEX_M7_FREE_RTOS)
            handle = xSemaphoreCreateMutex();
            if (handle == nullptr)
            {
                // FATAL: Unable to create mutex
                // Optional: Blink LED, assert, loop...
            }
#elif defined(THREADX)
            UINT status = tx_mutex_create(&mutex, nullptr, TX_NO_INHERIT);
            if (status != TX_SUCCESS)
            {
                // error treatment ...
            }
#endif
    }

    PMutex::~PMutex ()
    {
#if defined(PLATFORM_64BIT)
        // std::mutex destruktor ok
#elif defined(H7_FREE_RTOS)
        if (mutex)
            {
                osMutexDelete(mutex);
            }
#elif defined(CORTEX_M7_FREE_RTOS)
            if (handle)
            {
                vSemaphoreDelete(handle);
            }
#elif defined(THREADX)
            tx_mutex_delete(&mutex);
#endif
    }


    PMutexLocker::PMutexLocker (PMutex & m) : mutex(m)
    {
#if defined(PLATFORM_64BIT)
        mutex.mutex.lock();
#elif defined(H7_FREE_RTOS)
        osMutexAcquire(mutex.mutex, osWaitForever);
#elif defined(CORTEX_M7_FREE_RTOS)
            if (mutex.handle)
            {
                // Warte unendlich auf den Mutex
                xSemaphoreTake(mutex.handle, portMAX_DELAY);
            }
#elif defined(THREADX)
            tx_mutex_get(&mutex.mutex, TX_WAIT_FOREVER);
#endif



    }

    PMutexLocker::~PMutexLocker ()
    {
#if defined(PLATFORM_64BIT)
        mutex.mutex.unlock();
#elif defined(H7_FREE_RTOS)
        osMutexRelease(mutex.mutex);
#elif defined(CORTEX_M7_FREE_RTOS)
            if (mutex.handle)
            {
                xSemaphoreGive(mutex.handle);
            }
#elif defined(THREADX)
            tx_mutex_put(&mutex.mutex);
#endif
    }
}

