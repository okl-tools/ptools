#pragma once

// For convience only in context of PC Desktop programs ( using_* testing_* )
// * load:
//   If you need some data to be parsed,
//   load it with "from_file"
//
// * save:
//   Save result of something with "to_file"
//
//
// ----------------


#include <string>
#include <sstream>
#include <fstream>

inline std::string from_file (const std::string & sFile)
{
    std::ifstream in(sFile);

    if (!in)
    {
        return "";
    }

    std::ostringstream ss;
    ss << in.rdbuf();

    return ss.str();
}

inline void to_file(const std::string& sFile, const std::string& sContents)
{
    std::ofstream out(sFile);

    if (!out)
    {
        // optional: Fehlerbehandlung oder Logging
        return;
    }

    out << sContents;
    out.close();
}

const char * get_program (const char * pPath)
{
    if (pPath == nullptr)
    {
        return "nullptr";
    }

//    const char * p = pPath  + string_len(pPath);
    const char * p = pPath  + strlen(pPath);

    while (p > pPath)
    {
        if (*p == '/' || *p== '\\')
        {
            return p+1;
        }
        p--;
    }

    return pPath;
}


