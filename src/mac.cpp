#include "PTools.h"
#include <random>

namespace ptools
{

    uint64_t mac_to_uint64 (const uint8_t * mac)
    {
        uint64_t val = 0;

        for (int i = 0; i < 6; ++i)
        {
            val <<= 8;
            val |= mac[i];
        }

        return val;
    }

    uint64_t mac6_to_uint64 (uint8_t m0, uint8_t m1, uint8_t m2, uint8_t m3, uint8_t m4, uint8_t m5)
    {
        uint8_t ar[] = {m0, m1, m2, m3, m4, m5};

        return mac_to_uint64(ar);
    }


    void uint64_to_mac (uint64_t val, uint8_t * mac)
    {
        for (int i = 5; i >= 0; --i)
        {
            mac[i] = val & 0xFF;
            val >>= 8;
        }
    }


}