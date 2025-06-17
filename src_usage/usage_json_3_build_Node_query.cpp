#include "PTools.h"
#include "ObjectMemPool.h"
#include "json/Node.h"

#include "pc_tools.h" // file access: read or write


using namespace ptools;
using namespace ptools::json;


// .person.addresses[0].city
// '.Kinder[1].Instrumente'
// '.Kinder[1].Instrumente[4]'
// '.Mutter.hobby



static void test1 (const std::string & sFile, IObjectMemPool & pool)
{
    std::string s = from_file(sFile);
    const char * jsonText = s.data();

    //pp("$", jsonText);


    NodeBuilder builder(pool);
    Scanner scanner(jsonText, string_len(jsonText), builder);
    scanner.scan_json_data();

    builder.root.show(4);

//    Node n = std::move(builder.root);
//    n.show();

//    PString s2 = node.to_json_string();
//    PString s2 = builder.root.to_json_string();

//    pp("count Nodes:$", builder.root.count_nodes()); // 136

//    pp("-------------------------$------------------------", s2);
//    pp();
//    pp("json string:$ bytes", s.size());
//    pp("-----------------");
//    pp("root, type:$", builder.root.type_as_string());
//    pp("-----------------");

    pp("------------------------------------------------------------------------------------------------");

    Node & nRoot = builder.root;


    pp("nRoot, type:$", nRoot.type_as_string());

//    const char *pKey = "name";
    const char * pKey = "Mutter";
//    const char *pKey = "Mutti";
    Node * n = nRoot.get_node(pKey);
//    Node * n = nRoot.get_node_at(1);
    if (n)
    {
        if (n->is_type_string())
        {
            pp("found:$", n->stringValue);
        }
        else if (n->is_type_object())
        {
            pp("json:$", n->to_json_string());
        }
        else if (n->is_type_array())
        {
            pp("ARRAY");
        }
    }
    else
    {
        pp("'$' not found !", pKey);
    }


}

static void test_build_node_and_query (IObjectMemPool & pool)
{
    Node nRoot(pool, NodeType::Object);

    nRoot.add("name", "Kollermann");
//    nRoot.add("größe", 2.37);
//    nRoot.add("male", true);

    Node *nMutter = nRoot.create_object("Mutter");
    nMutter->add("vorName", "Helene");
    nMutter->add("hobby", "malen");
    nMutter->add("alter", 88);

    Node *nVater = nRoot.create_object("Jürgen");
    nVater->add("hobby", "klettern");
    nVater->add_bool("sportlich", true);


    Node * nKinder = nRoot.create_array("Kinder");
    Node * nKind = nKinder->add_to_array(NodeType::Object);
    nKind->add("vorName", "Franz");
    nKind->add("alter", 23);
    nKind->add("hobby", "schwimmen");

    nKind = nKinder->add_to_array(NodeType::Object);
    nKind->add("alter", 17);
    nKind->add("vorName", "Gitte");
    nKind->add("hobby", "blödeln");

    nRoot.show();
    pp("--------------");
    pp("$", nRoot.to_json_string());
    pp("--------------");

    Node * nFind = nRoot.query(".Mutter.vorName");
    if (nFind)
    {
        nFind->show();
    }
}


ObjectMemPoolWrapper<16, 2000, 100> pool;


int main ()
{
    logWriter.flagWriteTrailer = false;


    pool.show_info();

    test_build_node_and_query(pool);

    pool.show_info();

    return 0;
}