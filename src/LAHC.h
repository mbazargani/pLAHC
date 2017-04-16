/*
 * File:    LAHC.h
 * Author:  Mosab Bazargani
 * License: BSD 3-Clause License
 *
 * Description: The LAHC class implements the LAHC introduced by Bruke and 
 * Bykov (2017).
 * 
 * Created on March 31, 2016, 3:34 PM
 */

#ifndef LAHC_H
#define LAHC_H

#include "TSPLIB.h"
#include "TSP_mutation.h"
#include "Util.h"
#include "Configuration.h"
#include "Random.h"
#include <iostream>
#include <vector>
#include <stdlib.h> 
#include <fstream>
#include <algorithm>

using namespace std;

class LAHC {
public:
    std::vector <lahc_results> results;
    std::vector<int> success_iteration_tour_length;
    LAHC(TSPLIB &tsp, stopping_criteria stop_criteria, int lsize);
    LAHC(TSPLIB &tsp, stopping_criteria stop_criteria, int lsize,
            int *given_tour, std::vector<int> &history); 
    ~LAHC();
    void run();
    int *best_ever_tour();
    int *current_tour();
    string report_best_result();
    void report_progress(string prog_file, string run_num);
    void report_best_results(string best_results_file, string run_num);
    void sum_results_comp(vector<lahc_results> &sum_res);
    void copy_results(vector<lahc_results> **collective_results, int run);
    double list_average();
    bool list_converge();
    long num_iterations();
    int best_tour_size();
    void permutate();
    int list_val(int index);
    int current_tour_length();
private:
    TSPLIB *tsp_instance;
    stopping_criteria stp_criteria;
    tsp_mut_properties mut_conf;
    int *tour;
    int *best_tour;
    int tour_length;
    int num_cities;
    int list_size;
    int *list;
    int list_indicator;
    long iteration;
    long idle_iteration;
    int best_tour_length;
    lahc_results improvement;

    inline void mutation_cost();
    inline bool stop();
    inline void push_improvement();
    inline void update_best_ever_tour();
    void initial_list(std::vector<int> &history);
};

#endif /* LAHC_H */

