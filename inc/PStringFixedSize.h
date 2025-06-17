// PStringFixed.h
//
// okl, 2025 June 12
//
// Purpose: Basic string funcionality without MemoryPool
// If one or two strings are need and limits are known,
// this class fits.
// For more dynamic situations, like arrays or maps make use of PString
// ---------------------


#pragma once


#include "PString.h"


namespace ptools
{

    template<uint32_t sz=128>
    struct PStringFixedSize : AbstractString, PWriter
    {
        PStringFixedSize ();

        const char * get_data() const override;
        uint32_t size() const override;

        void reset(); // purpose: to reuse object - sets "pWriteCursor" to "mem"

        bool operator==(const char * pStr) const; // return false: if pStr==nullptr

        const char * to_string() const;
        PResult write_char (char ch) override;

        PResult write_mem (const char * pChars, uint32_t countChars) override;

        PStringFixedSize & operator  = (const char * pStr);
        PStringFixedSize & operator  = (const AbstractString & s);
        PStringFixedSize & operator += (const char * pStr);
        PStringFixedSize & operator += (const AbstractString & s);

    private:

        char mem[sz]; // memory to write into
        char *pWriteCursor = nullptr;
    };

}

#include "inline/PStringFixedSize_inline.h"