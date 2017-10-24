#pragma once

#include <iostream>
#include "restriction.h"
#include "easylogging++.h"

void PrintRestrictions()
{
    LOG(INFO) << "========= RESTRICTIONS =========";
    LOG(INFO) << "CHARGE : [" << HCHARGE_START << ", " << HCHARGE_END
        << ", " << HCHARGE_ACC << "]";
    LOG(INFO) << "EPSILON: [" << EPSILON_START << ", " << EPSILON_END
        << ", " << EPSILON_ACC << "]";
    LOG(INFO) << "SIGMA  : [" << SIGMA_START << ", " << SIGMA_END
        << ", " << SIGMA_ACC << "]";
    LOG(INFO) << "N      : [" << N_START << ", " << N_END
        << ", " << N_ACC << "]";
    LOG(INFO) << "BOND   : [" << BOND_START << ", " << BOND_END
        << ", " << BOND_ACC << "]";
}

void PrintAlgorithmInfo()
{
    LOG(INFO) << "=========      SA      =========";
    LOG(INFO) << "=========    Initial   =========";
    LOG(INFO) << "Initial Temperature: " << INITIAL_TEMPERATURE;
    LOG(INFO) << "Cooling Fraction   : " << COOLING_FRACTION;
    LOG(INFO) << "Cooling Criteria   : " << COOLING_CRIT;
}

void PrintCurrent()
{
    LOG(INFO) << "=========  Current   =========";
    LOG(INFO) << "CHARGE : " << current_charge;
    LOG(INFO) << "EPSILON: " << current_epsilon;
    LOG(INFO) << "SIGMA  : " << current_sigma;
    LOG(INFO) << "N      : " << current_n;
    LOG(INFO) << "BOND   : " << current_bond;
}

void PrintInitial()
{
    LOG(INFO) << "=========    Initial   =========";
    LOG(INFO) << "CHARGE : " << current_charge;
    LOG(INFO) << "EPSILON: " << current_epsilon;
    LOG(INFO) << "SIGMA  : " << current_sigma;
    LOG(INFO) << "N      : " << current_n;
    LOG(INFO) << "BOND   : " << current_bond;
}
