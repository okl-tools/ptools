#include "PString.h"
#include "PTools.h"

namespace ptools
{

    bool AbstractModifyableString::insert_at (uint32_t posAt, const char * pPart, uint32_t countChars)
    {
        return string_insert((char *) get_data(), posAt, pPart, countChars);
    }

    bool AbstractModifyableString::remove_chars (uint32_t pos, uint32_t countChars)
    {
        return string_remove_chars((char *) get_data(), pos, countChars);
    }

    bool AbstractModifyableString::remove_first_found_char (uint32_t pos, char ch)
    {
        int i = find(ch, pos);
        if (i >= 0)
        {
            remove_chars(i);
        }
        return false;
    }

    void AbstractModifyableString::remove_from_end_CRLF ()
    {
        if (get_data() == nullptr)
        {
            pp("AbstractModifyableString::remove_from_end_CRLF, get_data == nullptr");
            return;
        }
        const int64_t sz = size();
        if (sz == 0)
        {
            return;
        }

        const char * pFirst = get_data();
        char * p = (char *) get_data() + sz;

        uint32_t pos = get_pos();
        while (p >= pFirst)
        {
            if (*p == '\r' || *p == '\n' || *p == 0)
            {
                *p = 0;
                p--;
                pos--;
            }
            else
            {
                set_pos(pos + 1);
                break;
            }
        }

    }



}