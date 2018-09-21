#pragma once
#include <cmath>
#include "Schedule.hpp"
#include "TreeGeneration.hpp"

double acceptance_probability(int energy, int new_energy, double temperature);
void simulated_annealing();
