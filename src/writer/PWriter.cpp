#include "PWriter.h"

#include <iostream>

using namespace std;

namespace ptools
{
    PResult PWriter::write_LF ()
    {
        return write_char('\n');
    }
    PResult PWriter::write_CRLF ()
    {
        PResult res = write_char('\r');
        if (!res)
        {
            return res;
        }
        return write_char('\n');
    }

    PResult PWriter::write_cstring (const char * pChars)
    {
        if (pChars)
        {
            return write_mem(pChars, string_len(pChars));
        }

        return PResult();
    }

    PResult PWriter::write_spaces(uint32_t space)
    {
        while (space >0)
        {
            write_char( ' ');
            space--;
        }
        return PResult();
    }

    PResult PWriter::write (char * pStr)
    {
        return write_cstring(pStr);
    }
    PResult PWriter::write (const char * pStr)
    {
        return write_cstring(pStr);
    }

    PResult PWriter::write (const char ch)
    {
        return write_char(ch);
    }

    PResult PWriter::write (const bool flag)
    {
        return flag ?
               write("true"):
               write("false");
    }


    PResult PWriter::write (const float f, uint32_t decimals)
    {
        char bufConvert[64];
        float_to_ascii(f, bufConvert, decimals);
        return write_cstring(bufConvert);
    }

    PResult PWriter::write (const double f, uint32_t decimals)
    {
        char bufConvert[64];
        float_to_ascii(f, bufConvert, decimals);
        return write_cstring(bufConvert);
    }

    void PWriter::sprintF (const char * pStr)
    {
        write(pStr);
    }

    void PWriter::sprintLF (const char * format)
    {
        write_cstring(format);
        write_LF();
    }
    void PWriter::sprintCRLF (const char * format)
    {
        write_cstring(format);
        write_CRLF();
    }

#ifdef PC_VERSION

    PResult PWriter::write (const string & s)
    {
        write(s.data());
        return PResult();
    }
#endif


}
