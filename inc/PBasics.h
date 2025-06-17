#pragma once

#include <stdint.h>



#include <cstring>
#include <cstdio>
#include <stdint.h>
#include <concepts>
#include <cctype>

namespace ptools
{

    template<typename T>
    T min(T a, T b);

    template<typename T>
    T max(T a, T b);


    template <typename T>
    concept TOSTRING = requires(T t)
    {
        { t.to_string() } -> std::same_as<const char*>;
    };


    template<typename T>
    T clamp(T val, T low, T high);


    bool str_equal (const char * a, const char * b);
    bool is_peek(const char * p, const char * pDataEnd, char ch, uint32_t dist = 1);

    int pmem_cmp(const char *pMem1, uint32_t sz1, const char *pMem2, uint32_t sz2);
    int pmem_cmp_ignore_case(const char* pMem1, uint32_t sz1, const char* pMem2, uint32_t sz2);



}

#include "inline/PBasics_inline.h"