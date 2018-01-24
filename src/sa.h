#pragma once

#include <random>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>
#include "easylogging++.h"
#include "reader.h"

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
  if(Reader::charge_enabled)
  {
    current_charge = random_double(Reader::charge_start, Reader::charge_end);
    current_charge = round(current_charge * Reader::charge_accuracy) / Reader::charge_accuracy;
  }
  else
  {
    current_charge = round(Reader::charge_value * Reader::charge_accuracy) / Reader::charge_accuracy;
  }
  if(Reader::epsilon_enabled)
  {
    current_epsilon = random_double(Reader::epsilon_start, Reader::epsilon_end);
    current_epsilon = round(current_epsilon * Reader::epsilon_accuracy) / Reader::epsilon_accuracy;
  }
  else
  {
    current_epsilon = round(Reader::epsilon_value * Reader::epsilon_accuracy) / Reader::epsilon_accuracy;
  }
  if(Reader::sigma_enabled)
  {
    current_sigma = random_double(Reader::sigma_start, Reader::sigma_end);
    current_sigma = round(current_sigma * Reader::sigma_accuracy) / Reader::sigma_accuracy;
  }
  else
  {
    current_sigma = round(Reader::sigma_value * Reader::sigma_accuracy) / Reader::sigma_accuracy;
  }
  if(Reader::n_enabled)
  {
    current_n = random_int(Reader::n_start, Reader::n_end);
  }
  else
  {
    current_n = Reader::n_value;
  }
  if(Reader::bond_enabled)
  {
    current_bond = random_double(Reader::bond_start, Reader::bond_end);
    current_bond = round(current_bond * Reader::bond_accuracy) / Reader::bond_accuracy;
  }
  else
  {
    current_bond = round(Reader::bond_value * Reader::bond_accuracy) / Reader::bond_accuracy;
  }
  
  previous_charge = current_charge;
  previous_epsilon = current_epsilon;
  previous_sigma = current_sigma;
  previous_n = current_n;
  previous_bond = current_bond;

  best_charge = current_charge;
  best_epsilon = current_epsilon;
  best_sigma = current_sigma;
  best_n = current_n;
  best_bond = current_bond;
}

void pick_next()
{
  // pick which parameter to change
  int index;
  if(Reader::charge_enabled)
    index = random_int(1, 5);
  else if(Reader::n_enabled)
    index = random_int(1, 3);
  else
    index = random_int(1, 2);
  switch(index)
  {
  case 1:
    previous_epsilon = current_epsilon;
    current_epsilon = random_double(Reader::epsilon_start, Reader::epsilon_end);
    current_epsilon = round(current_epsilon * Reader::epsilon_accuracy) / Reader::epsilon_accuracy;
    break;
  case 2:
    previous_sigma = current_sigma;
    current_sigma = random_double(Reader::sigma_start, Reader::sigma_end);
    current_sigma = round(current_sigma * Reader::sigma_accuracy) / Reader::sigma_accuracy;
    break;
  case 3:
    previous_n = current_n;
    current_n = random_int(Reader::n_start, Reader::n_end);
    break;
  case 4:
    previous_charge = current_charge;
    current_charge = random_double(Reader::charge_start, Reader::charge_end);
    current_charge = round(current_charge * Reader::charge_accuracy) / Reader::charge_accuracy;
    break;
  case 5:
    previous_bond = current_bond;
    current_bond = random_double(Reader::bond_start, Reader::bond_end);
    current_bond = round(current_bond * Reader::bond_accuracy) / Reader::bond_accuracy;
    break;
  }
}

