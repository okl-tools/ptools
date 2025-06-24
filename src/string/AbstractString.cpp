#include "PString.h"
#include "PTools.h"

namespace ptools
{

    bool AbstractString::starts_with (const char * pPart) const
    {
        return string_starts_with(get_data(), pPart);
    }

    bool AbstractString::contains (const char * pPart) const
    {
        return string_find(get_data(), pPart) != nullptr;
    }

    bool AbstractString::is_valid () const
    {
        return get_data() != nullptr;
    }

    int32_t AbstractString::find_first_not_of (const char * pSearch, uint32_t pos) const
    {
        if (!pSearch || !is_valid())
        {
            return -1;
        }

        if (pos >= size())
        {
            return -1;
        }


        const char * pData = get_data();
        pData += pos;

        uint32_t iFind = MAX_UINT32;
        while (*pData)
        {
            if (strchr(pSearch, *pData) == nullptr)
            {
                iFind = pData - get_data();
                break;
            }
            pData++;
        }

        return iFind == MAX_UINT32 ?
               -1 :
               iFind;
    }


    int32_t AbstractString::find_first_of (const char * pSearch, uint32_t pos) const
    {
        if (!pSearch || !is_valid())
        {
            return -1;
        }

        if (pos >= size())
        {
            return -1;
        }


        const char * pData = get_data();
        pData += pos;

        uint32_t iFind = MAX_UINT32;
        while (*pSearch)
        {
            const char * p = strchr(pData, *pSearch);
            if (p)
            {
                uint32_t i = p - pData;
                if (iFind > i)
                {
                    iFind = i;
                }
            }
            pSearch++;
        }


        return iFind == MAX_UINT32 ?
               -1 :
               iFind + pos;
    }

    bool AbstractString::to_bool (uint32_t pos, uint32_t lenPart) const
    {
        if (get_data() == nullptr || size() == 0 || pos >= size())
        {
            return false;
        }
        return is_flag(get_data() + pos, lenPart);
    }

    int32_t AbstractString::to_int (uint32_t pos, uint32_t lenPart) const
    {
        if (get_data() == nullptr || size() == 0 || pos >= size())
        {
            return -1;
        }

        const char * pNumber = get_data() + pos;

        if (lenPart == MAX_UINT32)
        {
            lenPart = string_len(pNumber);
        }


        return ascii_to_number<int32_t>(pNumber, lenPart);
    }

    float AbstractString::to_float (uint32_t pos, uint32_t lenPart) const
    {
        if (get_data() == nullptr || size() == 0 || pos >= size())
        {
            return -1;
        }

        const char * pNumber = get_data() + pos;

        if (lenPart == MAX_UINT32)
        {
            lenPart = string_len(pNumber);
        }


        return ascii_to_number<float>(pNumber, lenPart);
    }

    bool AbstractString::empty () const
    {
        return get_data() == nullptr || size() == 0;
    }

    PStringView AbstractString::get_string_view (uint32_t posFrom, uint32_t countChars) const
    {
        if (posFrom >= size())
        {
            return PStringView(nullptr, 0);
        }

        const char * p = get_data() + posFrom;

        uint32_t partLen = size() - posFrom;
        if (countChars < partLen)
        {
            partLen = countChars;
        }

        return PStringView(p, partLen);
    }


    bool AbstractString::equals (const char * pStr, uint32_t lenStr) const
    {
        if (!pStr)
        {
            return false;
        }
        if (lenStr == MAX_UINT32)
        {
            lenStr = string_len(pStr);
        }

        return pmem_cmp(get_data(), size(), pStr, lenStr) == 0;
    }

    bool AbstractString::equals (const PStringView & view) const
    {
        return equals(view.mem, view.sz);
    }

    bool AbstractString::equals_ignore_case (const char * pStr, uint32_t lenStr) const
    {
        if (!pStr)
        {
            return false;
        }
        if (lenStr == MAX_UINT32)
        {
            lenStr = string_len(pStr);
        }

        return pmem_cmp_ignore_case(get_data(), size(), pStr, lenStr) == 0;
    }

    bool AbstractString::equals_ignore_case (const PStringView & view) const
    {
        return equals_ignore_case(view.mem, view.sz);
    }


    //


    PStringView AbstractString::get_file_extension () const
    {
        int posDot = find_backwards('.');

        pp("AbstractString::get_file_extension, posDot:$", posDot);

        if (posDot && posDot < size())
        {
            return get_string_view(posDot + 1);
        }

        return PStringView();
    }

    PStringView AbstractString::find_view (const char * pSearch) const
    {
        PStringView view;

        int32_t pos = find(pSearch);
        if (pos >= 0)
        {
            view.mem = get_data() + pos;
            view.sz = size() - pos;
        }

        return view;
    }

    int32_t AbstractString::find (const char * pSearch) const
    {
        if (get_data() == nullptr)
        {
            pp("AbstractString::find, get_data == nullptr");
            return -1;
        }
        if (pSearch == nullptr)
        {
            pp("AbstractString::find, pSearch == nullptr");
            return -1;
        }

//        const char * pFind = strstr(get_data(), pSearch);
        const char * pFind = string_find(get_data(), pSearch);
        if (!pFind)
        {
            return -1;
        }

        return pFind - get_data();
    }

    int32_t AbstractString::find (char ch, uint32_t pos) const
    {
        if (get_data() == nullptr)
        {
            pp("AbstractString::find, get_data == nullptr");
            return -1;
        }

        if (pos < MAX_UINT32 && pos >= size())
        {
            return -1;
        }


        const char * pData = get_data();
        if (pos < MAX_UINT32 && pos < size())
        {
            pData += pos;
        }


        const char * pFind = strchr(pData, ch);
        if (!pFind)
        {
            return -1;
        }

        return pFind - get_data();
    }

    int32_t AbstractString::find_backwards (const char * pSearch) const
    {
        if (get_data() == nullptr)
        {
            pp("AbstractString::find_backwards, get_data == nullptr");
            return -1;
        }
        if (pSearch == nullptr)
        {
            pp("AbstractString::find_backwards, pSearch == nullptr");
            return -1;
        }

        const char * pFind = string_find_backwards(get_data(), pSearch);
        if (!pFind)
        {
            return -1;
        }

        return pFind - get_data();
    }

    int32_t AbstractString::find_backwards (const char ch) const
    {
        if (get_data() == nullptr)
        {
            pp("AbstractString::find_backwards, get_data == nullptr");
            return -1;
        }

        const char * pFind = string_find_backwards(get_data(), ch, size());
        if (!pFind)
        {
            return -1;
        }

        return pFind - get_data();
    }

    bool AbstractString::is_file_extension (const char * c_str)
    {
        return false;
    }


}