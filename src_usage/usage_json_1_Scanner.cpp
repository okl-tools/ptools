#include "PTools.h"
#include "json/Json.h"
#include "json/SimpleHandler.h"

// to read a file
#include "pc_tools.h"

using namespace ptools;
using namespace ptools::json;


static void test_scan (const char * jsonText)
{
    SimpleHandler handler;
    json::Scanner scanner(jsonText, strlen(jsonText), handler);
    scanner.scan_json_data(); // shows the json tree structure during scanning
}

int main (int argc, char * argv[])
{
    logWriter.flagWriteTrailer = false;

    if (argc != 2)
    {
        pp("$ jsonFile", get_program(argv[0]));
        return 1;
    }

    std::string sFile = argv[1];
    std::string s = from_file(sFile);
    if (s.empty())
    {
        pp("problems with <<<$>>>", sFile);
    }
    else
    {
        test_scan(s.data());

    }

    return 0;
}


