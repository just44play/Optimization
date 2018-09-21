#include "Hop.hpp"

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
