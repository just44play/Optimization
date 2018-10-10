#include <iostream>
#include "SimulatedAnnealing.hpp"
#include "ThresholdAccepting.hpp"
#include "CuckooSearch.hpp"
#include <string>
using namespace std;

/*
WYWOLANIE PROGRAMU Z LINII KOMEND: ./SimulatedAnnealing SA  <- dla algorytmu SA,
 analogicznie TA lub CS dla pozostalych algorytmow.
Mozna tez zakomentowac warunki i odkomentowac pojedyncze wywolanie, aby uruchomic wybrany algorytm
*/

int main(int argc, char* argv[])
{
    if (string(argv[1]) == "SA")
        simulated_annealing();
    else if (string(argv[1]) == "TA")
        threshold_accepting();
    else if (string(argv[1]) == "CS")
        cuckoo_search();
    else cout << "Niepoprawne parametry" << endl;

    // simulated_annealing();
    // threshold_accepting();
    // cuckoo_search();

    return 0;
}
