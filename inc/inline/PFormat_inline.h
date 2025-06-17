#pragma once

#include <type_traits>


namespace ptools
{



    template<NUMBER NUM>
    inline NUM ascii_to_number(const char * pBuf, uint32_t len)
    {
        if (!pBuf)
        {
            return static_cast<NUM>(0);
        }

        NUM result = 0;
        uint32_t i = 0;
        bool isNegative = false;

        // Skip leading whitespace
        while ((len == 0 && std::isspace(pBuf[i])) || (len > 0 && i < len && std::isspace(pBuf[i])))
        {
            ++i;
        }

        // Handle optional sign (only for signed or float types)
        if constexpr (std::signed_integral<NUM> || std::floating_point<NUM>)
        {
            if ((len == 0 && pBuf[i] == '-') || (len > 0 && i < len && pBuf[i] == '-'))
            {
                isNegative = true;
                ++i;
            }
            else if ((len == 0 && pBuf[i] == '+') || (len > 0 && i < len && pBuf[i] == '+'))
            {
                ++i;
            }
        }

        bool hasDot = false;
        NUM frac = 1;

        while ((len == 0 && pBuf[i] != '\0') || (len > 0 && i < len))
        {
            char ch = pBuf[i];

            if (std::isdigit(ch))
            {
                int digit = ch - '0';
                if (hasDot)
                {
                    frac /= 10;
                    result += digit * frac;
                }
                else
                {
                    result = result * 10 + digit;
                }
            }
            else if constexpr (std::floating_point<NUM>)
            {
                if (ch == '.' && !hasDot)
                {
                    hasDot = true;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break; // Non-digit in integer mode
            }

            ++i;
        }

        if (isNegative)
        {
            result = -result;
        }

        return result;
    }


    template<std::integral T>
    inline char * number_to_ascii (T val, char * str, uint32_t trailingZeros)
    {
        char * p = str;

        if constexpr (std::is_same_v<T, bool>)
        {
            *p++ = val ? '1' : '0';
            *p = '\0';
            return p;
        }
        else
        {
            using U = typename std::make_unsigned<T>::type;

            if constexpr (std::is_signed_v<T>)
            {
                if (val < 0)
                {
                    *p++ = '-';
                    val = -val;
                }
            }

            U uval = static_cast<U>(val);

            char tmp[std::numeric_limits<U>::digits10 + 1];
            uint32_t len = 0;

            do
            {
                tmp[len++] = '0' + (uval % 10);
                uval /= 10;
            } while (uval > 0);

            while (len < trailingZeros)
            {
                *p++ = '0';
                trailingZeros--;
            }

            while (len > 0)
            {
                *p++ = tmp[--len];
            }

            *p = '\0';
            return p;
        }
    }

    template<std::integral T>
    const char * get_hex(T val, char * pMem)
    {
        static constexpr char hexDigits[] = "0123456789ABCDEF";

        constexpr int digits = sizeof(T) * 2;

        for (int i = 0; i < digits; ++i)
        {
            int shift = (digits - 1 - i) * 4;
            uint8_t nibble = (val >> shift) & 0xF;
            pMem[i] = hexDigits[nibble];
        }

        pMem[digits] = 0;
        return pMem;
    }

    template<std::integral T>
    inline const char * get_hex_trimmed(T val, char * pMem)
    {
        static constexpr char hexDigits[] = "0123456789abcdef"; // ← klein!

        constexpr int maxDigits = sizeof(T) * 2;

        pMem[0] = '0';
        pMem[1] = 'x';

        bool leading = true;
        int out = 2;

        for (int i = 0; i < maxDigits; ++i)
        {
            int shift = (maxDigits - 1 - i) * 4;
            uint8_t nibble = (val >> shift) & 0xF;

            if (nibble == 0 && leading && i < maxDigits - 1)
            {
                continue; // führende Null überspringen
            }

            leading = false;
            pMem[out++] = hexDigits[nibble];
        }

        pMem[out] = 0;
        return pMem;
    }


    template<std::integral T>
    inline const char * get_hex_string(T val, char * pMem)
    {
        static constexpr char hexDigits[] = "0123456789ABCDEF";

        constexpr int digits = sizeof(T) * 2;

        pMem[0] = '0';
        pMem[1] = 'x';

        for (int i = 0; i < digits; ++i)
        {
            int shift = (digits - 1 - i) * 4;
            uint8_t nibble = (val >> shift) & 0xF;
            pMem[2 + i] = hexDigits[nibble];
        }

        pMem[2 + digits] = 0;
        return pMem;
    }

    template<std::integral T>
    inline  char * number_to_hex_ascii (T val, char * str, uint32_t trailingZeros)
    {
        constexpr char hex_chars[] = "0123456789ABCDEF";

        // Speicher Start merken
        char * p = str;

        // Maximal 16 hex digits (64 bit), fixbuffer
        char tmp[sizeof(T) * 2];
        int len = 0;

        // Sonderfall: val == 0 → explizit behandeln
        if (val == 0)
        {
            tmp[len++] = '0';
        }
        else
        {
            while (val != 0)
            {
                tmp[len++] = hex_chars[val & 0xF];
                val >>= 4;
            }
        }

        // Mindestens `trailingZeros` auffüllen
        while ((uint32_t) len < trailingZeros)
        {
            tmp[len++] = '0';
        }

        // In richtige Reihenfolge schreiben (tmp ist reversed)
        for (int i = len - 1; i >= 0; --i)
        {
            *p++ = tmp[i];
        }

        *p = '\0'; // null-terminieren
        return p;
    }

}