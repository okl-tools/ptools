#include "pp.h"
#include "PWriter.h"

#ifdef H7_FREE_RTOS

#include "main.h"

namespace ptools
{
    PWriterUArt logWriter;


    void uart_tx_wrapper(void* uart, const uint8_t* data, uint16_t len)
    {
        // ???
        HAL_UART_Transmit(
                (UART_HandleTypeDef*)uart,
                data,
                len,
                HAL_MAX_DELAY);
    }


}

using namespace ptools;

void prepare_the_writer(UART_HandleTypeDef*  uart)
{
    logWriter.set_context_and_func(uart, uart_tx_wrapper);
}


extern "C"
{

//typedef void (*TxFunc)(void* context, const void* data, uint32_t len);

void prepare_writer(UART_HandleTypeDef* uart)
{
    prepare_the_writer(uart);
}

}

#else

namespace ptools
{
    PWriterConsole logWriter;


    void uart_tx_wrapper(void* uart, const uint8_t* data, uint16_t len)
    {
//        HAL_UART_Transmit(
//                (UART_HandleTypeDef*)uart,
//                data,
//                len,
//                HAL_MAX_DELAY);
    }


}

using namespace ptools;

void prepare_the_writer()
{
    //logWriter.set_context_and_func(uart, uart_tx_wrapper);
}


extern "C"
{


void prepare_writer_PC()
{

}

}

#endif
