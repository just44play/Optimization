#pragma once
#include <vector>
using namespace std;

class Node
{
    int id;
    int parent;
public:
    Node(int i, int p): id(i), parent(p) {}
    Node(): id(0) {}
    ~Node() {}

    int get_id();
    int get_parent();
};

int Node::get_id() { return id; }
int Node::get_parent() { return parent; }
