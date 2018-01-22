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

double previous_charge;
double previous_epsilon;
double previous_sigma;
int previous_n;
double previous_bond;

double best_charge;
double best_epsilon;
double best_sigma;
int best_n;
double best_bond;

double best_objective;

double current_temperature;
int iteration = 0;

int main()
{
  LOG(INFO) << "Program started";
  LOG(INFO) << "Reading input XML";
  Reader::Init("input.xml");
  PrintRestrictions();
  PrintAlgorithmInfo();
  init_variables();
  PrintInitial();
  annealing();
  LOG(INFO) << "Program finished successfully";
  return 0;
}
