#include "TreeGeneration.hpp"

vector<Hop> generate_tree_from_file()
{
    fstream file;
    file.open("/home/dzastina/SimulatedAnnealing/Tree.txt", ios::in);
    if (file.good() == false) {
        cout << "no file!";
        exit(0);
    }
    string line;
    vector<Node> nodes;
    int id = 1;
    while (getline(file, line)) {
        nodes.push_back(Node(id, stoi(line.c_str())));
        id++;
    }

    vector<Node> nodes_temp;
    vector<Path> paths;
    id = 1;
    for (int p = 0; p < nodes.size(); p++) {
        nodes_temp.push_back(nodes[p]);
        while (nodes_temp.rbegin()->get_parent() != 0) {
            nodes_temp.push_back(nodes[nodes_temp.rbegin()->get_parent()-1]);
        }

        paths.push_back(Path(id, nodes_temp));
        id++;
        nodes_temp.clear();
    }

    vector<Hop> hops;
    id = 1;
    for (int p = 0; p < paths.size(); p++) {
        for (int n = 0; n < paths[p].get_nodes().size(); n++) {
            hops.push_back(Hop(id, paths[p], paths[p].get_node(n)));
            id++;
        }
    }

    file.close();

    return hops;
}

int random_num(int min, int max)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);

    return dis(gen);
}

double random_num_real(double min, double max)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(min, max);

    return dis(gen);
}

vector<Hop> generate_random_tree(int number_of_nodes)
{
    srand(time(0));
    vector<Node> nodes;

    nodes.push_back(Node(1, 0));

    int id = 2;
    int random_parent = 0;

    for (int i = 1; i < number_of_nodes; i++) {
        random_parent = random_num(nodes[i-1].get_parent(), nodes.size());
        nodes.push_back(Node(id, random_parent));
        cout << "id: " << id << " parent: " << nodes[i].get_parent() << endl;
        id++;
    }

    vector<Node> nodes_temp;
    vector<Path> paths;
    id = 1;
    for (int p = 0; p < nodes.size(); p++) {
        nodes_temp.push_back(nodes[p]);
        while (nodes_temp.rbegin()->get_parent() != 0) {
            nodes_temp.push_back(nodes[nodes_temp.rbegin()->get_parent()-1]);
        }

        paths.push_back(Path(id, nodes_temp));
        id++;
        nodes_temp.clear();
    }

    vector<Hop> hops;
    id = 1;
    for (int p = 0; p < paths.size(); p++) {
        for (int n = 0; n < paths[p].get_nodes().size(); n++) {
            hops.push_back(Hop(id, paths[p], paths[p].get_node(n)));
            id++;
        }
    }

    return hops;
}
