#include "PFormat.h"
#include "PWriter.h"

namespace ptools
{

    int mem_compare(const void* pMem1, size_t len1, const void* pMem2, size_t len2)
    {
        if (!pMem1 && !pMem2)
            return 0;

        if (!pMem1)
            return -1;

        if (!pMem2)
            return 1;

        const uint8_t* a = static_cast<const uint8_t*>(pMem1);
        const uint8_t* b = static_cast<const uint8_t*>(pMem2);

        size_t minLen = (len1 < len2) ? len1 : len2;

        for (size_t i = 0; i < minLen; ++i)
        {
            if (a[i] != b[i])
            {
                return (int)a[i] - (int)b[i];
            }
        }

        if (len1 == len2)
        {
            return 0;
        }

        return (len1 < len2) ? -1 : +1;
    }

    bool string_starts_with(const char* pStr, const char* pStrWith)
    {
        if (!pStr || !pStrWith)
        {
            return false;
        }

        while (*pStrWith)
        {
            if (*pStr != *pStrWith)
            {
                return false;
            }

            ++pStr;
            ++pStrWith;
        }

        return true;
    }

    const char* string_find(const char* pStr, const char* pStrFind)
    {
        if (!pStr || !pStrFind)
        {
            return nullptr;
        }

        if (*pStrFind == '\0')
        {
            return pStr;
        }

        for (; *pStr != '\0'; ++pStr)
        {
            const char* p1 = pStr;
            const char* p2 = pStrFind;

            while (*p1 && *p2 && (*p1 == *p2))
            {
                ++p1;
                ++p2;
            }

            if (*p2 == '\0')
            {
                return pStr;
            }
        }

        return nullptr;
    }




    int string_compare(const char* pStr1, const char* pStr2)
    {
        if (!pStr1 && !pStr2)
            return 0;

        if (!pStr1)
            return -1;

        if (!pStr2)
            return 1;

        while (*pStr1 && (*pStr1 == *pStr2))
        {
            ++pStr1;
            ++pStr2;
        }

        return (unsigned char)(*pStr1) - (unsigned char)(*pStr2);
    }

    int string_compare_n(const char* str1, const char* str2, uint32_t num)
    {
//        pp("PFormat::string_compare_n , num:$", num);
        if (str1 == nullptr && str2 == nullptr)
        {
            return 0;
        }

        if (str1 == nullptr)
        {
            return -1;
        }

        if (str2 == nullptr)
        {
            return 1;
        }

        if (num == MAX_UINT32)
        {
            num = string_len(str1);
            uint32_t l2 = string_len(str2);
            if (l2 < num)
                num = l2;

        }

        for (uint32_t i = 0; i < num; ++i)
        {
            unsigned char c1 = static_cast<unsigned char>(str1[i]);
            unsigned char c2 = static_cast<unsigned char>(str2[i]);

            if (c1 != c2)
            {
                return c1 - c2;
            }

            if (c1 == '\0') // Beide gleich, und Stringende erreicht
            {
                return 0;
            }
        }

        return 0;
    }


    uint32_t string_len(const char* pStr)
    {
        uint32_t len = 0;

        if (pStr)
        {
            while (*pStr++)
            {
                ++len;
            }
        }

        return len;
    }

    char * string_copy (char * dest, const char * src, uint32_t len)
    {
        if (len)
        {
            while (*src && len)
            {
                *dest++ = *src++;
                len --;
            }

        }
        else
        {
            while (*src)
            {
                *dest++ = *src++;
            }
        }

        *dest = '\0';
        return dest;
    }

    char * strcat_end (char * dest, const char * src)
    {
        while (*dest)
            dest++;
        while (*src)
            *dest++ = *src++;
        *dest = '\0';
        return dest;
    }

    char * float_to_ascii (float val, char * str, uint32_t decimals)
    {
        // Vorzeichen behandeln
        if (val < 0.0f)
        {
            *str++ = '-';
            val = -val;
        }

        // Rundung vorbereiten: z.B. bei 2 Nachkommastellen → +0.005
        float rounding = 0.5f;
        for (uint32_t i = 0; i < decimals; ++i)
        {
            rounding /= 10.0f;
        }
        val += rounding;

        // Ganzzahliger Teil
        int intPart = static_cast<int>(val);
        str = number_to_ascii(intPart, str);

        *str++ = '.';

        // Nachkommateil
        float frac = val - intPart;
        for (uint32_t i = 0; i < decimals; ++i)
        {
            frac *= 10.0f;
            int digit = static_cast<int>(frac);
            *str++ = '0' + digit;
            frac -= digit;
        }

        *str = '\0';
        return str;
    }



    const char* string_find_backwards(const char* pStr, const char* pStrFind)
    {
        if (!pStr || !pStrFind || !*pStrFind)
        {
            return NULL;
        }

        size_t lenStr  = strlen(pStr);
        size_t lenFind = strlen(pStrFind);

        if (lenFind > lenStr)
        {
            return NULL;
        }

        // Beginne rückwärts durch den String zu iterieren
        const char* p = pStr + lenStr - lenFind;

        while (p >= pStr)
        {
            if (strncmp(p, pStrFind, lenFind) == 0)
            {
                return p;
            }
            --p;
        }

        return NULL;
    }


    const char* string_find_backwards(const char* pStr, char ch, int32_t fromPos)
    {
        if (!pStr)
        {
            return NULL;
        }

        const char* pLast = NULL;

        // Manuell bis zum Nullterminator laufen
        const char* p = pStr;
        while (*p)
        {
            ++p;
        }

        if (fromPos >=0)
        {
            const char *pTmp = pStr + fromPos;
            if (pTmp < p)
            {
                p = pTmp;
            }
        }


        // Jetzt rückwärts suchen
        while (p >= pStr)
        {
            if (*p == ch)
            {
                return p;
            }
            --p;
        }

        return NULL;
    }


}

