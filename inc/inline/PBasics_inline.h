#pragma once


namespace ptools
{

    template<typename T>
    inline T min (T a, T b)
    {
        return (a < b) ? a : b;
    }

    template<typename T>
    inline T max(T a, T b)
    {
        return (a > b) ? a : b;
    }

    template<typename T>
    inline T clamp(T val, T low, T high)
    {
        return (val < low) ? low : (val > high) ? high : val;
    }


    inline bool str_equal (const char * a, const char * b)
    {
        if (a == nullptr || b == nullptr)
            return false;

        while (*a && *b)
        {
            if (*a != *b)
            {
                return false;
            }
            ++a;
            ++b;
        }
        return *a == *b;
    }

    inline int pmem_cmp(const char *pMem1, uint32_t sz1, const char *pMem2, uint32_t sz2)
    {
        if (pMem1 == nullptr)
        {
            if (pMem2 == nullptr)
            {
                return 0;
            }
            return 1;
        }

        if (pMem2 == nullptr)
        {
            return 1;
        }


        uint32_t minLen = (sz1 < sz2) ? sz1 : sz2;

        for (uint32_t i = 0; i < minLen; ++i)
        {
            if (static_cast<uint8_t>(pMem1[i]) < static_cast<uint8_t>(pMem2[i]))
            {
                return -1;
            }

            if (static_cast<uint8_t>(pMem1[i]) > static_cast<uint8_t>(pMem2[i]))
            {
                return +1;
            }
        }

        if (sz1 < sz2)
        {
            return -1;
        }

        if (sz1 > sz2)
        {
            return +1;
        }

        return 0;
    }

    inline int pmem_cmp_ignore_case(const char* pMem1, uint32_t sz1, const char* pMem2, uint32_t sz2)
    {
        if (!pMem1 || !pMem2)
        {
            return (pMem1 == pMem2) ? 0 : (pMem1 ? 1 : -1);
        }

        uint32_t minLen = (sz1 < sz2) ? sz1 : sz2;

        for (uint32_t i = 0; i < minLen; ++i)
        {
            char a = pMem1[i];
            char b = pMem2[i];

            if (a >= 'A' && a <= 'Z') a += 'a' - 'A';
            if (b >= 'A' && b <= 'Z') b += 'a' - 'A';

            if (a != b)
            {
                return (unsigned char)a - (unsigned char)b;
            }
        }

        // Wenn der gemeinsame Teil gleich ist, entscheidet die Länge
        if (sz1 == sz2)
        {
            return 0;
        }
        return (sz1 < sz2) ? -1 : 1;
    }

    inline bool is_peek(const char * p, const char * pDataEnd, char ch, uint32_t dist)
    {
        if (p == nullptr || pDataEnd == nullptr)
            return false;


        if ((p + dist) >= pDataEnd)
        {
            return false;
        }

        return *(p + dist) == ch;

    }


}