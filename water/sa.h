#pragma once

#include <random>
#include <iomanip>
#include <sstream>

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
    system("cp -R T_500 temp_500");
    system("cp -R T_600 temp_600");

    // Replace Bond
    std::stringstream stream;
    stream << std::fixed << std::setprecision(4) << current_bond;
    std::string bond_rep = "sed -i 's/BBBBBB/" + stream.str() + "/g' temp_500/par_water.inp";
    system(bond_rep.c_str());
    bond_rep = "sed -i 's/BBBBBB/" + stream.str() + "/g' temp_600/par_water.inp";
    system(bond_rep.c_str());

    // Replace Sigma
    stream.str(std::string());
    stream << std::fixed << std::setprecision(6) << current_sigma;
    std::string sigma_rep = "sed -i 's/SSSSSSSSS/" + stream.str() + "/g' temp_500/par_water.inp";
    system(sigma_rep.c_str());
    sigma_rep = "sed -i 's/SSSSSSSSS/" + stream.str() + "/g' temp_600/par_water.inp";
    system(sigma_rep.c_str());

    // Replace Epsilon
    stream.str(std::string());
    stream << std::fixed << std::setprecision(6) << current_epsilon;
    std::string epsilon_rep = "sed -i 's/EEEEEEE/" + stream.str() + "/g' temp_500/par_water.inp";
    system(epsilon_rep.c_str());
    epsilon_rep = "sed -i 's/EEEEEEE/" + stream.str() + "/g' temp_600/par_water.inp";
    system(epsilon_rep.c_str());

    // Replace n
    stream.str(std::string());
    stream << current_n;
    std::string n_rep = "sed -i 's/NNN/" + stream.str() + "/g' temp_500/par_water.inp";
    system(n_rep.c_str());
    n_rep = "sed -i 's/NNN/" + stream.str() + "/g' temp_600/par_water.inp";
    system(n_rep.c_str());

    // Replace H Charges
    stream.str(std::string());
    stream << std::fixed << std::setprecision(6) << current_charge;
    std::string hcharge_rep = "sed -i 's/HHHHHHHH/" + stream.str() + "/g' temp_500/START_WATER_BOX_0.psf";
    system(hcharge_rep.c_str());
    hcharge_rep = "sed -i 's/HHHHHHHH/" + stream.str() + "/g' temp_500/START_WATER_BOX_1.psf";
    system(hcharge_rep.c_str());
    hcharge_rep = "sed -i 's/HHHHHHHH/" + stream.str() + "/g' temp_600/START_WATER_BOX_0.psf";
    system(hcharge_rep.c_str());
    hcharge_rep = "sed -i 's/HHHHHHHH/" + stream.str() + "/g' temp_600/START_WATER_BOX_1.psf";
    system(hcharge_rep.c_str());

    // Replace O Charges
    stream.str(std::string());
    stream << std::fixed << std::setprecision(6) << 2 * current_charge;
    std::string ocharge_rep = "sed -i 's/OOOOOOOO/" + stream.str() + "/g' temp_500/START_WATER_BOX_0.psf";
    system(ocharge_rep.c_str());
    ocharge_rep = "sed -i 's/OOOOOOOO/" + stream.str() + "/g' temp_500/START_WATER_BOX_1.psf";
    system(ocharge_rep.c_str());
    ocharge_rep = "sed -i 's/OOOOOOOO/" + stream.str() + "/g' temp_600/START_WATER_BOX_0.psf";
    system(ocharge_rep.c_str());
    ocharge_rep = "sed -i 's/OOOOOOOO/" + stream.str() + "/g' temp_600/START_WATER_BOX_1.psf";
    system(ocharge_rep.c_str());
}

void objective_function()
{
    make_temp();

    // Run T 500
    system("cd temp_500; ./GOMC_CPU_GEMC +p8 in.conf");

    // Run T 600
    system("cd temp_600; ./GOMC_CPU_GEMC +p8 in.conf");
}

void annealing()
{
    while(current_temperature > COOLING_CRIT)
    {
        pick_next();
        objective_function();

        iteration ++;
        current_temperature *= COOLING_FRACTION;
    }
}
