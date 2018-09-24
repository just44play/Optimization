#pragma once
#include <algorithm>
#include <array>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include "Hop.hpp"

class Schedule
{
    vector<Hop> hops;   // wektor tylko time slot
    array<vector<Hop>, 3> matrix;    // macierz channel / time slot
                                     // z gory zakladamy 2 kanaly
public:
    Schedule(vector<Hop> h): hops(h) { matrix[0] = hops;
                                       matrix[1].resize(hops.size());
                                       matrix[2].resize(hops.size()); }
    void mix_hops();
    void replace_hops(int pos1, int pos2);
    int count_delay(int path_id);   // time slot
    int get_max_delay();
    int get_energy();
    void remove_empty_slots();
    bool can_parallel(Hop h1, Hop h2);
    bool different_parents(Node n1, Node n2);
    bool half_duplex_transceiver(Node n1, Node n2);
    bool different_path(Path p1, Path p2);
    void to_string_matrix();
    void save_to_file();
    void shift_up();
    int size_of_matrix();
};
