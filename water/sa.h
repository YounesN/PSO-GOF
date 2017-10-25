#pragma once

#include <random>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>
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
    current_charge = random_double(HCHARGE_START, HCHARGE_END);
    current_charge = round(current_charge * HCHARGE_ACC) / HCHARGE_ACC;
    current_epsilon = random_double(EPSILON_START, EPSILON_END);
    current_epsilon = round(current_epsilon * EPSILON_ACC) / EPSILON_ACC;
    current_sigma = random_double(SIGMA_START, SIGMA_END);
    current_sigma = round(current_sigma * SIGMA_ACC) / SIGMA_ACC;
    current_n = random_int(N_START, N_END);
    current_bond = random_double(BOND_START, BOND_END);
    current_bond = round(current_bond * BOND_ACC) / BOND_ACC;

    previous_charge = current_charge;
    previous_epsilon = current_epsilon;
    previous_sigma = current_sigma;
    previous_n = current_n;
    previous_bond = current_bond;
}

void pick_next()
{
    // pick which parameter to change
    int index = random_int(1, 5);
    switch(index)
    {
    case 1:
        previous_charge = current_charge;
        current_charge = random_double(HCHARGE_START, HCHARGE_END);
        current_charge = round(current_charge * HCHARGE_ACC) / HCHARGE_ACC;
        break;
    case 2:
        previous_epsilon = current_epsilon;
        current_epsilon = random_double(EPSILON_START, EPSILON_END);
        current_epsilon = round(current_epsilon * EPSILON_ACC) / EPSILON_ACC;
        break;
    case 3:
        previous_sigma = current_sigma;
        current_sigma = random_double(SIGMA_START, SIGMA_END);
        current_sigma = round(current_sigma * SIGMA_ACC) / SIGMA_ACC;
        break;
    case 4:
        previous_n = current_n;
        current_n = random_int(N_START, N_END);
        break;
    case 5:
        previous_bond = current_bond;
        current_bond = random_double(BOND_START, BOND_END);
        current_bond = round(current_bond * BOND_ACC) / BOND_ACC;
        break;
    }
}

