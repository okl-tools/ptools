#ifdef PC_VERSION

#include "PWriter.h"
#include <iostream>

using namespace std;

namespace ptools
{
    PResult PWriterConsole::write_char (char ch)
    {
        cout << ch;

        return PResult();
    }

    PResult PWriterConsole::write_mem (const char * pChars, uint32_t sz)
    {
        if (pChars)
        {
            while (sz > 0)
            {
                PResult res = write_char(*pChars++);
                if (res.isError())
                {
                    return res;
                }
                sz--;
            }
        }

        return PResult();
    }

    void PWriterConsole::write_trailer ()
    {
        if (flagWriteTrailer)
        {
            updateTrailer();
            write_cstring(buf_trailler);
        }
    }
}

#endif