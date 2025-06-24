#include "json/Node.h"

namespace ptools::json
{
    struct JToken : PStringView
    {

        JToken (const char * mem, uint32_t szX = MAX_UINT32)
        {
            this->mem = mem;
            if (szX == MAX_UINT32)
            {
                sz = strlen(mem);
            }
            else
            {
                sz = szX;
            }
        }


        uint32_t index = MAX_UINT32;

        bool is_array_index () const
        {
            return index < MAX_UINT32;
        }

        void to_writer (PWriter & writer) const
        {
            if (is_array_index())
            {
                writer.sprintF("index:$", index);
            }
            else
            {
                writer.sprintF("key:'");
                writer.write_mem(get_data(), size());
                writer.sprintF("'");
            }
        }

    private:
        JToken () = delete;

    };


    const char * Node::node_type_to_String (NodeType type)
    {
        switch (type)
        {
            case NodeType::NullType:
                return "NullType";
            case NodeType::Boolean:
                return "Boolean";
            case NodeType::Number:
                return "Number";
            case NodeType::String:
                return "String";
            case NodeType::Object:
                return "Object";
            case NodeType::Array:
                return "Array";

        }

        return "unknown-type";
    }

    Node::Node (IObjectMemPool & pool) : pool(pool), stringValue(pool), mapValues(pool), arrayValues(pool)
    {

    }

    Node::Node (IObjectMemPool & pool, NodeType t) : pool(pool), stringValue(pool), type(t), mapValues(pool), arrayValues(pool)
    {
    }

    Node::Node (IObjectMemPool & pool, NodeType t, bool boolValue) : pool(pool), stringValue(pool), type(t), boolValue(boolValue), mapValues(pool), arrayValues(pool)
    {
    }

    Node::Node (IObjectMemPool & pool, NodeType t, double numberValue) : pool(pool), stringValue(pool), type(t), numberValue(numberValue), mapValues(pool), arrayValues(pool)
    {
    }

//
    Node::Node (IObjectMemPool & pool, NodeType t, const PString & ps) : pool(pool), stringValue(pool), type(t), numberValue(numberValue), mapValues(pool), arrayValues(pool)
    {
        stringValue = ps;
    }

    Node::Node (const Node & n) : pool(n.pool), stringValue(pool), mapValues(n.pool), arrayValues(n.pool)
    {
        type = n.type;
        boolValue = n.boolValue;
        numberValue = n.numberValue;
        if (n.stringValue.is_valid())
        {
            stringValue.sprintF("$", n.stringValue.to_string());
        }
        mapValues = n.mapValues;
        arrayValues = n.arrayValues;
    }

    Node & Node::operator= (const Node & n)
    {
        type = n.type;
        boolValue = n.boolValue;
        numberValue = n.numberValue;
        if (n.stringValue.is_valid())
        {
            stringValue.sprintF("$", n.stringValue.to_string());
        }
        mapValues = n.mapValues;
        arrayValues = n.arrayValues;
        return *this;
    }

    Node::Node (Node && n) : pool(n.pool), stringValue(pool), mapValues(n.pool), arrayValues(n.pool)
    {

//        type = n.type;
//        boolValue = n.boolValue;
//        numberValue = n.numberValue;
//        if (n.stringValue.is_valid())
//        {
//            stringValue.sprintF("$", n.stringValue.to_string());
//        }
//        mapValues = n.mapValues;
//        arrayValues = n.arrayValues;

//        pp("MOVE CONTRUCTOR - NODE, pool:$", (const void*) &pool);


        std::swap(type, n.type);
        std::swap(boolValue, n.boolValue);
        std::swap(numberValue, n.numberValue);
        std::swap(stringValue, n.stringValue);
        std::swap(mapValues, n.mapValues);
        std::swap(arrayValues, n.arrayValues);

    }

    Node & Node::operator= (Node && n)
    {
//        throw std::runtime_error("ASIGNMENT ERR");

        pool = n.pool;

//        pp("MOVE ASSIGNMENT - NODE, pool:$", (const void*) &pool);


        std::swap(type, n.type);
        std::swap(boolValue, n.boolValue);
        std::swap(numberValue, n.numberValue);
        std::swap(stringValue, n.stringValue);
        std::swap(mapValues, n.mapValues);
        std::swap(arrayValues, n.arrayValues);

        return *this;
    }

