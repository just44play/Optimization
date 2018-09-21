#pragma once
#include "Path.hpp"
#include <string>

class Hop
{
    int id;
    Path path;
    Node node;
public:
    Hop(int i, Path p, Node n): id(i), path(p), node(n) {}
    Hop(): id(0) {}
    ~Hop() {}
    int get_id();
    Path get_path();
    Node get_node();
    int get_path_id();
    int get_node_id();
    string to_string_hop();
};


