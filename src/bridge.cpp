#include "pp.h"
#include "PFormat.h"


extern "C" {

void write_debug(const char*);



#ifdef CORTEX_M7_NETX_DUO

#include "main.h"
#include "tx_api.h"


extern UART_HandleTypeDef huart3;

static int flagHalTime = 0;

void set_hal_time (int flag)
{
    flagHalTime = flag;
}


void write_string_buf (const char * pStr)
{
    if (pStr)
    {
        HAL_UART_Transmit(&huart3, (uint8_t *) pStr, strlen(pStr), HAL_MAX_DELAY);
    }
}

uint32_t get_milliseconds ()
{
    if (flagHalTime)
    {
        return HAL_GetTick();
    }

    return (tx_time_get() * 1000) / TX_TIMER_TICKS_PER_SECOND;
}

uint32_t get_thread_ID ()
{
    TX_THREAD * thread_current = tx_thread_identify();

    return thread_current ? ((uintptr_t) thread_current & 0xFFFFFF) : 0;
}

#elif CORTEX_M7_FREE_RTOS

#include "stm32f7xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

extern UART_HandleTypeDef huart3;

static int flagHalTime = 0;

static SemaphoreHandle_t uart_mutex = NULL;

void init_uart_mutex(void)
{
    uart_mutex = xSemaphoreCreateMutex();
}

void write_string_buf(const char *pStr)
{
    if (!pStr)
        return;

    if (uart_mutex)
    {
        if (xSemaphoreTake(uart_mutex, portMAX_DELAY) == pdTRUE)
        {
            HAL_UART_Transmit(&huart3, (uint8_t *)pStr, strlen(pStr), HAL_MAX_DELAY);
            xSemaphoreGive(uart_mutex);
        }
    }
    else
    {
        // Fallback (nicht thread-safe!)
        HAL_UART_Transmit(&huart3, (uint8_t *)pStr, strlen(pStr), HAL_MAX_DELAY);
    }
}

uint32_t get_milliseconds ()
{
    return HAL_GetTick();
}

uint32_t get_thread_ID()
{
    TaskHandle_t thread_current = xTaskGetCurrentTaskHandle();

    if (thread_current)
    {
        // Hole Pointerwert, maske auf 24 Bit, skaliere in 5-stelligen Bereich
        uintptr_t raw = (uintptr_t)thread_current & 0xFFFFFF;

        // 89999 = (99999 - 10000)
        return 10000 + (raw % 89999);
    }

    return 0;
}


// ------------------------------------------------------------------------------------------------------------------------------------------
#elif CORTEX_M7_BARE_METAL

#include "main.h"


extern UART_HandleTypeDef huart3;

static int flagHalTime = 0;

void set_hal_time (int flag)
{
    flagHalTime = flag;
}


void write_string_buf (const char * pStr)
{
    if (pStr)
    {
        HAL_UART_Transmit(&huart3, (uint8_t *) pStr, strlen(pStr), HAL_MAX_DELAY);
    }
}

uint32_t get_milliseconds ()
{
    return HAL_GetTick();
}

uint32_t get_thread_ID ()
{
    return 0;
}

#elif H7_FREE_RTOS

#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo.h"

#include "FreeRTOS.h"
#include "task.h"


extern UART_HandleTypeDef hcom_uart[COMn];

//UART_HandleTypeDef & huart3 = hcom_uart[COMn];

void write_string_buf (const char * pStr)
{
    if (pStr)
    {
//        HAL_UART_Transmit(&huart3, (uint8_t *) pStr, strlen(pStr), HAL_MAX_DELAY);
//        HAL_UART_Transmit(&hcom_uart[COM1], (uint8_t *) pStr, strlen(pStr), HAL_MAX_DELAY);
        HAL_UART_Transmit(&hcom_uart[COM1], (uint8_t *) pStr, ptools::string_len(pStr), HAL_MAX_DELAY);
    }
}

uint32_t get_milliseconds ()
{
    return HAL_GetTick();
}

void sleep_milliseconds(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}


uint32_t get_thread_ID ()
{
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();

    return (uint32_t)(uintptr_t)handle;
}

void sleep_ms(uint32_t ms)
{
    HAL_Delay(ms);
}



#else

#include <stdio.h>
#include <time.h>
#include <stdint.h>

static int flagHalTime = 0;

void set_hal_time (int flag)
{
    flagHalTime = flag;
}


void write_string_buf (const char * pStr)
{
    if (pStr)
    {
        printf("%s", pStr);
    }
}

uint32_t get_milliseconds_x()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

static struct timespec start_ts;

__attribute__((constructor))
static void init_start_time()
{
    clock_gettime(CLOCK_MONOTONIC, &start_ts);
}

uint32_t get_milliseconds()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    uint64_t ms_start = start_ts.tv_sec * 1000ULL + start_ts.tv_nsec / 1000000;
    uint64_t ms_now   = now.tv_sec * 1000ULL + now.tv_nsec / 1000000;

    return (uint32_t)(ms_now - ms_start);
}


uint32_t get_thread_ID ()
{
//    TX_THREAD * thread_current = tx_thread_identify();
//    return thread_current ? ((uintptr_t) thread_current & 0xFFFFFF) : 0;
    return 0;
}

void sleep_milliseconds(uint32_t ms)
{
    struct timespec ts;

    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;

    nanosleep(&ts, NULL);
}


#endif

}