    void Node::show (uint32_t space) const
    {
        //pp("Node::show, type:$", node_type_to_String(type));
        if (is_type_primitive())
        {
            if (type == NodeType::Boolean)
            {
                pp(space, "boolValue:$", boolValue);
            }
            else if (type == NodeType::Number)
            {
                pp(space, "numberValue:$", numberValue);
            }
            else if (type == NodeType::String)
            {
                pp(space, "string:'$'", stringValue);
            }
            else if (type == NodeType::NullType)
            {
                pp(space, "NullType");
            }
        }
        else if (is_type_object())
        {
//            pp("object:$", mapValues.size());
            for (const auto & it: mapValues)
            {
                const PString & key = it.key;
                pp(space, "$", key);
                const Node & node = it.val;
                node.show(space + 4);
            }
        }
        else if (is_type_array())
        {
            pp(space, "[$]", arrayValues.size());
            for (const Node & node: arrayValues)
            {
                pp(space + 2, "object");
                node.show(space + 4);
            }
        }
    }

    PString Node::to_json_string (uint32_t space, NodeType typeParent, bool flagRoot) const
    {
        PString sOut(pool);


        if (type == NodeType::NullType)
        {
            sOut = "null-type";
        }
        else if (type == NodeType::Boolean)
        {
            sOut = boolValue ?
                   "true" :
                   "false";
        }
        else if (type == NodeType::Number)
        {
            sOut.sprintF("$", numberValue);
        }
        else if (type == NodeType::String)
        {
            sOut.sprintF("\"$\"", stringValue.to_string());
        }
        else if (type == NodeType::Array)
        {
            sOut += "\n";
            sOut.write_spaces(space);
            sOut += "[";
            sOut += "\n";

            for (size_t i = 0; i < arrayValues.size(); i++)
            {
                const Node & rNode = arrayValues[i];

                sOut.write_spaces(space + 4);
                sOut.sprintF("$", rNode.to_json_string(space + 4, type, false).to_string());

                if (i + 1 < arrayValues.size())
                {
                    sOut += ",";
                }

                sOut += "\n";
            }

            sOut.write_spaces(space);
            sOut += "]";
        }
        else if (type == NodeType::Object)
        {
            if (typeParent == NodeType::Object)
            {
                if (!flagRoot)
                {
                    sOut += "\n";
                }
                sOut.write_spaces(space);
                sOut += "{\n";
            }
            else
            {
                sOut += "{\n";

            }


            const auto & rItems = mapValues;

            for (size_t i = 0; i < rItems.size(); i++)
            {
                const auto & rKV = rItems.vItems[i];

                sOut.write_spaces(space + 4);
                sOut.sprintF("\"$\" : ", rKV.key.to_string());
                sOut.sprintF("$", rKV.val.to_json_string(space + 4, type, false).to_string());


                if (i + 1 < rItems.size())
                {
                    sOut += ",";
                }

                sOut += "\n";
            }
            sOut.write_spaces(space);
            sOut += "}";


            //sOut += sPad + "}";
        }

        return sOut;
    }

    static void walk_nodes (const Node & nParent, uint32_t & count)
    {
        if (nParent.type == NodeType::Array)
        {
            for (size_t i = 0; i < nParent.arrayValues.size(); i++)
            {
                const Node & rNode = nParent.arrayValues[i];
                walk_nodes(rNode, count);
            }
        }
        else if (nParent.type == NodeType::Object)
        {
            const auto & rItems = nParent.mapValues;
            for (size_t i = 0; i < rItems.size(); i++)
            {
                const auto & rKV = rItems.vItems[i];
                const Node & rNode = rKV.val;
                walk_nodes(rNode, count);
            }
        }

        count++;
    }

    uint32_t Node::count_nodes () const
    {
        uint32_t count = 0;
        walk_nodes(*this, count);
        return count;
    }

