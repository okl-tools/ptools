#pragma once

#include <concepts>
#include <type_traits>
#include <iostream>
#include <limits>
#include <stdint.h>


namespace ptools
{

#if UINTPTR_MAX == 0xFFFFFFFF
    #define PLATFORM_32BIT
#elif UINTPTR_MAX == 0xFFFFFFFFFFFFFFFF
    #define PLATFORM_64BIT
#endif


    inline static const uint32_t MAX_UINT32 = 0xFFFFFFFFu;


    template <typename T>
    concept NUMBER = std::integral<T> || std::floating_point<T>;


    template<std::integral T>
    constexpr int max_digits ()
    {
        return std::numeric_limits<T>::digits10 + 2; // +1 for sign, +1 for nullterm
    }

    template<typename T>
    concept Copyable = std::is_copy_constructible_v<T>;


    template <typename T, typename Base>
    concept DERIVED = std::is_base_of_v<Base, T>;

    template<std::integral T>
    char * number_to_ascii (T val, char * str, uint32_t trailingZeros = 0);

    uint32_t string_len (const char *pStr);
    char * string_copy (char * dest, const char * src, uint32_t len=0);
    char * strcat_end (char * dest, const char * src);

    int mem_compare(const void *pMem1, size_t len1, const void *pMem2, size_t len2 );
    int string_compare(const char * pStr1,  const char * pStr2);
    int string_compare_n(const char* str1, const char* str2, uint32_t num=MAX_UINT32);
    bool string_starts_with(const char* pStr, const char* pStrWith);
    const char* string_find(const char* pStr, const char* pStrFind);
    const char* string_find_backwards(const char* pStr, const char* pStrFind);
    const char* string_find_backwards(const char* pStr, char ch, int32_t fromPos=-1);


    inline bool is_string(const char* c_str)
    {
        return false;
    }


    template<typename... Rest>
    inline bool is_string(const char* c_str, const char* test, Rest... rest)
    {
        if (!c_str || !test)
        {
            return false;
        }

        return string_compare(c_str, test) == 0 || is_string(c_str, rest...);
    }

    template<std::integral T>
    const char * get_hex(T val, char * pMem);

    template<std::integral T>
    const char * get_hex_string(T val, char * pMem);

    template<std::integral T>
    const char * get_hex_trimmed(T val, char * pMem);


    // buggy ?
    template<std::integral T>
    char * number_to_hex_ascii (T val, char * str, uint32_t trailingZeros = 0);

    char * float_to_ascii (float val, char * str, uint32_t decimals = 2);


    struct PTime
    {
        uint32_t msAbsolute = 0;

        uint32_t secTotal=0;
        uint32_t msec = 0;

        uint32_t hours = 0;
        uint32_t minutes = 0;
        uint32_t seconds = 0;

        static PTime get_time(uint32_t msAbsolute = 0);

    };

    template<NUMBER NUM>
    NUM ascii_to_number(const char * pBuf, uint32_t len);

    bool is_flag(const char * pBuf, uint32_t len=MAX_UINT32);

    enum class SHOW
    {
        BASIC,
        FULL
    };


}

#include "inline/PFormat_inline.h"
