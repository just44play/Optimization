#pragma once
#include "Node.hpp"

class Path
{
    int id;
    vector<Node> nodes;
public:
    Path(int i, vector<Node> n): id(i), nodes(n) {}
    Path(): id(0) {}
    ~Path() {}

    int get_id();
    int get_current_node_id();
    vector<Node> get_nodes();
    Node get_node(int position);
};

int Path::get_id() { return id; }
vector<Node> Path::get_nodes() { return nodes; }
Node Path::get_node(int position) { return nodes[position]; }
