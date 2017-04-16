/*
 * File:    Configuration.h
 * Author:  Mosab Bazargani
 * License: BSD 3-Clause License
 *
 * Description: The Configuration class reads the given input file.
 * 
 * Created on March 31, 2016, 3:34 PM
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#undef MAX_INT
#undef EPSILON
#define MAX_INT std::numeric_limits<int>::max()
#define EPSILON std::numeric_limits<float>::min()

#include "Util.h"
#include <stdlib.h> 
#include <stdio.h>
#include <fstream>
#include <iostream>

using namespace std;

typedef struct {
    bool max_iteration;
    long max_iteration_par;
    bool solution_quality;
    double solution_quality_par;
    bool point_of_convergence;
    double point_of_convergence_par;
} stopping_criteria;

class Configuration {
public:
    string inputfile;
    // TSP PARAMETERS
    string problem_name;
    // GENERAL PARAMETERS
    string algorithm;
    int seed;
    int num_run;
    // LAHC PARAMETERS
    int list_size;
    // INCRIMENTAL LAHC PARAMETERS
    int base_list_size;
    int list_scaling_size;  
    // STOPPING CRITERION PARAMETERS
    stopping_criteria stopping;
    // STORE DATA PARAMETERS
    string restore_progress;
    string restore_special_events;
    string restore_best_results;
    string output;
    Configuration(string input);
    string dump_parameters();
    
private:
    // Tools used for reading inputfile
    void set_parameter(string &key, string &value);
    inline void set_stopping_criterion_parameter(string &value);
    void read_inputfile(ifstream &inputfile);
};


#endif /* CONFIGURATION_H */

