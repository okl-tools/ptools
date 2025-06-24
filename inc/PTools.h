// PTools.h - power tools :-)
// see https://tools.okl.de or https://github.com/okl-tools/ptools
//
// okl, 2025 May 23
//
// can be used from cortex_m7 and from linux
//
#pragma once

#include <stdint.h>
#include <stddef.h>

#include "pp.h"
#include "PFormat.h"
#include "PString.h"


namespace ptools
{
    namespace version
    {
        uint32_t major ();
        uint32_t minor ();
        uint32_t patch ();

        void to_writer(PWriter & writer);
    }

    uint64_t mac_to_uint64 (const uint8_t * mac);
    uint64_t mac6_to_uint64 (uint8_t m0, uint8_t m1, uint8_t m2, uint8_t m3, uint8_t m4, uint8_t m5);
    void uint64_to_mac (uint64_t val, uint8_t * mac);

    void rand_seed_by_milliseconds();
    int32_t rand_next_int(int32_t randMin=0, int32_t randMax=1000);

}



