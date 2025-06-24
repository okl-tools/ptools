#pragma once

namespace ptools
{
    template<uint32_t SZ>
    inline PStringFixedSize<SZ> & PStringFixedSize<SZ>::operator= (const char * pStr)
    {
        reset();
        write_mem(pStr, string_len(pStr));
        return *this;
    }

    template<uint32_t SZ>
    inline PStringFixedSize<SZ> & PStringFixedSize<SZ>::operator+= (const AbstractString & pw)
    {
        write_mem(pw.get_data(), pw.size());
        return *this;
    }

    template<uint32_t SZ>
    inline PStringFixedSize<SZ> & PStringFixedSize<SZ>::operator= (const AbstractString & pw)
    {
        reset();
        write_mem(pw.get_data(), pw.size());
        return *this;
    }

    template<uint32_t SZ>
    inline PStringFixedSize<SZ> & PStringFixedSize<SZ>::operator+= (const char * pStr)
    {
        write_mem(pStr, string_len(pStr));
        return *this;
    }

    template<uint32_t SZ>
    inline uint32_t PStringFixedSize<SZ>::size () const
    {
        return string_len(mem);
    }

    template<uint32_t SZ>
    inline PResult PStringFixedSize<SZ>::write_mem (const char * pChars, uint32_t countChars)
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

    template<uint32_t SZ>
    inline PResult PStringFixedSize<SZ>::write_char (char ch)
    {

        if (pWriteCursor < mem + SZ)
        {
            *pWriteCursor++ = ch;
            *pWriteCursor = '\0';
        }

        return PResult();
    }

    template<uint32_t SZ>
    inline const char * PStringFixedSize<SZ>::to_string () const
    {
//        pp("PStringFixedSize_inline::to_string, size:$", size());
        if (mem == nullptr || size() == 0)
        {
            return nullptr;
        }
        return mem;
    }

    template<uint32_t SZ>
    inline bool PStringFixedSize<SZ>::operator== (const char * pStr) const
    {
        return equals(pStr);
    }

    template<uint32_t SZ>
    inline void PStringFixedSize<SZ>::reset ()
    {
        pWriteCursor = &mem[0];
    }

    template<uint32_t SZ>
    inline const char * PStringFixedSize<SZ>::get_data () const
    {
        return mem;
    }

    template<uint32_t SZ>
    inline PStringFixedSize<SZ>::PStringFixedSize (const char *pData, uint32_t len)
    {
        memset(mem, 0, SZ);

        if (pData)
        {
            if (len >0 && len < SZ-1)
            {
                string_copy(mem, pData, len);
            }
            else
            {
                if (len == 0)
                    len = SZ-1;
                char * dest = mem;
                while (*pData && len)
                {
                    *dest++ = *pData++;
                    len--;
//                    pp("len:$", len);
                }
                *dest = 0;
            }
        }

        pWriteCursor = &mem[0];
    }


}