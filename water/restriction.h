#pragma once

#define HCHARGE_START 0.3
#define HCHARGE_END 0.5
#define HCHARGE_ACC 0.0001

#define EPSILON_START 50
#define EPSILON_END 200
#define EPSILON_ACC 0.01

#define SIGMA_START 2.5
#define SIGMA_END 3.5
#define SIGMA_ACC 0.001

#define N_START 8
#define N_END 10
#define N_ACC 1

#define BOND_START 1.1
#define BOND_END 9.5
#define BOND_ACC 0.0001

#define INITIAL_TEMPERATURE 400
#define COOLING_FRACTION 0.999
#define COOLING_CRIT 0.001

extern double current_charge;
extern double current_epsilon;
extern double current_sigma;
extern int current_n;
extern double current_bond;

extern double current_temperature;
extern int iteration;
