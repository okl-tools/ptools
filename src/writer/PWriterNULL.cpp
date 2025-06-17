#include "PWriter.h"

#include <iostream>

using namespace std;

namespace ptools
{
    PResult PWriterNULL::write_char (char ch)
    {
        countBytesWritten++;

        return PResult();
    }

    PResult PWriterNULL::write_mem (const char * pChars, uint32_t sz)
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



}
