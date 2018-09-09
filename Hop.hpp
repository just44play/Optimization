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

int Hop::get_id() { return id; }
Path Hop::get_path() { return path; }
Node Hop::get_node() { return node; }
int Hop::get_path_id() { return path.get_id(); }
int Hop::get_node_id() { return node.get_id(); }
string Hop::to_string_hop()
{
    string str = "["+to_string(get_path_id())+","+
            to_string(get_node_id())+"]";

    return str;
}
