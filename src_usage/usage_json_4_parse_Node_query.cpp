#include "PTools.h"
#include "PFormat.h"

#include "PWriter.h"
#include "ObjectMemPool.h"
#include "PStack.h"
#include "json/Node.h"

#include "pc_tools.h" // file access: read or write


using namespace ptools;
using namespace ptools::json;


// .person.addresses[0].city
// '.Kinder[1].Instrumente'
// '.Kinder[1].Instrumente[4]'
// '.Mutter.hobby



// this works in embedded world also
static void test_parse_and_query (IObjectMemPool & pool, const char * jsonText, const char * query)
{
    NodeBuilder builder(pool);
    Scanner scanner(jsonText, string_len(jsonText), builder);
    scanner.scan_json_data();

    Node & nRoot = builder.root;

    Node * nFind = nRoot.query(query);

    if (nFind)
    {
        pp("----------");
        pp("success");
        pp("----------");
        nFind->show();
    }
    else
    {
        pp("----------");
        pp("'$' not found !", query);
    }

}


ObjectMemPoolWrapper<32, 10000, 500> pool;


int main (int argc, char * argv[])
{
    logWriter.flagWriteTrailer = false;

    if (argc != 3)
    {
        pp("$ jsonFile query", get_program(argv[0]));
        pp("$ ../src_usage/json-files/familie-stegemann5.json .Kinder[1].hobby", get_program(argv[0]));
        return 1;
    }

    std::string sFileJSON = argv[1];
    std::string sQuery = argv[2];

    std::string s = from_file(sFileJSON);
    if (s.empty())
    {
        pp("ERROR, problems with '$'", sFileJSON);
        return 1;
    }

    pp("read file '$'", sFileJSON.data());
    pp("query '$'", sQuery.data());

    const char * jsonText = s.data();
    const char * query = sQuery.data();



//    pool.show_info();

    test_parse_and_query(pool, jsonText, query);

//    pool.show_info();

    return 0;
}