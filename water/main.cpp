#include <iostream>
#include "info.h"
#include "restriction.h"
#include "sa.h"

double current_charge;
double current_epsilon;
double current_sigma;
int current_n;
double current_bond;

double previous_charge;
double previous_epsilon;
double previous_sigma;
int previous_n;
double previous_bond;

double current_temperature;
int iteration = 0;

int main()
{
  PrintRestrictions();
  PrintAlgorithmInfo();
  init_variables();
  PrintInitial();
  objective_function();
  //annealing();
  return 0;
}