void make_temp()
{
    system("rm -rf temp_500; cp -R T_500 temp_500");
    system("rm -rf temp_600; cp -R T_600 temp_600");

    // Replace Bond
    std::stringstream stream;
    stream << std::fixed << std::setprecision(4) << current_bond;
    std::string bond_rep = "sed -i 's/BBBBBB/" + stream.str() + "/g' temp_500/Liq/par_water.inp";
    system(bond_rep.c_str());
    bond_rep = "sed -i 's/BBBBBB/" + stream.str() + "/g' temp_500/Vap/par_water.inp";
    system(bond_rep.c_str());
    bond_rep = "sed -i 's/BBBBBB/" + stream.str() + "/g' temp_600/Liq/par_water.inp";
    system(bond_rep.c_str());
    bond_rep = "sed -i 's/BBBBBB/" + stream.str() + "/g' temp_600/Vap/par_water.inp";
    system(bond_rep.c_str());
    

    // Replace Sigma
    stream.str(std::string());
    stream << std::fixed << std::setprecision(6) << current_sigma;
    std::string sigma_rep = "sed -i 's/SSSSSSSSS/" + stream.str() + "/g' temp_500/Liq/par_water.inp";
    system(sigma_rep.c_str());
    sigma_rep = "sed -i 's/SSSSSSSSS/" + stream.str() + "/g' temp_500/Vap/par_water.inp";
    system(sigma_rep.c_str());
    sigma_rep = "sed -i 's/SSSSSSSSS/" + stream.str() + "/g' temp_600/Liq/par_water.inp";
    system(sigma_rep.c_str());
    sigma_rep = "sed -i 's/SSSSSSSSS/" + stream.str() + "/g' temp_600/Vap/par_water.inp";
    system(sigma_rep.c_str());

    // Replace Epsilon
    stream.str(std::string());
    stream << std::fixed << std::setprecision(6) << current_epsilon;
    std::string epsilon_rep = "sed -i 's/EEEEEEE/" + stream.str() + "/g' temp_500/Liq/par_water.inp";
    system(epsilon_rep.c_str());
    epsilon_rep = "sed -i 's/EEEEEEE/" + stream.str() + "/g' temp_500/Vap/par_water.inp";
    system(epsilon_rep.c_str());
    epsilon_rep = "sed -i 's/EEEEEEE/" + stream.str() + "/g' temp_600/Liq/par_water.inp";
    system(epsilon_rep.c_str());
    epsilon_rep = "sed -i 's/EEEEEEE/" + stream.str() + "/g' temp_600/Vap/par_water.inp";
    system(epsilon_rep.c_str());

    // Replace n
    stream.str(std::string());
    stream << current_n;
    std::string n_rep = "sed -i 's/NNN/" + stream.str() + "/g' temp_500/Liq/par_water.inp";
    system(n_rep.c_str());
    n_rep = "sed -i 's/NNN/" + stream.str() + "/g' temp_500/Vap/par_water.inp";
    system(n_rep.c_str());
    n_rep = "sed -i 's/NNN/" + stream.str() + "/g' temp_600/Liq/par_water.inp";
    system(n_rep.c_str());
    n_rep = "sed -i 's/NNN/" + stream.str() + "/g' temp_600/Vap/par_water.inp";
    system(n_rep.c_str());

    // Replace H Charges
    stream.str(std::string());
    stream << std::fixed << std::setprecision(6) << current_charge;
    std::string hcharge_rep = "sed -i 's/HHHHHHHH/" + stream.str() + "/g' temp_500/Liq/START_WATER_BOX_0.psf";
    system(hcharge_rep.c_str());
    hcharge_rep = "sed -i 's/HHHHHHHH/" + stream.str() + "/g' temp_500/Vap/START_WATER_BOX_0.psf";
    system(hcharge_rep.c_str());
    hcharge_rep = "sed -i 's/HHHHHHHH/" + stream.str() + "/g' temp_600/Liq/START_WATER_BOX_0.psf";
    system(hcharge_rep.c_str());
    hcharge_rep = "sed -i 's/HHHHHHHH/" + stream.str() + "/g' temp_600/Vap/START_WATER_BOX_0.psf";
    system(hcharge_rep.c_str());

    // Replace O Charges
    stream.str(std::string());
    stream << std::fixed << std::setprecision(6) << 2 * current_charge;
    std::string ocharge_rep = "sed -i 's/OOOOOOOO/" + stream.str() + "/g' temp_500/Liq/START_WATER_BOX_0.psf";
    system(ocharge_rep.c_str());
    ocharge_rep = "sed -i 's/OOOOOOOO/" + stream.str() + "/g' temp_500/Vap/START_WATER_BOX_0.psf";
    system(ocharge_rep.c_str());
    ocharge_rep = "sed -i 's/OOOOOOOO/" + stream.str() + "/g' temp_600/Liq/START_WATER_BOX_0.psf";
    system(ocharge_rep.c_str());
    ocharge_rep = "sed -i 's/OOOOOOOO/" + stream.str() + "/g' temp_600/Vap/START_WATER_BOX_0.psf";
    system(ocharge_rep.c_str());
}

