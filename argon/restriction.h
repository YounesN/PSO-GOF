#pragma once

#include <vector>

#define EPSILON_START 100
#define EPSILON_END 130
#define EPSILON_ACC 100

#define SIGMA_START 2.4
#define SIGMA_END 4.4
#define SIGMA_ACC 1000

#define N_START 12
#define N_END 18
#define N_ACC 1

#define INITIAL_TEMPERATURE 300
#define COOLING_FRACTION 0.9
#define COOLING_CRIT 0.001

#define DEN_LIQ_100 1313.7
#define DEN_LIQ_130 1068.1
#define DEN_VAP_100 16.859
#define DEN_VAP_130 103.56

extern double current_epsilon;
extern double current_sigma;
extern int current_n;
extern std::vector<int> possible_n;

extern double previous_epsilon;
extern double previous_sigma;
extern int previous_n;

extern double best_epsilon;
extern double best_sigma;
extern int best_n;

extern double current_temperature;
extern int iteration;
