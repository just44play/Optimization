#include "CuckooSearch.hpp"

int levy_flight()
{
    int levy_dist = random_num(1,100);
  //  double r = random_num_real(0.001,1.0);
  //  double levy_dist = (double) 1/r;    // funkcja 1/x niezle przybliza rozklad Levy'ego
//  /  cout << "los = " << levy_dist << endl;
    if (levy_dist > 50) return 20;
    else if (levy_dist <= 50 and levy_dist >= 20) return 40;
    else return 60;
}

void cuckoo_search()
{
    int number_of_nests = 9;
    double pa = 0.25;   // prawdopodobienstwo odrzucenia gniazda
    int iter = 500;

    //Schedule sch(generate_random_tree(25));

    Schedule sch(generate_tree_from_file());

    vector<Schedule> nests(number_of_nests, sch);
    for (int n = 0; n < number_of_nests; n++) { // populacja poczatkowa
        nests[n].mix_hops();
    }

    Schedule best = nests[0];
    for (int n = 1; n < number_of_nests; n++) {
        if (nests[n].get_energy() < best.get_energy()) {
            best = nests[n];
            swap(nests[n], nests[0]);
        }
    }
    best = nests[0];

    cout << "Rozwiazanie poczatkowe max_delay = "
         << best.get_max_delay() << endl;

    while (iter > 0) {      

        for (int n = 0; n < number_of_nests; n++) {

            Schedule cuckoo = nests[n];

            int flight = levy_flight();

            for (int l = 0; l < flight; l++) {
                int pos1 = random_num(0, cuckoo.size_of_matrix()-1);
                int pos2 = random_num(0, cuckoo.size_of_matrix()-1);
                cuckoo.replace_hops(pos1, pos2);
            }

            int random_nest_idx = random_num(0, number_of_nests-1);
            while (random_nest_idx == n)
                random_nest_idx = random_num(0, number_of_nests-1);

            if (cuckoo.get_energy() <= nests[random_nest_idx].get_energy()) {
                nests[random_nest_idx] = cuckoo;
                nests[random_nest_idx].remove_empty_slots();
            }

            iter--;
        }

        Schedule current_best = nests[0];
        for (int n = 1; n < number_of_nests; n++) {
            if (nests[n].get_energy() < current_best.get_energy()) {
                current_best = nests[n];
                swap(nests[n], nests[0]);
            }
        }
        current_best = nests[0];

        for (int n = 1; n < number_of_nests; n++) {
            if (random_num_real(0,1) < pa) {
                int pos1 = random_num(0, number_of_nests-1);
                int pos2 = random_num(0, number_of_nests-1);
                nests[n].replace_hops(pos1, pos2);
                nests[n].remove_empty_slots();
            }
        }

        for (int n = 1; n < number_of_nests; n++) {
            if (nests[n].get_energy() < current_best.get_energy()) {
                current_best = nests[n];
                swap(nests[n], nests[0]);
            }
        }
        current_best = nests[0];

        if (current_best.get_energy() < best.get_energy())
            best = current_best;

        iter--;
    }
    best.remove_empty_slots();
    cout << "Rozwiazanie koncowe max_delay = "
         << best.get_max_delay() << endl;
}
