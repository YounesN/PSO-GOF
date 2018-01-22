#include <iostream>
#include "info.h"
#include "restriction.h"
#include "sa.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

double current_charge;
double current_epsilon;
double current_sigma;
int current_n;
double current_bond;
std::vector<int> possible_n;

double previous_charge;
double previous_epsilon;
double previous_sigma;
int previous_n;
double previous_bond;

double current_temperature;
int iteration = 0;

int main()
{
  LOG(INFO) << "Program started";
  PrintRestrictions();
  PrintAlgorithmInfo();
  init_variables();
  PrintInitial();
  annealing();
  LOG(INFO) << "Program finished successfully";
  return 0;
}
