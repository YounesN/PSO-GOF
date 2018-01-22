#pragma once

#include <iostream>
#include "restriction.h"
#include "easylogging++.h"

void PrintRestrictions()
{
    LOG(INFO) << "========= RESTRICTIONS =========";
    LOG(INFO) << "EPSILON: [" << EPSILON_START << ", " << EPSILON_END
        << ", " << EPSILON_ACC << "]";
    LOG(INFO) << "SIGMA  : [" << SIGMA_START << ", " << SIGMA_END
        << ", " << SIGMA_ACC << "]";
    LOG(INFO) << "N      : [" << N_START << ", " << N_END
        << ", " << N_ACC << "]";
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
    LOG(INFO) << "EPSILON: " << current_epsilon;
    LOG(INFO) << "SIGMA  : " << current_sigma;
    LOG(INFO) << "N      : " << current_n;
}

void PrintInitial()
{
    LOG(INFO) << "=========    Initial   =========";
    LOG(INFO) << "EPSILON: " << current_epsilon;
    LOG(INFO) << "SIGMA  : " << current_sigma;
    LOG(INFO) << "N      : " << current_n;
}
