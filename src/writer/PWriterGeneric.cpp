#include "PWriter.h"
#include "pp.h"

#include <iostream>

using namespace std;

namespace ptools
{
    void PWriterGeneric::set_context_and_func(void* the_conn, TxFunc userFunc)
    {
        conn = the_conn;
        txFunc = userFunc;
    }

    PResult PWriterGeneric::write_char (char ch)
    {
        char buf[1] = {ch};
        return write_mem(buf, 1);
    }

    PResult PWriterGeneric::write_mem (const char* buf, uint32_t sz)
    {

        if (txFunc && conn)
        {
            txFunc(conn, (uint8_t*)buf, sz);
        }

        return PResult();
    }



}
