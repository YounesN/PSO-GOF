#pragma once

#define HCHARGE_START 0.3
#define HCHARGE_END 0.5
#define HCHARGE_ACC 10000

#define EPSILON_START 50
#define EPSILON_END 200
#define EPSILON_ACC 100

#define SIGMA_START 2.5
#define SIGMA_END 3.5
#define SIGMA_ACC 1000

#define N_START 8
#define N_END 10
#define N_ACC 1

#define BOND_START 0.9
#define BOND_END 1.2
#define BOND_ACC 10000

#define INITIAL_TEMPERATURE 400
#define COOLING_FRACTION 0.999
#define COOLING_CRIT 0.001

#define DEN_LIQ_500 831.31
#define DEN_LIQ_600 649.41
#define DEN_VAP_500 13.199
#define DEN_VAP_600 72.842

extern double current_charge;
extern double current_epsilon;
extern double current_sigma;
extern int current_n;
extern double current_bond;

extern double previous_charge;
extern double previous_epsilon;
extern double previous_sigma;
extern int previous_n;
extern double previous_bond;

extern double best_charge;
extern double best_epsilon;
extern double best_sigma;
extern int best_n;
extern double best_bond;

extern double current_temperature;
extern int iteration;
