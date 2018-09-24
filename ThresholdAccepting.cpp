#include "ThresholdAccepting.hpp"

double acceptance_condition(int energy, int new_energy, double thres)
{
    if (new_energy < energy)
        return thres + 1.0;

    return new_energy - energy;
}

void threshold_accepting()
{
    double threshold = 10000;
    double threshold_factor = 0.95;
   // int iter = 1000;

   // Schedule current_solution(generate_tree_from_file());
    Schedule current_solution(generate_random_tree(25));

    current_solution.mix_hops();
    cout << "Rozwiazanie poczatkowe max_delay = "
            << current_solution.get_max_delay() << endl;

    current_solution.to_string_matrix();

    Schedule best = current_solution;

    clock_t start = clock();
    while ( (clock() - start)/CLOCKS_PER_SEC <= 15) {
        Schedule new_solution = current_solution;

        int pos1 = random_num(0, new_solution.size_of_matrix()-1);
        int pos2 = random_num(0, new_solution.size_of_matrix()-1);

        new_solution.replace_hops(pos1, pos2);

        int current_energy = current_solution.get_energy();
        int neighbor_energy = new_solution.get_energy();

        if (acceptance_condition(current_energy, neighbor_energy, threshold) > threshold) {

            current_solution = new_solution;
        }

        if (current_solution.get_energy() < best.get_energy())
            best = current_solution;

        threshold *= threshold_factor;
      //  iter--;
    }

    best.remove_empty_slots();
    cout << "Rozwiazanie koncowe max_delay = " << best.get_max_delay() << endl;
    best.to_string_matrix();
}
