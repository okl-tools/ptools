#include "json/Node.h"

namespace ptools::json
{

    inline const char * node_type_to_String(NodeType type)
    {
        switch (type)
        {
            case NodeType::NullType: return "NullType";
            case NodeType::Boolean: return "Boolean";
            case NodeType::Number: return "Number";
            case NodeType::String: return "String";
            case NodeType::Object: return "Object";
            case NodeType::Array: return "Array";

        }

        return "unknown-type";
    }


    static void pp_info (int depth, const char * type, const char * data, size_t len)
    {
        for (int i = 0; i < depth * 4; i++)
        {
            pp_string(" ");
        }
        pp_string(type);
        pp_print_buf(data, len);
        pp("");
    }

    NodeBuilder::NodeBuilder (IObjectMemPool & pool) : pool(pool), root(pool), stackNodeContext(pool)
    {
//        root.type = NodeType::Object;

        // Start mit einem leeren Objekt
//            stackNodeContext.push(NodeContext{pool, Node(pool, NodeType::Object), "HURK"});
//            stackNodeContext.push(NodeContext{pool, Node(pool, NodeType::Object)});

    }

    void NodeBuilder::start_object ()
    {
//        pp(depth * 4, "start_object");
        depth++;
        stackNodeContext.push(NodeContext{pool, Node(pool, NodeType::Object)});
//        pp("Nodebuilder::start_object, stackNodeContext.size:$", stackNodeContext.size());
    }

    void NodeBuilder::end_object ()
    {
        //Node obj = stackNodeContext.top().node;
        NodeContext * pNC = stackNodeContext.top_object();
        if (pNC)
        {
//            Node obj = std::move(pNC->node);
            Node obj = pNC->node;
            stackNodeContext.pop();
            attach_node(std::move(obj));
        }
        else
        {
            pp("Node::end_object - No top object");
            pp(" Node::end_object - No top object");
            pp("  Node::end_object - No top object");
        }

        depth--;
//        pp(depth * 4, "end_object");
    }

    void NodeBuilder::start_array ()
    {
//        pp(depth * 4, "start_array");
        depth++;
        stackNodeContext.push(NodeContext{pool, Node(pool, NodeType::Array)});
    }

    void NodeBuilder::end_array ()
    {
        NodeContext * pNC = stackNodeContext.top_object();
        if (pNC)
        {
            Node arr = pNC->node;
            stackNodeContext.pop();
            attach_node(std::move(arr));
        }
        else
        {
            pp("NodeBuilder::end_array - No top object");
            pp(" NodeBuilder::end_array - No top object");
            pp("  NodeBuilder::end_array - No top object");
        }
        depth--;
//        pp(depth * 4, "end_array");
    }

    void NodeBuilder::key (const char * data, size_t len)
    {
//        pp_info(depth, "key:", data, len);
        NodeContext * pNC = stackNodeContext.top_object();
        if (pNC)
        {
            pNC->currentKey.set_obj_mem_pool(&pool);
            pNC->currentKey = "";
            pNC->currentKey.write_mem(data, len);
        }
        else
        {
            pp("NodeBuilder::key - No top object");
            pp(" NodeBuilder::key - No top object");
            pp("  NodeBuilder::key - No top object");
        }
    }

    void NodeBuilder::string_value (const char * data, size_t len)
    {
//        pp_info(depth, "val(string):", data, len);
        PString ps(pool);
        ps.write_mem(data, len);
        attach_node(std::move(Node(pool, NodeType::String, ps)));
    }

    void NodeBuilder::number_value (const char * data, size_t len)
    {
//        pp_info(depth, "val (number):", data, len);
        double val = ascii_to_number<double>(data, len);
        attach_node(std::move(Node(pool, NodeType::Number, val)));
    }


    void NodeBuilder::boolean_value (bool value)
    {
//        pp(depth * 4, "val (boolean): $", value);
        attach_node(std::move(Node(pool, NodeType::Boolean, value)));
    }

    void NodeBuilder::null_value ()
    {
//        pp(depth * 4, "val (null_value)");
        attach_node(std::move(Node(pool, NodeType::NullType)));
    }

    void NodeBuilder::error (const char * message, size_t position)
    {
        pp(depth * 4, "ERROR at $: $", position, message);
    }


    void NodeBuilder::attach_node (Node && n)
    {
//        pp("NodeBuilder::attach_node");
        if (stackNodeContext.is_empty())
        {
//            pp(depth*4, "n, type:$", node_type_to_String(n.type));
            root = std::move(n);
//            pp(depth*4, "EMPTY - MOVE ROOT, type:$", node_type_to_String(root.type));

            return;
        }


        NodeContext * pNC = stackNodeContext.top_object();
        if (pNC == nullptr)
        {
            pp("Node::attach_node - pNC == nullptr !");
            return;
        }

        NodeType parentType = pNC->node.type;
//        pp("NodeBuilder::attach_node, parent type:$", node_type_to_String(parentType));

        if (parentType == NodeType::Object)
        {
//            pp(depth*4, "NodeBuilder::attach_node, key <<<$>>>", pNC->currentKey);
            pNC->node.mapValues.add(pNC->currentKey, n);

            /*
            if (pNC->node.mapValues.contains(pNC->currentKey))
            {
                pNC->node.mapValues.replace(pNC->currentKey, n);
//                pNC->node.mapValues.insert(pNC->currentKey, n);
            }
            else
            {
//                pp("ATACH NODE, map:$", pNC->node.mapValues.size());
                pNC->node.mapValues.insert_copy(pNC->currentKey, n);
            }
             */
        }
        else if (parentType == NodeType::Array)
        {
            //stackNodeContext.top().node.arrayValues.push_back(std::move(n));
            pNC->node.arrayValues.push_back(std::move(n));
        }

    }


}
