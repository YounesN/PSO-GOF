#pragma once

#include <iostream>
#include "restriction.h"
#include "easylogging++.h"
#include "reader.h"

void PrintRestrictions()
{
    LOG(INFO) << "========= RESTRICTIONS =========";
    LOG(INFO) << "CHARGE : [" << Reader::charge_start << ", " << Reader::charge_end
        << ", " << Reader::charge_accuracy << "]";
    LOG(INFO) << "EPSILON: [" << Reader::epsilon_start << ", " << Reader::epsilon_end
        << ", " << Reader::epsilon_accuracy << "]";
    LOG(INFO) << "SIGMA  : [" << Reader::sigma_start << ", " << Reader::sigma_end
        << ", " << Reader::sigma_accuracy << "]";
    LOG(INFO) << "N      : [" << Reader::n_start << ", " << Reader::n_end
        << ", 1]";
    LOG(INFO) << "BOND   : [" << Reader::bond_start << ", " << Reader::bond_end
        << ", " << Reader::bond_accuracy << "]";
}

void PrintAlgorithmInfo()
{
    LOG(INFO) << "=========      SA      =========";
    LOG(INFO) << "=========    Initial   =========";
    LOG(INFO) << "Initial Temperature: " << Reader::initial_temp;
    LOG(INFO) << "Cooling Fraction   : " << Reader::frac_temp;
    LOG(INFO) << "Cooling Criteria   : " << Reader::crit_temp;
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