std::string get_last_line_of(std::string filename, std::string substring)
{
    std::string line;
    std::string last_line;
    LOG(INFO) << "Opening file " << filename << "...";
    std::ifstream myfile(filename.c_str());
    if(!myfile)
    {
        LOG(FATAL) << "Couldn't open file " << filename << "!";
        LOG(FATAL) << "Exiting...";
    }
    else
    {
        LOG(INFO) << "Successfully opened file " << filename << "!";
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
    LOG(INFO) << "Finding density in " << filename << "!";
    std::string last_line = get_last_line_of(filename, "STAT_0");
    double density;
    std::size_t found = last_line.find_last_of(" \t");
    density = std::stod(last_line.substr(found+1));
    LOG(INFO) << "Density found: " << density;
    return density;
}

double check_density_500_Liq() 
{
    std::string loc("temp_500/Liq/out.log");
    double den = return_density(loc);
    LOG(INFO) << "Expected density: " << DEN_LIQ_500;
    LOG(INFO) << "Density reported: " << den;
    double err = 0.0;
    if(den!=0.0)
        err = ((abs(DEN_LIQ_500-den))/den)*100;
    else
        err = 99999999999.9;
    LOG(INFO) << "Error: " << err;
    return err;
}

double check_density_500_Vap()
{
    std::string loc("temp_500/Vap/out.log");
    double den = return_density(loc);
    LOG(INFO) << "Expected density: " << DEN_VAP_500;
    LOG(INFO) << "Density reported: " << den;
    double err = 0.0;
    if(den!=0.0)
        err = ((abs(DEN_VAP_500-den))/den)*100;
    else
        err = 99999999999.9;
    LOG(INFO) << "Error: " << err;
    return err;
}

double check_density_600_Liq()
{
    std::string loc("temp_600/Liq/out.log");
    double den = return_density(loc);
    LOG(INFO) << "Expected density: " << DEN_LIQ_600;
    LOG(INFO) << "Density reported: " << den;
    double err = 0.0;
    if(den!=0.0)
        err = ((abs(DEN_LIQ_600-den))/den)*100;
    else
        err = 99999999999.9;
    LOG(INFO) << "Error: " << err;
    return err;
}

double check_density_600_Vap()
{
    std::string loc("temp_600/Vap/out.log");
    double den = return_density(loc);
    LOG(INFO) << "Expected density: " << DEN_VAP_600;
    LOG(INFO) << "Density reported: " << den;
    double err = 0.0;
    if(den!=0.0)
        err = ((abs(DEN_VAP_600-den))/den)*100;
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
    LOG(INFO) << "Running 500K Liq...";
    system("cd temp_500/Liq; ./GOMC_GPU_NPT in.conf > out.log");
    LOG(INFO) << "Finished 500K Liq!";
    curr_obj = check_density_500_Liq();
    if(curr_obj > 1.0)
        return 999999999999999.9;
    else
        total_obj += curr_obj;

    // Run T 500 Vap
    LOG(INFO) << "Running 500K Vap...";
    system("cd temp_500/Vap; ./GOMC_GPU_NPT in.conf > out.log");
    LOG(INFO) << "Finished 500K Vap!";
    curr_obj = check_density_500_Vap();
    if(curr_obj > 1.0)
        return 999999999999999.9;
    else
        total_obj += curr_obj;

    // Run T 600 Liq
    LOG(INFO) << "Running 600K Liq...";
    system("cd temp_600/Liq; ./GOMC_GPU_NPT in.conf > out.log");
    LOG(INFO) << "Finished 600K Liq!";
    curr_obj = check_density_600_Liq();
    if(curr_obj > 1.0)
        return 999999999999999.9;
    else
        total_obj += curr_obj;

    // Run T 600 Vap
    LOG(INFO) << "Running 600K Vap...";
    system("cd temp_600/Vap; ./GOMC_GPU_NPT in.conf > out.log");
    LOG(INFO) << "Finished 600K Vap!";
    curr_obj = check_density_600_Vap();
    if(curr_obj > 1.0)
        return 999999999999999.9;
    else
        total_obj += curr_obj;
    
    return total_obj;
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
        LOG(INFO) << "Current objective before run: " << current_obj;
        LOG(INFO) << "Current temperature: " << current_temperature;
        
        pick_next();
        PrintCurrent();
        current_obj = objective_function();
        delta = current_obj - objective;
        LOG(INFO) << "Current objective after run: " << current_obj;
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
        current_temperature *= COOLING_FRACTION;
        LOG(INFO) << "=========== END ITERATION ===========";
    }
}
