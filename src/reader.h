#pragma once

#include "tinyxml2.h"
#include <iostream>
#include <sstream>

class Reader {
  public:
    static void Init(const char* filename)
    {
      tinyxml2::XMLDocument doc;
      doc.LoadFile(filename);

      // read configs
      command = std::string (doc.FirstChildElement("configuration")->FirstChildElement("command")->GetText());
      densityLiq1 = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("densityliq1")->GetText());
      densityLiq2 = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("densityliq2")->GetText());
      densityVap1 = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("densityvap1")->GetText());
      densityVap2 = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("densityvap2")->GetText());
      dir1 = std::string (doc.FirstChildElement("configuration")->FirstChildElement("directory1")->GetText());
      dir2 = std::string (doc.FirstChildElement("configuration")->FirstChildElement("directory2")->GetText());
      psf_file = std::string (doc.FirstChildElement("configuration")->FirstChildElement("psffile")->GetText());
      par_file = std::string (doc.FirstChildElement("configuration")->FirstChildElement("parfile")->GetText());

      std::string sigma_en = std::string (doc.FirstChildElement("configuration")->FirstChildElement("sigma")->FirstChildElement("enabled")->GetText());
      if(sigma_en == "true")
        sigma_enabled = true;
      std::string epsilon_en = std::string (doc.FirstChildElement("configuration")->FirstChildElement("epsilon")->FirstChildElement("enabled")->GetText());
      if(epsilon_en == "true")
        epsilon_enabled = true;
      std::string n_en = std::string (doc.FirstChildElement("configuration")->FirstChildElement("n")->FirstChildElement("enabled")->GetText());
      if(n_en == "true")
        n_enabled = true;
      std::string charge_en = std::string (doc.FirstChildElement("configuration")->FirstChildElement("charge")->FirstChildElement("enabled")->GetText());
      if(charge_en == "true")
        charge_enabled = true;
      std::string bond_en = std::string (doc.FirstChildElement("configuration")->FirstChildElement("bond")->FirstChildElement("enabled")->GetText());
      if(bond_en == "true")
        bond_enabled = true;

      epsilon_value = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("epsilon")->FirstChildElement("value")->GetText());
      sigma_value = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("sigma")->FirstChildElement("value")->GetText());
      n_value = toInt(doc.FirstChildElement("configuration")->FirstChildElement("n")->FirstChildElement("value")->GetText());
      charge_value = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("charge")->FirstChildElement("value")->GetText());
      bond_value = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("bond")->FirstChildElement("value")->GetText());

      epsilon_start = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("epsilon")->FirstChildElement("start")->GetText());
      sigma_start = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("sigma")->FirstChildElement("start")->GetText());
      n_start = toInt(doc.FirstChildElement("configuration")->FirstChildElement("n")->FirstChildElement("start")->GetText());
      charge_start = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("charge")->FirstChildElement("start")->GetText());
      bond_start = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("bond")->FirstChildElement("start")->GetText());

      epsilon_end = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("epsilon")->FirstChildElement("end")->GetText());
      sigma_end = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("sigma")->FirstChildElement("end")->GetText());
      n_end = toInt(doc.FirstChildElement("configuration")->FirstChildElement("n")->FirstChildElement("end")->GetText());
      charge_end = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("charge")->FirstChildElement("end")->GetText());
      bond_end = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("bond")->FirstChildElement("end")->GetText());

      epsilon_accuracy = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("epsilon")->FirstChildElement("accuracy")->GetText());
      sigma_accuracy = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("sigma")->FirstChildElement("accuracy")->GetText());
      charge_accuracy = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("charge")->FirstChildElement("accuracy")->GetText());
      bond_accuracy = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("bond")->FirstChildElement("accuracy")->GetText());

      initial_temp = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("temp")->FirstChildElement("initial")->GetText());
      frac_temp = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("temp")->FirstChildElement("fraction")->GetText());
      crit_temp = toDouble(doc.FirstChildElement("configuration")->FirstChildElement("temp")->FirstChildElement("critpoint")->GetText());
    }

    static void GenNext() {
      
    }

    // What command use to run the simulation
    static std::string command;

    // Density of each sim
    static double densityLiq1;
    static double densityLiq2;
    static double densityVap1;
    static double densityVap2;

    // Directory names
    static std::string dir1;
    static std::string dir2;

    // Files
    static std::string psf_file;
    static std::string par_file;

    // See which one is enabled and which one is disabled
    // if disabled use *_value
    // if enabled use random number
    static bool epsilon_enabled;
    static bool sigma_enabled;
    static bool n_enabled;
    static bool charge_enabled;
    static bool bond_enabled;

    // Use these values if it was disabled
    static double epsilon_value;
    static double sigma_value;
    static int n_value;
    static double charge_value;
    static double bond_value;

    // Current value of each
    static double epsilon_current_value;
    static double sigma_current_value;
    static int n_current_value;
    static double charge_current_value;
    static double bond_current_value;

    // Start of each 
    static double epsilon_start;
    static double sigma_start;
    static int n_start;
    static double charge_start;
    static double bond_start;

    // End of each
    static double epsilon_end;
    static double sigma_end;
    static int n_end;
    static double charge_end;
    static double bond_end;

    // Accuracy of each
    static double epsilon_accuracy;
    static double sigma_accuracy;
    static double charge_accuracy;
    static double bond_accuracy;

    static double initial_temp;
    static double frac_temp;
    static double crit_temp;

  private:
    static double toDouble(const char * s)
    {
      std::istringstream ss(s);
      double n;
      ss >> n;
      return n;
    }

    static int toInt(const char * s)
    {
      std::istringstream ss(s);
      int n;
      ss >> n;
      return n;
    }
};

std::string Reader::command = "";

// Density of each sim
double Reader::densityLiq1 = 0.0;
double Reader::densityLiq2 = 0.0;
double Reader::densityVap1 = 0.0;
double Reader::densityVap2 = 0.0;

// Directory names
std::string Reader::dir1 = "";
std::string Reader::dir2 = "";

std::string Reader::psf_file = "";
std::string Reader::par_file = "";

bool Reader::epsilon_enabled = false;
bool Reader::sigma_enabled = false;
bool Reader::n_enabled = false;
bool Reader::charge_enabled = false;
bool Reader::bond_enabled = false;

// Use these values if it was disabled
double Reader::epsilon_value = 0.0;
double Reader::sigma_value = 0.0;
int Reader::n_value = 0.0;
double Reader::charge_value = 0.0;
double Reader::bond_value = 0.0;

// Current value of each
double Reader::epsilon_current_value = 0.0;
double Reader::sigma_current_value = 0.0;
int Reader::n_current_value = 0.0;
double Reader::charge_current_value = 0.0;
double Reader::bond_current_value = 0.0;

// Start of each 
double Reader::epsilon_start = 0.0;
double Reader::sigma_start = 0.0;
int Reader::n_start = 0.0;
double Reader::charge_start = 0.0;
double Reader::bond_start = 0.0;

// End of each
double Reader::epsilon_end = 0.0;
double Reader::sigma_end = 0.0;
int Reader::n_end = 0.0;
double Reader::charge_end = 0.0;
double Reader::bond_end = 0.0;

double Reader::epsilon_accuracy = 0.0;
double Reader::sigma_accuracy = 0.0;
double Reader::charge_accuracy = 0.0;
double Reader::bond_accuracy = 0.0;

double Reader::initial_temp = 0.0;
double Reader::frac_temp = 0.0;
double Reader::crit_temp = 0.0;
