#include "CuckooSearch.hpp"

int levy_flight()
{
    double r = random_num_real(0.001,5.000);
    double levy_dist = (double) 1/r;    // funkcja 1/x niezle przybliza rozklad Levy'ego

    if (levy_dist > 1.0) return 1;
    else if (levy_dist >= 0.5 and levy_dist <= 1.0) return 2;
    else return 3;
}

void cuckoo_search()
{
    int number_of_nests = 10;
    double pa = 0.25;   // prawdopodobienstwo odrzucenia gniazda
    int iter = 60000;

    //Schedule sch(generate_random_tree(25));

    Schedule sch(generate_tree_from_file());

    vector<Schedule> nests(number_of_nests, sch);
    for (int n = 0; n < number_of_nests; n++) { // populacja poczatkowa
        nests[n].mix_hops();
    }

    sort(nests.begin(), nests.end(),
         [](Schedule& a, Schedule& b){ return a.get_energy() < b.get_energy();});

    Schedule best = nests[0];

    cout << "Rozwiazanie poczatkowe max_delay = "
         << best.get_max_delay() << endl;

   // best.to_string_matrix();

  //  clock_t start = clock();
  //  while ( (clock() - start)/CLOCKS_PER_SEC <= 10) {
    while (iter > 0) {

        if (iter % 1000 == 0) {
            cout << iter << "..." << endl;
            cout << best.get_energy() << endl << endl;
        }

        Schedule cuckoo = best;

        for (int l = 0; l < levy_flight(); l++) {
            int pos1 = random_num(0, cuckoo.size_of_matrix()-1);
            int pos2 = random_num(0, cuckoo.size_of_matrix()-1);
            cuckoo.replace_hops(pos1, pos2);
        }

        int current_nest_idx = random_num(0, number_of_nests-1);

        if (cuckoo.get_energy() < nests[current_nest_idx].get_energy()) {
            nests[current_nest_idx] = cuckoo;
            nests[current_nest_idx].remove_empty_slots();
        }

        sort(nests.begin(), nests.end(),
             [](Schedule& a, Schedule& b){ return a.get_energy() < b.get_energy();});

        if (nests[0].get_energy() < best.get_energy())
            best = nests[0];

        for (int n = (int)(number_of_nests-number_of_nests*pa); n < number_of_nests; n++) {
            for (int l = 0; l < levy_flight(); l++) {
                int pos1 = random_num(0, number_of_nests-1);
                int pos2 = random_num(0, number_of_nests-1);
                nests[n].replace_hops(pos1, pos2);
            }
        }

        iter--;
    }
    best.remove_empty_slots();
    cout << "Rozwiazanie koncowe max_delay = "
         << best.get_max_delay() << endl;
  //  best.to_string_matrix();
}
