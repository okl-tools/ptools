#include "PString.h"

namespace ptools
{
    PStringView::PStringView ()
    {
        mem = nullptr;
        sz = 0;
    }

    PStringView::PStringView (const char * mem, uint32_t szX) : mem(mem)
    {
        if (szX == MAX_UINT32)
        {
            if (mem)
            {
                sz = strlen(mem);
            }
            else
            {
                sz = 0;
            }
        }
        else
        {
            sz = szX;
        }
    }


    PStringView::PStringView (const char * pMem, PStringView::BORDER border) : mem(pMem), sz(0)
    {
        if (border == BORDER::CRLF) // there could be other criterias to border/limit a string view
        {
            while (*pMem)
            {
                if (*pMem == '\r' || *pMem == '\n')
                {
                    break;
                }
                sz++;
                pMem++;
            }
        }
        else
        {
            if (mem)
            {
                sz = strlen(mem);
            }
        }

    }

    PStringView::PStringView (const PString & s, BORDER border):PStringView(s.get_data(), border)
    {

    }


    bool PStringView::operator== (const char * pStr) const
    {
        return equals(pStr);
    }

    bool PStringView::operator== (const PStringView & view) const
    {
        if (mem == nullptr && view.mem == nullptr)
            return true;

        return equals(view.mem, view.size());
    }


    void PStringView::to_writer (PWriter & writer) const
    {
        if (!mem)
        {
            writer.write("nullptr");
            return;
        }

        const char * p = &mem[0];
        uint32_t count = sz;
        while (count)
        {
            writer.write_char(*p);
            count--;
            p++;
        }

    }

    PString PStringView::to_PString (IObjectMemPool & pool) const
    {
        PString s(pool);
        s.write_mem(mem, sz);
        return s;
    }

    void PStringView::to_PString (PString & s) const
    {
        s.write_mem(mem, sz);
    }


    int32_t PStringView::to_int (uint32_t pos, uint32_t lenPart) const
    {
        if (mem == nullptr || size() == 0 || pos >= size())
        {
            return -1;
        }

        const char * pNumber = mem + pos;

        if (lenPart == MAX_UINT32)
        {
            lenPart = string_len(pNumber);
        }


        return ascii_to_number<int32_t>(pNumber, lenPart);
    }

    PStringView::operator bool () const
    {
        return is_valid();
    }

    uint32_t PStringView::size () const
    {
        return sz;
    }

    const char * PStringView::get_data () const
    {
        return mem;
    }



}
