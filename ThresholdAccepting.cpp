#include "ThresholdAccepting.hpp"

double acceptance_condition(int energy, int new_energy, double thres)
{
    if (new_energy < energy)
        return thres - 1.0;

    return new_energy - energy;
}

void threshold_accepting()
{
    double threshold = 25000;
    double threshold_factor = 0.05;
    int iter = 20000;
    int iter_r;

   // Schedule current_solution(generate_tree_from_file());
    Schedule current_solution(generate_random_tree(25));

    current_solution.mix_hops();

    Schedule best = current_solution;

    cout << "Rozwiazanie poczatkowe max_delay = "
            << best.get_max_delay() << endl;

     while (iter > 0) {
        Schedule new_solution = current_solution;

        new_solution.replace_hops();

        int current_energy = current_solution.get_energy();
        int neighbor_energy = new_solution.get_energy();

        if (acceptance_condition(current_energy, neighbor_energy, threshold) < threshold) {

            current_solution = new_solution;
            current_solution.remove_empty_slots();

            if (current_solution.get_energy() <= best.get_energy())
                best = current_solution;

        }

        threshold *= 1- threshold_factor;

        iter_r = 20000-iter;
        if (iter_r%100 == 0 or iter_r == 10 or iter_r == 20 or iter_r == 30 or
                iter_r == 40 or iter_r == 50 or iter_r == 60 or iter_r == 80) {
            cout << 20000 - iter << "..." << endl;
            cout << best.get_max_delay() << endl << endl;
            results_file(iter_r, best.get_max_delay());
        }

        iter--;
    }

    cout << "Rozwiazanie koncowe max_delay = " << best.get_max_delay() << endl;
    best.save_to_file();
}
