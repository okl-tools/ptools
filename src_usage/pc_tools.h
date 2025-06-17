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
//#include <iostream>
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

