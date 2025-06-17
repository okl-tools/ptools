#pragma once

namespace ptools
{
    template<uint32_t sz>
    inline PStringFixedSize<sz> & PStringFixedSize<sz>::operator= (const char * pStr)
    {
        reset();
        write_mem(pStr, string_len(pStr));
        return *this;
    }

    template<uint32_t sz>
    inline PStringFixedSize<sz> & PStringFixedSize<sz>::operator+= (const AbstractString & pw)
    {
        write_mem(pw.get_data(), pw.size());
        return *this;
    }

    template<uint32_t sz>
    inline PStringFixedSize<sz> & PStringFixedSize<sz>::operator= (const AbstractString & pw)
    {
        reset();
        write_mem(pw.get_data(), pw.size());
        return *this;
    }

    template<uint32_t sz>
    inline PStringFixedSize<sz> & PStringFixedSize<sz>::operator+= (const char * pStr)
    {
        write_mem(pStr, string_len(pStr));
        return *this;
    }

    template<uint32_t sz>
    inline uint32_t PStringFixedSize<sz>::size () const
    {
        return string_len(mem);
    }

    template<uint32_t sz>
    inline PResult PStringFixedSize<sz>::write_mem (const char * pChars, uint32_t countChars)
    {
        if (pChars)
        {
            while (countChars > 0)
            {
                PResult res = write_char(*pChars++);
                if (res.isError())
                {
                    return res;
                }
                countChars--;
            }
        }

        return PResult();
    }

    template<uint32_t sz>
    inline PResult PStringFixedSize<sz>::write_char (char ch)
    {

        if (pWriteCursor < mem + sz)
        {
            *pWriteCursor++ = ch;
            *pWriteCursor = '\0';
        }

        return PResult();
    }

    template<uint32_t sz>
    inline const char * PStringFixedSize<sz>::to_string () const
    {
        return mem;
    }

    template<uint32_t sz>
    inline bool PStringFixedSize<sz>::operator== (const char * pStr) const
    {
        return equals(pStr);
    }

    template<uint32_t sz>
    inline void PStringFixedSize<sz>::reset ()
    {
        pWriteCursor = &mem[0];
    }

    template<uint32_t sz>
    inline const char * PStringFixedSize<sz>::get_data () const
    {
        return mem;
    }

    template<uint32_t sz>
    inline PStringFixedSize<sz>::PStringFixedSize ()
    {
        pWriteCursor = &mem[0];
    }


}