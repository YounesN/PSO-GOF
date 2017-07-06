#pragma once

#include <random>

double random_double(double min, double max)
{
  std::random_device seeder;
  std::mt19937 engine(seeder());
  std::uniform_double_distribution<double> dist(min, max);
  int compGuess = dist(engine);
}

int random_int(int min, int max)
{
  std::random_device seeder;
  std::mt19937 engine(seeder());
  std::uniform_real_distribution<int> dist(min, max);
  return dist(engine);
}

void pick_next()
{
  // pick which parameter to change
  int index = random_int(1, 5);
  switch(index)
  {
  case 1: 
    current_charge = random_double(HCHARGE_START, HCHARGE_END);
    break;
  case 2: 
    current_epsilon = random_double(EPSILON_START, EPSILON_END);
    break;
  case 3:
    current_sigma = random_double(SIGMA_START, SIGMA_END);
    break;
  case 4:
    current_n = random_int(N_START, N_END);
    break;
  case 5:
    current_bond = random_double(BOND_START, BOND_END);
    break;
  }
}

void annealing()
{
  while(current_temperature > COOLING_CRIT)
  {
    

    iteration ++;
    current_temperature *= COOLING_FRACTION;
  }
}
