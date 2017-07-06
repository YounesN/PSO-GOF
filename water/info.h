#pragma once

#include <iostream>
#include "restriction.h"

void PrintRestrictions()
{
  std::cout << "========= RESTRICTIONS =========" << std::endl;
  std::cout << "CHARGE : [" << HCHARGE_START << ", " << HCHARGE_END
	    << ", " << HCHARGE_ACC << "]" << std::endl;
  std::cout << "EPSILON: [" << EPSILON_START << ", " << EPSILON_END
	    << ", " << EPSILON_ACC << "]" << std::endl;
  std::cout << "SIGMA  : [" << SIGMA_START << ", " << SIGMA_END
	    << ", " << SIGMA_ACC << "]" << std::endl;
  std::cout << "N      : [" << N_START << ", " << N_END
	    << ", " << N_ACC << "]" << std::endl;
  std::cout << "BOND   : [" << BOND_START << ", " << BOND_END
	    << ", " << BOND_ACC << "]" << std::endl;
}

void PrintAlgorithmInfo()
{
  std::cout << "=========      SA      =========" << std::endl;
  std::cout << "Initial Temperature: " << INITIAL_TEMPERATURE << std::endl;
  std::cout << "Cooling Fraction   : " << COOLING_FRACTION << std::endl;
  std::cout << "Cooling Criteria   : " << COOLING_CRIT << std::endl;
}
