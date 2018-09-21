#pragma once
#include <time.h>
#include "Schedule.hpp"
#include "TreeGeneration.hpp"

double acceptance_condition(int energy, int new_energy, double thres);
void threshold_accepting();
