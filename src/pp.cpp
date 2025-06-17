#include "pp.h"
#include "PWriter.h"



namespace ptools
{

    void pp ()
    {
        logWriter.write_trailer();
        logWriter.write_LF();
    }
    void pp (const char * pStr)
    {
        logWriter.write_trailer();
        logWriter.write_cstring(pStr);
        logWriter.write_LF();
    }
    void pp (int space, const char * pStr)
    {
        logWriter.write_trailer();
        logWriter.write_spaces(space);
        logWriter.write_cstring(pStr);
        logWriter.write_LF();

    }

}