#ifdef PC_VERSION

#include "PTools.h"
#include "PWriter.h"

#ifdef __linux__

#include <syscall.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <chrono>

#include <iomanip>
#include <ctime>
#include <sstream>
#endif

using PID = int32_t; // Process ID

PID process_ID ()
{

#ifdef __linux__
    return getpid();
#endif

    return 0;
}


using namespace std;

namespace ptools
{
    PWriterConsole::PWriterConsole ()
    {
        flagWriteTrailer = true;
        flagWriteTime = false;
    }


    static bool flagHeaderWritten = false;

    void PWriterConsole::write_header()
    {
//        pp("ptools version $.$.$", version::major(), version::minor(), version::patch());


        std::time_t now = std::time(nullptr);
        std::tm* local = std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(local, "%Y-%m-%d, %H:%M:%S");
        std::string sDateTime = oss.str();

//        pp("PID:$ - $", process_ID(), sDateTime);
//        pp("$, PID:$", sDateTime, process_ID());
        pp("ptools version $.$.$, $, PID:$", version::major(), version::minor(), version::patch(), sDateTime, process_ID());
//        pp("---------------------------------");


    }


    void PWriterConsole::set_write_header(bool flagHeader)
    {
        flagHeaderWritten = !flagHeader;
    }

    PResult PWriterConsole::write_char (char ch)
    {

        cout << ch;

        return PResult();
    }

    PResult PWriterConsole::write_mem (const char * pChars, uint32_t sz)
    {
        if (pChars)
        {
            PMutexLocker locker(mutexWriter);
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
            if (!flagHeaderWritten)
            {
                flagHeaderWritten = true;
                write_header();
            }

            updateTrailer();
            write_cstring(buf_trailler);
        }
    }
}

#endif