#pragma once

#include <random>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include "easylogging++.h"

double random_double(double min, double max)
{
  std::random_device seeder;
  std::mt19937 engine(seeder());
  std::uniform_real_distribution<double> dist(min, max);
  return dist(engine);
}

int random_int(int min, int max)
{
  std::random_device seeder;
  std::mt19937 engine(seeder());
  std::uniform_int_distribution<> dist(min, max);
  return dist(engine);
}

void init_variables()
{
  current_epsilon = random_double(EPSILON_START, EPSILON_END);
  current_epsilon = round(current_epsilon * EPSILON_ACC) / EPSILON_ACC;
  current_sigma = random_double(SIGMA_START, SIGMA_END);
  current_sigma = round(current_sigma * SIGMA_ACC) / SIGMA_ACC;
  possible_n.clear();
  for(int i=0; i<=N_END-N_START; i++)
    possible_n.push_back(N_START+i);
  int ind = random_int(0, possible_n.size()-1);
  current_n = possible_n[ind];
  
  previous_epsilon = current_epsilon;
  previous_sigma = current_sigma;
  previous_n = current_n;
}

void pick_next()
{
  // pick which parameter to change
  //int index = random_int(1, 3);
  //switch(index)
  //{
  //case 1:
  previous_epsilon = current_epsilon;
  current_epsilon = random_double(EPSILON_START, EPSILON_END);
  current_epsilon = round(current_epsilon * EPSILON_ACC) / EPSILON_ACC;
  //break;
  //case 2:
  previous_sigma = current_sigma;
  current_sigma = random_double(SIGMA_START, SIGMA_END);
  current_sigma = round(current_sigma * SIGMA_ACC) / SIGMA_ACC;
  //break;
  // case 3:
  previous_n = current_n;
  int ind = random_int(0, possible_n.size()-1);
  current_n = possible_n[ind];
  //break;
  //}
}

void make_temp()
{
  system("rm -rf temp_100; cp -R T_100 temp_100");
  system("rm -rf temp_130; cp -R T_130 temp_130");
  
  std::stringstream stream;
  // Replace Sigma
  stream.str(std::string());
  stream << std::fixed << std::setprecision(6) << current_sigma;
  std::string sigma_rep = "sed -i 's/SSSSSSS/" + stream.str() 
    + "/g' temp_100/Liq/Par_Vrabec01_Noble_Gases_Exotic.inp";
  system(sigma_rep.c_str());
  sigma_rep = "sed -i 's/SSSSSSS/" + stream.str() 
    + "/g' temp_100/Vap/Par_Vrabec01_Noble_Gases_Exotic.inp";
  system(sigma_rep.c_str());
  sigma_rep = "sed -i 's/SSSSSSS/" + stream.str() 
    + "/g' temp_130/Liq/Par_Vrabec01_Noble_Gases_Exotic.inp";
  system(sigma_rep.c_str());
  sigma_rep = "sed -i 's/SSSSSSS/" + stream.str() 
    + "/g' temp_130/Vap/Par_Vrabec01_Noble_Gases_Exotic.inp";
  system(sigma_rep.c_str());
  
  // Replace Epsilon
  stream.str(std::string());
  stream << std::fixed << std::setprecision(6) << current_epsilon;
  std::string epsilon_rep = "sed -i 's/EEEEEEEEE/" + stream.str() 
    + "/g' temp_100/Liq/Par_Vrabec01_Noble_Gases_Exotic.inp";
  system(epsilon_rep.c_str());
  epsilon_rep = "sed -i 's/EEEEEEEEE/" + stream.str() 
    + "/g' temp_100/Vap/Par_Vrabec01_Noble_Gases_Exotic.inp";
  system(epsilon_rep.c_str());
  epsilon_rep = "sed -i 's/EEEEEEEEE/" + stream.str() 
    + "/g' temp_130/Liq/Par_Vrabec01_Noble_Gases_Exotic.inp";
  system(epsilon_rep.c_str());
  epsilon_rep = "sed -i 's/EEEEEEEEE/" + stream.str() 
    + "/g' temp_130/Vap/Par_Vrabec01_Noble_Gases_Exotic.inp";
  system(epsilon_rep.c_str());
  
  // Replace n
  stream.str(std::string());
  stream << current_n;
  std::string n_rep = "sed -i 's/NNN/" + stream.str() 
    + "/g' temp_100/Liq/Par_Vrabec01_Noble_Gases_Exotic.inp";
  system(n_rep.c_str());
  n_rep = "sed -i 's/NNN/" + stream.str() 
    + "/g' temp_100/Vap/Par_Vrabec01_Noble_Gases_Exotic.inp";
  system(n_rep.c_str());
  n_rep = "sed -i 's/NNN/" + stream.str() 
    + "/g' temp_130/Liq/Par_Vrabec01_Noble_Gases_Exotic.inp";
  system(n_rep.c_str());
  n_rep = "sed -i 's/NNN/" + stream.str() 
    + "/g' temp_130/Vap/Par_Vrabec01_Noble_Gases_Exotic.inp";
  system(n_rep.c_str());
}

std::string get_last_line_of(std::string filename, std::string substring)
{
  std::string line;
  std::string last_line;
  std::ifstream myfile(filename.c_str());
  if(!myfile)
  {
    LOG(FATAL) << "Couldn't open file " << filename << "!";
    LOG(FATAL) << "Exiting...";
  }
  while(!myfile.eof())
  {
    getline(myfile, line, '\n');
    if(line.find(substring) != -1)
      last_line = line;
  }
  return last_line;
}