    const char * Node::type_as_string () const
    {
        switch (type)
        {
            case NodeType::NullType:
                return "NullType";
            case NodeType::Boolean:
                return "Boolean";
            case NodeType::Number:
                return "Number";
            case NodeType::String:
                return "String";
            case NodeType::Object:
                return "Object";
            case NodeType::Array:
                return "Array";

        }

        return "unknown-type";
    }

    Node * Node::get_node (const char * key)
    {
        PString s(pool, key);
        return mapValues.find(s);
    }

    Node * Node::get_node_at (uint32_t index)
    {
        if (index >= arrayValues.size())
        {
            return nullptr;
        }
        return &arrayValues[index];
    }

    bool Node::is_type_primitive () const
    {
        return type == NodeType::NullType || type == NodeType::Boolean || type == NodeType::Number || type == NodeType::String;
    }

    bool Node::is_type_bool () const
    {
        return type == NodeType::Boolean;
    }

    bool get_query_token (const PString & path, PArray<JToken> & arToks)
    {

        size_t i = 0;
        while (i < path.size())
        {
            if (path[i] == '.')
            {
                ++i;
                size_t start = i;
                while (i < path.size() && path[i] != '.' && path[i] != '[')
                    ++i;

                JToken key = path.create_view<JToken>(start, i - start);
                if (!key.empty())
                {
                    arToks.push_back(key);
                }
            }
            else if (path[i] == '[')
            {
                ++i;
                size_t start = i;
                while (i < path.size() && path[i] != ']')
                    ++i;

                if (i >= path.size())
                {
                    break;
                }

//            PStringView key = path.get_string_view(start, i - start);
                JToken key = path.create_view<JToken>(start, i - start);
                key.index = key.to_int();

                arToks.push_back(key);

                ++i; // skip ']'
            }
            else
            {
                ++i; // skip anything invalid
            }
        }
        return true;
    }

    bool Node::is_type_number () const
    {
        return type == NodeType::Number;
    }

    bool Node::is_type_string () const
    {
        return type == NodeType::String;
    }

    bool Node::is_type_null_type () const
    {
        return type == NodeType::NullType;
    }

    bool Node::is_type_array () const
    {
        return type == NodeType::Array;
    }

    bool Node::is_type_object () const
    {
        return type == NodeType::Object;
    }

    Node * Node::query (const char * pQuery)
    {
        if (pQuery == nullptr)
        {
            pp("Node::query, pQuery == nullptr !");
            return nullptr;
        }

        PArray<JToken> arToks(pool);

        PString sQuery(pool, pQuery);
        get_query_token(sQuery, arToks);

        Node * n = this;
//        for (const JToken & tok: arToks)
        for (uint32_t i = 0; n && i < arToks.size(); i++)
        {
            const JToken & tok = arToks[i];
            if (tok.is_array_index())
            {
//                pp("--- [$] ---", tok.index);
                n = n->get_node_at(tok.index);
            }
            else
            {
                PString key = tok.to_PString(pool);
//                pp("--- $ ---", key);
                n = n->get_node(key.to_string());
            }

//            pp("$", tok);
        }

        return n;
    }

    Node * Node::add (const char * pKey, NodeType type)
    {
        Node n(pool, type);
        PString key(pool, pKey);
        mapValues.add(key, n);
        return mapValues.find(key);
    }

    Node * Node::add_to_array (NodeType type)
    {
        Node n(pool, type);
        arrayValues.push_back(n);
        return arrayValues.last();
    }

    Node * Node::add (const char * pKey, const char * pStr)
    {
        Node * n = add(pKey, NodeType::String);
        n->stringValue = pStr;
        return n;
    }

    Node * Node::add_bool (const char * pKey, bool flag)
    {
        Node * n = add(pKey, NodeType::Boolean);
        n->boolValue = flag;
        return n;
    }

    Node * Node::create_object (const char * pKey)
    {
        return add(pKey, NodeType::Object);
    }

    Node * Node::create_array (const char * pKey)
    {
        return add(pKey, NodeType::Array);
    }


}
