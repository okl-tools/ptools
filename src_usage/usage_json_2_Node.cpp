#include "PTools.h"
#include "ObjectMemPool.h"
#include "json/Node.h"

#include "pc_tools.h" // file access: read or write


using namespace ptools;
using namespace ptools::json;



static void test_build_Node (IObjectMemPool & pool, const char * jsonText)
{
    NodeBuilder builder(pool);
    Scanner scanner(jsonText, string_len(jsonText), builder);
    if (!scanner.scan_json_data())
    {
        pp("usage_json_3_Node::test_build_Node - scanner problems");
        return;
    }

    Node & nRoot = builder.root; // access
    nRoot.show(4);

    pool.show_info(4);

}

ObjectMemPoolWrapper<32, 4000, 1000> pool;


int main (int argc, char * argv[])
{
    if (argc != 2)
    {
        pp("$ jsonFile", get_program(argv[0]));
        return 1;
    }

    logWriter.flagWriteTrailer = false;

    std::string sFile = argv[1];
    std::string s = from_file(sFile);

    if (!s.empty())
    {
        pool.show_info();
        test_build_Node(pool, s.data());
        pool.show_info();
    }
    else
    {
        pp("problems with file '$'", sFile);
    }


    return 0;
}