double return_density(std::string filename)
{
  std::string last_line = get_last_line_of(filename, "STAT_0");
  double density;
  std::size_t found = last_line.find_last_of(" \t");
  density = std::stod(last_line.substr(found+1));
  return density;
}

double check_density_100_Liq() 
{
  std::string loc("temp_100/Liq/out.log");
  double den = return_density(loc);
  LOG(INFO) << "Expected density: " << DEN_LIQ_100;
  LOG(INFO) << "Density reported: " << den;
  double err = 0.0;
  if(den!=0.0)
    err = ((abs(DEN_LIQ_100-den))/DEN_LIQ_100)*100;
  else
    err = 99999999999.9;
  LOG(INFO) << "Error: " << err;
  return err;
}

double check_density_100_Vap()
{
  std::string loc("temp_100/Vap/out.log");
  double den = return_density(loc);
  LOG(INFO) << "Expected density: " << DEN_VAP_100;
  LOG(INFO) << "Density reported: " << den;
  double err = 0.0;
  if(den!=0.0)
    err = ((abs(DEN_VAP_100-den))/DEN_VAP_100)*100;
  else
    err = 99999999999.9;
  LOG(INFO) << "Error: " << err;
  return err;
}

double check_density_130_Liq()
{
  std::string loc("temp_130/Liq/out.log");
  double den = return_density(loc);
  LOG(INFO) << "Expected density: " << DEN_LIQ_130;
  LOG(INFO) << "Density reported: " << den;
  double err = 0.0;
  if(den!=0.0)
    err = ((abs(DEN_LIQ_130-den))/DEN_LIQ_130)*100;
  else
    err = 99999999999.9;
  LOG(INFO) << "Error: " << err;
  return err;
}

double check_density_130_Vap()
{
  std::string loc("temp_130/Vap/out.log");
  double den = return_density(loc);
  LOG(INFO) << "Expected density: " << DEN_VAP_130;
  LOG(INFO) << "Density reported: " << den;
  double err = 0.0;
  if(den!=0.0)
    err = ((abs(DEN_VAP_130-den))/DEN_VAP_130)*100;
  else
    err = 99999999999.9;
  LOG(INFO) << "Error: " << err;
  return err;
}

double objective_function()
{
  make_temp();
  
  double total_obj = 0.0;
  double curr_obj = 0.0;
  // Run T 100 Liq
  LOG(INFO) << "Running 100K Liq...";
  system("cd temp_100/Liq; ./GOMC_CPU_NPT in.conf > out.log");
  LOG(INFO) << "Finished 100K Liq!";
  curr_obj = check_density_100_Liq();
  if(curr_obj > 1.0)
    return curr_obj;
  else
    total_obj += curr_obj;
  
  // Run T 100 Vap
  LOG(INFO) << "Running 100K Vap...";
  system("cd temp_100/Vap; ./GOMC_CPU_NPT in.conf > out.log");
  LOG(INFO) << "Finished 100K Vap!";
  curr_obj = check_density_100_Vap();
  total_obj += curr_obj;
  if(curr_obj > 1.0)
    return total_obj/2;
  
  // Run T 130 Liq
  LOG(INFO) << "Running 130K Liq...";
  system("cd temp_130/Liq; ./GOMC_CPU_NPT in.conf > out.log");
  LOG(INFO) << "Finished 130K Liq!";
  curr_obj = check_density_130_Liq();
  total_obj += curr_obj;
  if(curr_obj > 1.0)
    return total_obj/3;
  
  // Run T 130 Vap
  LOG(INFO) << "Running 130K Vap...";
  system("cd temp_130/Vap; ./GOMC_CPU_NPT in.conf > out.log");
  LOG(INFO) << "Finished 130K Vap!";
  curr_obj = check_density_130_Vap();
  total_obj += curr_obj;
  return total_obj/4;
}

bool accept_prob(double delta, double current_temperature)
{
  return (exp(-1 * delta / current_temperature) > random_double(0, 1));
}

void annealing()
{
  current_temperature = INITIAL_TEMPERATURE;
  double current_obj = 0.0;
  double objective = 99999999.9;
  double delta = 0.0;
  pick_next();
  objective = objective_function();
  
  while(current_temperature > COOLING_CRIT)
  {
    LOG(INFO) << "=========== NEW ITERATION ===========";
    LOG(INFO) << "Objective: " << objective;
    LOG(INFO) << "Current temperature: " << current_temperature;
    
    pick_next();
    PrintCurrent();
    current_obj = objective_function();
    delta = current_obj - objective;
    LOG(INFO) << "Current objective: " << current_obj;
    LOG(INFO) << "DELTA: " << delta;
    if((delta<0) || (accept_prob(delta, current_temperature)))
    {
      LOG(INFO) << "Accepting new configuration";
      // if we accept, change the objective value to the new one
      objective = current_obj;
    }
    else
      {
	LOG(INFO) << "Rejecting new configuration";
	// if we reject go back to previous state
	current_epsilon = previous_epsilon;
	current_n = previous_n;
	current_sigma = previous_sigma;
      }
    
    iteration ++;
    LOG(INFO) << "iteration: " << iteration;
    current_temperature *= COOLING_FRACTION;
    LOG(INFO) << "=========== END ITERATION ===========";
  }
}
