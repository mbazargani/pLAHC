/* 
 * File:    pLAHC.h
 * Author:  Mosab Bazargani
 * License: BSD 3-Clause License
 * 
 * Description: This is an implementation of Parameter-less Late Acceptance 
 * Hill-Climbing (pLAHC) algorithm (the work is published in GECCO'17).
 *
 * Created on April 28, 2016, 11:13 PM
 */

#ifndef PLAHC_H
#define PLAHC_H

#include "LAHC.h"
#include "TSPLIB.h"
#include "Util.h"
#include "Configuration.h"
#include "Random.h"
#include <iostream>
#include <vector>
#include <stdlib.h> 
#include <fstream>
#include <math.h>


using namespace std;

class pLAHC {
public:
    std::vector <LAHC*> race_track;
    std::vector <explahc_special_events> special_events;
    pLAHC(TSPLIB &tsp, stopping_criteria stop_criterion,
            int base_lsize, int list_ssize);
    ~pLAHC();
    void run();
    string report_best_result();
    void report_progress(string prog_file, string run_num);
    void report_special_events(string events_file, string run_num);
    void report_best_results(string best_results_file, string run_num);
    string report_avg_runs();
    long num_iterations();
    void copy_special_events(vector<explahc_special_events>
            **collective_special_events, int run);
    void copy_results(vector<explahc_results>
            **collective_results, int run);
private:
    TSPLIB *tsp_instance;
    stopping_criteria stp_criteria;
    int base_list_size;
    int list_scaling_size;
    int current_index;
    long total_iteration;
    long total_effective_iteration;
    explahc_special_events initial_special_event;
    int best_ever_tour_length;
    int best_ever_tour_index;

    bool stop();
    void initial_new_list(int lsize);
    void update_special_even();

    inline long number_total_imrovements();
    inline std::vector<int> activated_lsizes();
    inline std::vector<int> best_tours_lsizes();
    inline std::vector<long> lsizes_number_iteration();
    inline std::vector<double> lsizes_avgs();
    inline std::vector<long> lsizes_number_improvments();
};

#endif /* PLAHC_H */

