#include <iostream>
#include "info.h"
#include "restriction.h"

double current_charge;
double current_epsilon;
double current_sigma;
int current_n;
double current_bond;

double current_temperature;
int iteration = 0;

int main()
{
  PrintRestrictions();
  PrintAlgorithmInfo();

  return 0;
}
