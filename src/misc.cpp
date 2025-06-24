#include "PTools.h"
#include <cstdlib>

namespace ptools
{
    namespace version
    {
        uint32_t major ()
        {
            return 0;
        }

        uint32_t minor ()
        {
            return 4;
        }

        uint32_t patch ()
        {
            return 3;
        }

        void to_writer(PWriter & writer)
        {
            writer.sprintF("$.$.$", major(), minor(), patch());
        }

    }


    bool is_flag(const char * pBuf, uint32_t len)
    {
        uint32_t i = 0;
        // Skip leading whitespace
        while ((len == 0 && std::isspace(pBuf[i])) || (len > 0 && i < len && std::isspace(pBuf[i])))
        {
            ++i;
        }

        pBuf += i;

//        pp("-------------$-------------- len:$", pBuf, len);

        return (string_compare_n(pBuf, "true", len) == 0) ||
                (string_compare_n(pBuf, "1", len) == 0) ||
                (string_compare_n(pBuf, "ON", len) == 0) ||
                (string_compare_n(pBuf, "on", len) == 0) ||
                (string_compare_n(pBuf, "TRUE", len) == 0)||
                (string_compare_n(pBuf, "True", len) == 0);
    }

    void rand_seed_by_milliseconds()
    {
#ifdef PC_VERSION

        auto f = []() -> uint64_t
        {
            struct timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);

            uint64_t ms_now   = now.tv_sec * 1000ULL + now.tv_nsec / 1000000;
            return ms_now;

        };

        std::srand(f());
#else
        std::srand(get_milliseconds());

#endif
    }


    int32_t rand_next_int(int32_t randMin, int32_t randMax)
    {
        if (randMax <= randMin)
        {
            return randMin;
        }

        int32_t range = randMax - randMin + 1;
        return randMin + (std::rand() % range);
    }


}