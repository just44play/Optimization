#include "Path.hpp"

int Path::get_id() { return id; }
vector<Node> Path::get_nodes() { return nodes; }
Node Path::get_node(int position) { return nodes[position]; }
