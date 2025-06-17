// json/Node.h
//
// okl, 2025 June 12
//
// Node is a DOM (XML - Document Object Model) like structure.
// -------------------------------------------------------------------

#pragma once

#include "PTools.h"
#include "IObjectMemPool.h"
#include "Json.h"
#include "PMap.h"
#include "PArray.h"
#include "PStack.h"

namespace ptools::json
{

    enum class NodeType
    {
        NullType, Boolean, Number, String, Object, Array
    };

    struct Node
    {
    private:
        IObjectMemPool & pool;

    public:
        NodeType type = NodeType::NullType;

        // primitive values
        bool boolValue = false;
        double numberValue = 0.0;
        PString stringValue;

        // containers
        PMap<PString, Node> mapValues;
        PArray<Node> arrayValues;

        Node * get_node(const char * key) ; // returns Node found by key or nullptr
        Node * get_node_at(uint32_t index) ; // returns Node by index or nullptr

        Node * add(const char * pKey, NodeType type); // returns added Node
        Node * add(const char * pKey, const char * pStr); // returns added Node

        template<NUMBER NUM>
        Node * add(const char * pKey, const NUM val) // returns added Node
        {
            Node * n = add(pKey, NodeType::Number);
            n->numberValue = val;
            return n;
        }

        Node * add_to_array(NodeType type); // returns added Node

        Node * add_bool(const char * pKey, bool flag);
        Node * create_object(const char * pKey);
        Node * create_array(const char * pKey);

        Node (IObjectMemPool & pool);
        Node (IObjectMemPool & pool, NodeType t);
        explicit Node (IObjectMemPool & pool, NodeType t, bool boolValue);
        explicit Node (IObjectMemPool & pool, NodeType t, double numberValue);
        explicit Node (IObjectMemPool & pool, NodeType t, const PString & ps);

        Node (const Node & n);

        Node & operator= (const Node & n);

        Node (Node && n);

        Node & operator= (Node && n);


        bool is_type_primitive () const; // "bool" or "number" or "string" or "null_type"

        // Type Information
        bool is_type_bool() const;
        bool is_type_number() const;
        bool is_type_string() const;
        bool is_type_null_type() const;
        bool is_type_array () const;
        bool is_type_object () const;

        void show (uint32_t space=0) const;


        // Back to json string
        PString to_json_string (uint32_t space = 0, NodeType typeParent=NodeType::Object, bool flagRoot=true) const;

        uint32_t count_nodes() const;

        const char * type_as_string() const;

        //
        Node * query(const char *pQuery) ;

        static const char * node_type_to_String (NodeType type);

    };

    //--------------


    struct NodeBuilder : public Handler
    {
        Node root;
        IObjectMemPool & pool;

    private:
        struct NodeContext
        {
            IObjectMemPool & pool;
            Node node;
            PString currentKey;
        };

        PStack<NodeContext> stackNodeContext;

        // debug only
        int depth = 0;


    public:
        NodeBuilder (IObjectMemPool & pool);


    protected:

        void start_object () override;
        void end_object () override;

        void start_array () override;
        void end_array () override;


        void key (const char * data, size_t len) override;
        void string_value (const char * data, size_t len) override;
        void number_value (const char * data, size_t len) override;
        void boolean_value (bool value) override;
        void null_value () override;
        void error (const char * message, size_t position) override;

    private:


        void attach_node (Node && n);
    };
}
