#include <iostream>
#include "SimulatedAnnealing.hpp"
#include "ThresholdAccepting.hpp"
#include "CuckooSearch.hpp"
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
    if (string(argv[1]) == "SA")
        simulated_annealing();
    else if (string(argv[1]) == "TA")
        threshold_accepting();
    else if (string(argv[1]) == "CS")
        cuckoo_search();
    else cout << "Niepoprawne parametry" << endl;


    return 0;
}
