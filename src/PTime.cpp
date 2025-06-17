#include "PFormat.h"
#include "pp.h"

namespace ptools
{
    PTime PTime::get_time (uint32_t ms)
    {
        if (ms == 0)
        {
            ms = get_milliseconds();
        }

        PTime pt;

        pt.secTotal = ms / 1000;
        pt.msec = ms % 1000;

        pt.hours = (pt.secTotal / 3600) % 24;
        pt.minutes = (pt.secTotal / 60) % 60;
        pt.seconds = pt.secTotal % 60;

        return pt;
    }

}