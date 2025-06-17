#include "PWriter.h"
#include "pp.h"

#include <iostream>

using namespace std;

namespace ptools
{
    void PTrailer::updateTrailer ()
    {
        buf_trailler[0] = 0;
        counterLogMessage++;


        char * p = strcat_end(buf_trailler, "[");
        p = number_to_ascii(counterLogMessage, p, 6);


        if (flagWriteTime)
        {
            p = strcat_end(buf_trailler, "] [");
            PTime time = PTime::get_time();

            p = number_to_ascii(time.hours, p, 2);
            p = strcat_end(buf_trailler, ":");

            p = number_to_ascii(time.minutes, p, 2);
            p = strcat_end(buf_trailler, ":");

            p = number_to_ascii(time.seconds, p, 2);
            p = strcat_end(buf_trailler, ",");

            p = number_to_ascii(time.msec, p, 3);
    }


#if defined(CORTEX_M7_NETX_DUO) || defined(CORTEX_M7_FREE_RTOS) || defined(H7_FREE_RTOS)
        p = strcat_end(p, "][");
        p = number_to_ascii(get_thread_ID(), p, 4);
        p = strcat_end(p, "] ");
#else
        p = strcat_end(p, "] ");
#endif

    }



}