void make_temp()
{
  std::string copy_1 = "rm -rf temp_1; cp -R " + Reader::dir1 + " temp_1";
  std::string copy_2 = "rm -rf temp_2; cp -R " + Reader::dir2 + " temp_2";
  system(copy_1.c_str());
  system(copy_2.c_str());
  
  // Replace Bond
  std::stringstream stream;
  stream << std::fixed << std::setprecision(4) << current_bond;
  std::string bond_rep = "sed -i 's/BBBBBB/" + stream.str() 
    + "/g' temp_1/Liq/" + Reader::par_file;
  system(bond_rep.c_str());
  bond_rep = "sed -i 's/BBBBBB/" + stream.str() 
    + "/g' temp_1/Vap/" + Reader::par_file;
  system(bond_rep.c_str());
  bond_rep = "sed -i 's/BBBBBB/" + stream.str() 
    + "/g' temp_2/Liq/" + Reader::par_file;
  system(bond_rep.c_str());
  bond_rep = "sed -i 's/BBBBBB/" + stream.str() 
    + "/g' temp_2/Vap/" + Reader::par_file;
  system(bond_rep.c_str());
  
  // Replace Sigma
  stream.str(std::string());
  stream << std::fixed << std::setprecision(6) << current_sigma;
  std::string sigma_rep = "sed -i 's/SSSSSSSSS/" 
    + stream.str() + "/g' temp_1/Liq/" + Reader::par_file;
  system(sigma_rep.c_str());
  sigma_rep = "sed -i 's/SSSSSSSSS/" 
    + stream.str() + "/g' temp_1/Vap/" + Reader::par_file;
  system(sigma_rep.c_str());
  sigma_rep = "sed -i 's/SSSSSSSSS/" 
    + stream.str() + "/g' temp_2/Liq/" + Reader::par_file;
  system(sigma_rep.c_str());
  sigma_rep = "sed -i 's/SSSSSSSSS/" 
    + stream.str() + "/g' temp_2/Vap/" + Reader::par_file;
  system(sigma_rep.c_str());
  
  // Replace Epsilon
  stream.str(std::string());
  stream << std::fixed << std::setprecision(6) << current_epsilon;
  std::string epsilon_rep = "sed -i 's/EEEEEEE/" + stream.str() 
    + "/g' temp_1/Liq/" + Reader::par_file;
  system(epsilon_rep.c_str());
  epsilon_rep = "sed -i 's/EEEEEEE/" + stream.str() 
    + "/g' temp_1/Vap/" + Reader::par_file;
  system(epsilon_rep.c_str());
  epsilon_rep = "sed -i 's/EEEEEEE/" + stream.str() 
    + "/g' temp_2/Liq/" + Reader::par_file;
  system(epsilon_rep.c_str());
  epsilon_rep = "sed -i 's/EEEEEEE/" + stream.str() 
    + "/g' temp_2/Vap/" + Reader::par_file;
  system(epsilon_rep.c_str());
  
  // Replace n
  stream.str(std::string());
  stream << current_n;
  std::string n_rep = "sed -i 's/NNN/" + stream.str()
    + "/g' temp_1/Liq/" + Reader::par_file;
  system(n_rep.c_str());
  n_rep = "sed -i 's/NNN/" + stream.str()
    + "/g' temp_1/Vap/" + Reader::par_file;
  system(n_rep.c_str());
  n_rep = "sed -i 's/NNN/" + stream.str()
    + "/g' temp_2/Liq/" + Reader::par_file;
  system(n_rep.c_str());
  n_rep = "sed -i 's/NNN/" + stream.str()
    + "/g' temp_2/Vap/" + Reader::par_file;
  system(n_rep.c_str());
  
  // Replace H Charges
  stream.str(std::string());
  stream << std::fixed << std::setprecision(6) << current_charge;
  std::string hcharge_rep = "sed -i 's/HHHHHHHH/" + stream.str() 
    + "/g' temp_1/Liq/" + Reader::psf_file;
  system(hcharge_rep.c_str());
  hcharge_rep = "sed -i 's/HHHHHHHH/" + stream.str() 
    + "/g' temp_1/Vap/" + Reader::psf_file;
  system(hcharge_rep.c_str());
  hcharge_rep = "sed -i 's/HHHHHHHH/" + stream.str() 
    + "/g' temp_2/Liq/" + Reader::psf_file;
  system(hcharge_rep.c_str());
  hcharge_rep = "sed -i 's/HHHHHHHH/" + stream.str() 
    + "/g' temp_2/Vap/" + Reader::psf_file;
  system(hcharge_rep.c_str());
  
  // Replace O Charges
  stream.str(std::string());
  stream << std::fixed << std::setprecision(6) << 2 * current_charge;
  std::string ocharge_rep = "sed -i 's/OOOOOOOO/" 
    + stream.str() + "/g' temp_1/Liq/" + Reader::psf_file;
  system(ocharge_rep.c_str());
  ocharge_rep = "sed -i 's/OOOOOOOO/" + stream.str() 
    + "/g' temp_1/Vap/" + Reader::psf_file;
  system(ocharge_rep.c_str());
  ocharge_rep = "sed -i 's/OOOOOOOO/" + stream.str() 
    + "/g' temp_2/Liq/" + Reader::psf_file;
  system(ocharge_rep.c_str());
  ocharge_rep = "sed -i 's/OOOOOOOO/" + stream.str() 
    + "/g' temp_2/Vap/" + Reader::psf_file;
  system(ocharge_rep.c_str());
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

double check_density_1_Liq()
{
  std::string loc("temp_1/Liq/out.log");
  double den = return_density(loc);
  LOG(INFO) << "Expected density: " << Reader::densityLiq1;
  LOG(INFO) << "Density reported: " << den;
  double err = 0.0;
  if(den!=0.0)
    err = ((abs(Reader::densityLiq1-den))/Reader::densityLiq1)*100;
  else
    err = 99999999999.9;
  LOG(INFO) << "Error: " << err;
  return err;
}

double check_density_1_Vap()
{
  std::string loc("temp_1/Vap/out.log");
  double den = return_density(loc);
  LOG(INFO) << "Expected density: " << Reader::densityVap1;
  LOG(INFO) << "Density reported: " << den;
  double err = 0.0;
  if(den!=0.0)
    err = ((abs(Reader::densityVap1-den))/Reader::densityVap1)*100;
  else
    err = 99999999999.9;
  LOG(INFO) << "Error: " << err;
  return err;
}

double check_density_2_Liq()
{
  std::string loc("temp_2/Liq/out.log");
  double den = return_density(loc);
  LOG(INFO) << "Expected density: " << Reader::densityLiq2;
  LOG(INFO) << "Density reported: " << den;
  double err = 0.0;
  if(den!=0.0)
    err = ((abs(Reader::densityLiq2-den))/Reader::densityLiq2)*100;
  else
    err = 99999999999.9;
  LOG(INFO) << "Error: " << err;
  return err;
}

double check_density_2_Vap()
{
  std::string loc("temp_2/Vap/out.log");
  double den = return_density(loc);
  LOG(INFO) << "Expected density: " << Reader::densityVap2;
  LOG(INFO) << "Density reported: " << den;
  double err = 0.0;
  if(den!=0.0)
    err = ((abs(Reader::densityVap2-den))/Reader::densityVap2)*100;
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
  // Run T 500 Liq
  LOG(INFO) << "Running 1st Liq...";
  std::string cmd = "cd temp_1/Liq/; ";
  cmd += Reader::command;
  LOG(INFO) << cmd;
  system(cmd.c_str());
  LOG(INFO) << "Finished 1st Liq!";
  curr_obj = check_density_1_Liq();
  if(curr_obj > 1.0)
    return curr_obj;
  else
    total_obj += curr_obj;
  
  // Run T 500 Vap
  LOG(INFO) << "Running 1st Vap...";
  cmd = "cd temp_1/Vap/; ";
  cmd += Reader::command;
  system(cmd.c_str());
  LOG(INFO) << "Finished 1st Vap!";
  curr_obj = check_density_1_Vap();
  total_obj += curr_obj;
  if(curr_obj > 1.0)
    return total_obj/2;
  
  // Run T 600 Liq
  LOG(INFO) << "Running 2nd Liq...";
  cmd = "cd temp_2/Liq/; ";
  cmd += Reader::command;
  system(cmd.c_str());
  LOG(INFO) << "Finished 2nd Liq!";
  curr_obj = check_density_2_Liq();
  total_obj += curr_obj;
  if(curr_obj > 1.0)
    return total_obj/3;
  else
    total_obj += curr_obj;
  
  // Run T 600 Vap
  LOG(INFO) << "Running 2nd Vap...";
  cmd = "cd temp_2/Vap/; ";
  cmd += Reader::command;
  system(cmd.c_str());
  LOG(INFO) << "Finished 2nd Vap!";
  curr_obj = check_density_2_Vap();
  total_obj += curr_obj;
  return total_obj/4;
}

bool accept_prob(double delta, double current_temperature)
{
    return (exp(-1 * delta / current_temperature) > random_double(0, 1));
}

void annealing()
{
  current_temperature = Reader::initial_temp;
  double current_obj = 0.0;
  double objective = 99999999.9;
  double delta = 0.0;
  pick_next();
  objective = objective_function();
  
  while(current_temperature > Reader::crit_temp)
  {
    LOG(INFO) << "=========== NEW ITERATION ===========";
    LOG(INFO) << "Objective: " << objective;
    LOG(INFO) << "Current temperature: " << current_temperature;
    
    pick_next();
    PrintCurrent();
    current_obj = objective_function();
    delta = current_obj - objective;
    if(current_obj < best_objective)
    {
      best_objective = current_obj;
      best_charge = current_charge;
      best_epsilon = current_epsilon;
      best_sigma = current_sigma;
      best_n = current_n;
      best_bond = current_bond;
    }
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
      current_charge = previous_charge;
      current_bond = previous_bond;
      current_epsilon = previous_epsilon;
      current_n = previous_n;
      current_sigma = previous_sigma;
    }
    
    iteration ++;
    LOG(INFO) << "iteration: " << iteration;
    current_temperature *= Reader::frac_temp;
    LOG(INFO) << "=========== END ITERATION ===========";
  }
  PrintBest();
}
