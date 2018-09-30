#include "ThresholdAccepting.hpp"

double acceptance_condition(int energy, int new_energy, double thres)
{
    if (new_energy < energy)
        return thres - 1.0;

    return new_energy - energy;
}

void threshold_accepting()
{
    double threshold = 30000;
    double threshold_factor = 0.05;
    int iter = 10000;

    Schedule current_solution(generate_tree_from_file());
   // Schedule current_solution(generate_random_tree(25));

    current_solution.mix_hops();

    Schedule best = current_solution;

    cout << "Rozwiazanie poczatkowe max_delay = "
            << best.get_max_delay() << endl;

   // best.to_string_matrix();

   // clock_t start = clock();
  //  while ( (clock() - start)/CLOCKS_PER_SEC <= 15) {
     while (iter > 0) {
        Schedule new_solution = current_solution;

        int pos1 = random_num(0, new_solution.size_of_matrix()-1);
        int pos2 = random_num(0, new_solution.size_of_matrix()-1);

        new_solution.replace_hops(pos1, pos2);

        int current_energy = current_solution.get_energy();
        int neighbor_energy = new_solution.get_energy();

        if (acceptance_condition(current_energy, neighbor_energy, threshold) < threshold) {

            current_solution = new_solution;
            current_solution.remove_empty_slots();

            if (current_solution.get_energy() <= best.get_energy())
                best = current_solution;

        }

        threshold *= 1- threshold_factor;

        if (iter % 1000 == 0) {
            cout << iter << "..." << endl;
            cout << best.get_energy() << endl << endl;;
        }

        iter--;
    }

  //  best.remove_empty_slots();
    cout << "Rozwiazanie koncowe max_delay = " << best.get_max_delay() << endl;
   // best.to_string_matrix();
    best.save_to_file();
}
