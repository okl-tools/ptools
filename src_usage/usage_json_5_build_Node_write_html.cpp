// program needs a path to write into as argument ( html file name )
// program generates a Node ( family - german: Familie )
// program traverses Node and writes a html file
// optionally program shows the json tree
//
//

#include "PTools.h"
#include "ObjectMemPool.h"
#include "json/Node.h"

#include "pc_tools.h" // file access: read or write


using namespace ptools;
using namespace ptools::json;


static void test_build (Node & nRoot)
{
    nRoot.add("name", "Stegemann");

    Node * nMutter = nRoot.create_object("Mutter");
    nMutter->add("vorName", "Helene");
    nMutter->add("hobby", "malen");
    nMutter->add("alter", 48);

    Node * nVater = nRoot.create_object("Jürgen");
    nVater->add("hobby", "klettern");
    nVater->add_bool("sportlich", true);
    nVater->add("alter", 51);


    Node * nKinder = nRoot.create_array("Kinder");
    Node * nKind = nKinder->add_to_array(NodeType::Object);
    nKind->add("vorName", "Franz");
    nKind->add("alter", 23);
    nKind->add("hobby", "schwimmen");

    nKind = nKinder->add_to_array(NodeType::Object);
    nKind->add("alter", 17);
    nKind->add("vorName", "Gitte");
    nKind->add("hobby", "blödeln");

//    nRoot.show();
//    pp("--------------");
//    pp("$", nRoot.to_json_string());

}


void render_node_as_html (PString & s, const Node & node)
{
    using namespace ptools::json;

    if (node.is_type_object())
    {
        s << "<ul>\n";
        for (const auto & it: node.mapValues)
        {
            const char * key = it.key.c_str();
            const Node & val = it.val;

            if (val.is_type_primitive())
            {
//                os << "<li><b>" << key << ":</b> ";
                s << "<li>" << key << ": ";
                if (val.is_type_string())
                {
                    s << "\"" << val.stringValue.c_str() << "\"";
                }
                else if (val.is_type_number())
                {
                    s << val.numberValue;
                }
                else if (val.is_type_bool())
                {
                    s << (val.boolValue ?
                          "true" :
                          "false");
                }
                else
                {
                    s << "null";
                }
                s << "</li>\n";
            }
            else
            {
//                os << "<li><b>" << key << ":</b>\n";
                s << "<li>" << key << ":\n";
                render_node_as_html(s, val);
                s << "</li>\n";
            }
        }
        s << "</ul>\n";
    }
    else if (node.is_type_array())
    {
        s << "<ol>\n";
        for (const Node & item: node.arrayValues)
        {
            s << "<li>\n";
            render_node_as_html(s, item);
            s << "</li>\n";
        }
        s << "</ol>\n";
    }
    else if (node.is_type_primitive()) // falls root selbst primitive ist
    {
        if (node.is_type_string())
        {
            s << "\"" << node.stringValue.c_str() << "\"";
        }
        else if (node.is_type_number())
        {
            s << node.numberValue;
        }
        else if (node.is_type_bool())
        {
            s << node.boolValue;
//            os << (node.boolValue ? "true" : "false");
        }
        else
        {
            s << "null";
        }
    }
}

static void render_as_html_document (PString & s, const Node & root)
{
    s << "<!DOCTYPE html>\n<html>\n<head>\n";
    s << "<meta charset=\"UTF-8\">\n";
    s << "<title>JSON View</title>\n";
    s << "<style>\n";
    s << "body { font-family: sans-serif; }\n";
    s << "ul, ul ul, ul ul ul {\n";
    s << "  list-style-type: disc;\n";
    s << "  margin-left: 1em;\n";
    s << "  padding-left: 1em;\n";
    s << "}\n";
    s << "ol {\n";
    s << "  list-style-type: decimal;\n";
    s << "  margin-left: 1em;\n";
    s << "  padding-left: 1em;\n";
    s << "}\n";
    s << "li { margin: 0.3em 0; }\n";
    s << "</style>\n";
    s << "</head>\n<body>\n";
    s << "<h2>JSON</h2>\n";
    s << "<hr/>\n";

    render_node_as_html(s, root);

    s << "</body>\n</html>\n";
}


// this can run inside embedded context !
static PString test_gen_json_write_html (IObjectMemPool & pool, bool flagShowNode)
{
    Node nRoot(pool, NodeType::Object);
    test_build(nRoot);

    PString sHTML(pool);
    render_as_html_document(sHTML, nRoot);

    if (flagShowNode)
    {
        pp("Deutsche Familie - German family");
        pp("---------------------------------");
        PString sJson = nRoot.to_json_string();
        pp(sJson);
        pp("------------");
    }

    return sHTML;
}

ObjectMemPoolWrapper<32, 10000, 500> pool;

int main (int argc, char * argv[])
{
    logWriter.flagWriteTrailer = false;

    if (argc < 2)
    {
        pp("$ htmlFile [showNode]", get_program(argv[0]));
        return 1;
    }
    const std::string sFileHTML = argv[1];
    const bool flagShowNode = argc > 2;

    PString sHTML = test_gen_json_write_html(pool, flagShowNode);

    if (sHTML)
    {
        to_file(sFileHTML, sHTML.get_data());
        pp("written to: '$'", sFileHTML.data());
    }


    return 0;
}