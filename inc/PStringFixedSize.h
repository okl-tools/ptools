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

    template<uint32_t SZ = 128>
    struct PStringFixedSize : AbstractModifyableString, PWriter
    {
        PStringFixedSize (const char *pData= nullptr, uint32_t len=0);

        const char * get_data () const override;
        uint32_t size () const override;

        uint32_t get_pos () const override
        {
            return pWriteCursor ?
                   pWriteCursor - mem :
                   0;
        }

        void set_pos (uint32_t pos) override
        {
//            if (pWriteCursor < mem + SZ)
            if (pos < SZ)
            {
                pWriteCursor = mem + pos;
            }
        }

        uint32_t get_capacity () const
        {
            return SZ;
        }



        void reset (); // purpose: to reuse object - sets "pWriteCursor" to "mem"

        bool operator== (const char * pStr) const; // return false: if pStr==nullptr

        const char * to_string () const;
        PResult write_char (char ch) override;

        PResult write_mem (const char * pChars, uint32_t countChars) override;

        PStringFixedSize & operator= (const char * pStr);
        PStringFixedSize & operator= (const AbstractString & s);
        PStringFixedSize & operator+= (const char * pStr);
        PStringFixedSize & operator+= (const AbstractString & s);

    private:

        char mem[SZ]; // memory to write into
        char * pWriteCursor = nullptr;
    };

}

#include "inline/PStringFixedSize_inline.h"