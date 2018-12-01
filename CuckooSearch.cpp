#include "CuckooSearch.hpp"
#include <cmath>

double levy_flight()
{
    double rnd = random_num_real(0.0001, 0.9999);
    return pow(rnd, -1.0/3.0);
}

void long_fly(Schedule& s)
{
    int rnd = random_num(3,8);
    while (rnd--) {
        s.replace_hops();
    }
}

Schedule get_best_nest(vector<Schedule>& n)
{
    sort(n.begin(), n.end(), [](Schedule& a, Schedule& b)
                        { return a.get_energy() < b.get_energy(); });

    return n[0];
}

void cuckoo_search()
{
    int number_of_nests = 50;
    int pa = 0.5 * number_of_nests;   // prawdopodobienstwo odrzucenia gniazda
    int pc = 0.4 * number_of_nests;
    int iter = 20000;
    int iter_r;

    Schedule sch(generate_random_tree(number_of_nests));

  //  Schedule sch(generate_tree_from_file());

    vector<Schedule> nests(number_of_nests, sch);
    for (int n = 0; n < number_of_nests; n++) { // populacja poczatkowa
        nests[n].mix_hops();
    }

    Schedule best = get_best_nest(nests);

    cout << "Rozwiazanie poczatkowe max_delay = "
         << best.get_max_delay() << endl;

    while (iter > 0) {      

        Schedule cuckoo = nests[random_num(0, pc)];

        double l = levy_flight();
        if (l > 2.0) {
            long_fly(cuckoo);
        }
        else cuckoo.replace_hops();

        int random_nest_index = random_num(0, number_of_nests-1);
        if (nests[random_nest_index].get_energy() > cuckoo.get_energy()) {
            nests[random_nest_index] = cuckoo;
            nests[random_nest_index].remove_empty_slots();
        }

        if (iter % number_of_nests == 0) {
            for (int i = number_of_nests - pa; i < number_of_nests; i++)
                long_fly(nests[i]);

            Schedule current_best = best;
            current_best = get_best_nest(nests);
            if (current_best.get_energy() < best.get_energy())
                best = current_best;
        }

        iter_r = 20000-iter;
        if (iter_r%100 == 0 or iter_r == 10 or iter_r == 20 or iter_r == 30 or
                iter_r == 40 or iter_r == 50 or iter_r == 60 or iter_r == 80) {
            cout << 20000 - iter << "..." << endl;
            cout << best.get_max_delay() << endl << endl;
        }

        iter--;
    }

    cout << "Rozwiazanie koncowe max_delay = " << best.get_max_delay() << endl;
    best.save_to_file();
}
