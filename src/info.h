#pragma once

#include <iostream>
#include "restriction.h"
#include "easylogging++.h"
#include "reader.h"

void PrintRestrictions()
{
  LOG(INFO) << "========= RESTRICTIONS =========";
  if(Reader::charge_enabled)
    LOG(INFO) << "CHARGE : [" << Reader::charge_start << ", " << Reader::charge_end
      << ", " << Reader::charge_accuracy << "]";
  if(Reader::epsilon_enabled)
    LOG(INFO) << "EPSILON: [" << Reader::epsilon_start << ", " << Reader::epsilon_end
      << ", " << Reader::epsilon_accuracy << "]";
  if(Reader::sigma_enabled)
    LOG(INFO) << "SIGMA  : [" << Reader::sigma_start << ", " << Reader::sigma_end
      << ", " << Reader::sigma_accuracy << "]";
  if(Reader::n_enabled)
    LOG(INFO) << "N      : [" << Reader::n_start << ", " << Reader::n_end
      << ", 1]";
  if(Reader::bond_enabled)
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
  if(Reader::charge_enabled)
    (INFO) << "CHARGE : " << current_charge;
  if(Reader::epsilon_enabled)
    (INFO) << "EPSILON: " << current_epsilon;
  if(Reader::sigma_enabled)
    (INFO) << "SIGMA  : " << current_sigma;
  if(Reader::n_enabled)
    (INFO) << "N      : " << current_n;
  if(Reader::bond_enabled)
    (INFO) << "BOND   : " << current_bond;
}

void PrintInitial()
{
    LOG(INFO) << "=========    Initial   =========";
    if(Reader::charge_enabled)
      LOG(INFO) << "CHARGE : " << current_charge;
    if(Reader::epsilon_enabled)
      LOG(INFO) << "EPSILON: " << current_epsilon;
    if(Reader::sigma_enabled)
      LOG(INFO) << "SIGMA  : " << current_sigma;
    if(Reader::n_enabled)
      LOG(INFO) << "N      : " << current_n;
    if(Reader::bond_enabled)
      LOG(INFO) << "BOND   : " << current_bond;
}
