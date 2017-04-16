/*
 * File:    LAHC.cpp
 * Author:  Mosab Bazargani
 * License: BSD 3-Clause License
 *
 * Description: The LAHC class implements the LAHC introduced by Bruke and 
 * Bykov (2017).
 * 
 */

#include "LAHC.h"

using namespace std;
extern Random RANDOM; // random number generator

LAHC::LAHC(TSPLIB &tsp, stopping_criteria stop_criteria, int lsize) {
    stp_criteria = stop_criteria;
    list_size = lsize;
    num_cities = tsp.properties.DIMENSION;
    tsp_instance = &tsp;
    tour = new int[num_cities];
    best_tour = new int[num_cities];
    list = new int[list_size];
    mut_conf.cuts = new int[2];
    mut_conf.num_cities = num_cities;
    list_indicator = 0;
    iteration = 0;
    idle_iteration = 0;
    makeshuffle(num_cities, tour, num_cities); // permute a tour
    tour_length = best_tour_length = mut_conf.length = tsp.tour_length(tour);
    for (int i = 0; i < list_size; i++)
        list[i] = tour_length;
    success_iteration_tour_length.push_back(best_tour_length);
}

LAHC::LAHC(TSPLIB &tsp, stopping_criteria stop_criteria, int lsize,
        int *given_tour, std::vector<int> &history) {
    stp_criteria = stop_criteria;
    list_size = lsize;
    num_cities = tsp.properties.DIMENSION;
    tsp_instance = &tsp;
    tour = new int[num_cities];
    best_tour = new int[num_cities];
    list = new int[list_size];
    mut_conf.cuts = new int[2];
    mut_conf.num_cities = num_cities;
    list_indicator = 0;
    iteration = 0;
    idle_iteration = 0;
    for (int i = 0; i < num_cities; i++)
        tour[i] = given_tour[i];
    tour_length = best_tour_length = mut_conf.length = tsp.tour_length(tour);
    initial_list(history);
    success_iteration_tour_length.push_back(best_tour_length);
}

LAHC::~LAHC() {
    delete [] tour;
    delete [] best_tour;
    delete [] list;
    delete [] mut_conf.cuts;

    success_iteration_tour_length.resize(0);
    success_iteration_tour_length.clear();
    success_iteration_tour_length.shrink_to_fit();
    results.resize(0);
    results.clear();
    results.shrink_to_fit();
}

void LAHC::run() {
    push_improvement();
    while (stop()) {
        mutation_cost();
        if (mut_conf.length >= tour_length)
            idle_iteration++;
        else
            idle_iteration = 0;
        list_indicator = iteration % list_size;
        if (mut_conf.length < list[list_indicator] ||
                mut_conf.length <= tour_length) {
            update_tour(tour, &mut_conf);
            tour_length = mut_conf.length;
        }
        if (tour_length < list[list_indicator])
            list[list_indicator] = tour_length;
        if (tour_length < best_tour_length) {
            best_tour_length = mut_conf.length;
            success_iteration_tour_length.push_back(best_tour_length);
            update_best_ever_tour();
            push_improvement();
        }
        iteration++;
    }
    if (results[results.size() - 1].iteration != iteration - 1)
        push_improvement(); // to restore the last stage
}

void LAHC::initial_list(std::vector<int>& history) {
    for (int i = 0; i < list_size; i++)
        list[i] = history[i % history.size()];
    // Now we sort the list
    sort(list, list + list_size, std::greater<int>());
}

void LAHC::permutate() {
    // this function only do one iteration of the LAHC
    mutation_cost();
    if (mut_conf.length <= list[list_indicator] ||
            mut_conf.length <= tour_length) {
        if (mut_conf.length < tour_length)
            idle_iteration = 0;
        update_tour(tour, &mut_conf);
        tour_length = mut_conf.length;
    }
    list[list_indicator] = tour_length;
    if (mut_conf.length < best_tour_length) {
        best_tour_length = mut_conf.length;
        push_improvement();
    }
    list_indicator = iteration % list_size;
    iteration++;
    idle_iteration++;
}

inline void LAHC::mutation_cost() {
    // This modules randomly selected one of the following mutation at each call.
    //    can be one of the following:
    //    double-bridge, or simple_inversion -- used in 2-opt
    //    swapping
    //    replacement
    mut_conf.length = tour_length;
    //    int selected_mutation = RANDOM.uniform((long) 0, (long) 2);
    // here we only use 'double_bridge' as it is implemented in
    // 2017 version of Burke and Bykov
    int selected_mutation = 0; 
    switch (selected_mutation) {
        case 0: // double-bridge mutation
            double_bridge(tour, &mut_conf, tsp_instance->distance_matrix);
            break;
        case 1: // swap mutation
            swap(tour, &mut_conf, tsp_instance->distance_matrix);
            break;
        case 2: // displacement mutation
            displacement(tour, &mut_conf, tsp_instance->distance_matrix);
            break;
    }
}

int *LAHC::best_ever_tour() {
    return best_tour;
}

int *LAHC::current_tour() {
    return tour;
}

double LAHC::list_average() {
    double avg_list = 0.0;
    for (int i = 0; i < list_size; i++)
        avg_list += list[i];
    return avg_list / (double) list_size;
}

bool LAHC::list_converge() {
    for (int i = 1; i < list_size; i++)
        if (list[0] != list[i])
            return false;
    return true;
}

inline bool LAHC::stop() {
    if (stp_criteria.max_iteration){
        return stp_criteria.max_iteration_par > iteration; 
    }
    else if (stp_criteria.point_of_convergence && stp_criteria.solution_quality)
        return ((iteration < 100000 || idle_iteration *
            stp_criteria.point_of_convergence_par < iteration) &&
            (best_tour_length > stp_criteria.solution_quality_par));
    else if (stp_criteria.point_of_convergence &&
            not stp_criteria.solution_quality){
        return (iteration < 100000 || idle_iteration *
            stp_criteria.point_of_convergence_par < iteration);        
    }
    else
        error("The `stopping_criterion` is not implemented --> LAHC::stop()");

    return false;
}

inline void LAHC::push_improvement() {

    improvement.best_tour_length = best_tour_length;
    improvement.iteration = iteration;
    improvement.list_avg = list_average();
    results.push_back(improvement);
}

inline void LAHC::update_best_ever_tour() {
    for (int i = 0; i < num_cities; i++)
        best_tour[i] = tour[i];
}

string LAHC::report_best_result() {
    string msg = "";
    msg += ">> num iteration:      ";
    msg += std::to_string(results[results.size() - 1].iteration) + "\n";
    msg += std::string(21, ' ') + ">> best tour length:   ";
    msg += std::to_string(results[results.size() - 1].best_tour_length) + "\n";
    msg += std::string(21, ' ') + ">> list avg.:          ";
    msg += std::to_string(results[results.size() - 1].list_avg) + "\n";
    msg += std::string(21, ' ') + "--------------------------------------";

    return msg;
}

void LAHC::report_progress(string prog_file, string run_num) {
    // it will store the progress in a file with name <output>.progress
    ofstream progress(prog_file.c_str(), ios_base::app);
    progress << "## Run number " << run_num << endl;

    for (long i = 0; i < results.size(); i++)
        progress << "(" << std::to_string(results[i].iteration) <<
        ", " << std::to_string(results[i].best_tour_length) << ", " <<
        std::to_string(results[i].list_avg) << ") ";
    progress << endl;
    progress.close();
}

void LAHC::report_best_results(string best_results_file, string run_num) {
    // it will store best results obtained in each run in a file 
    // with name <output>.bests

    ofstream bests(best_results_file.c_str(), ios_base::app);
    bests << run_num << " " << std::to_string(iteration) << " " <<
            std::to_string(best_tour_length) << " " <<
            std::to_string(list_average()) << endl;
    bests.close();
}

void LAHC::sum_results_comp(vector<lahc_results> &sum_res) {
    if (sum_res.size() == 0) {
        for (long i = 1; i < results.size(); i++)
            for (long j = results[i - 1].iteration; j < results[i].iteration; j++) {
                if (j % 200000 == 0)
                    cout << j << " ";
                sum_res.push_back(results[i - 1]);
                sum_res.back().iteration = j;
            }
        sum_res.push_back(results.back());
        return;
    }

    if (sum_res.back().iteration == results.back().iteration) {
        for (long i = 1; i < results.size(); i++)
            for (long j = results[i - 1].iteration; j < results[i].iteration; j++) {
                sum_res[j].best_tour_length += results[i - 1].best_tour_length;
                sum_res[j].list_avg += results[i - 1].list_avg;
            }
        sum_res.back().best_tour_length += results.back().best_tour_length;
        sum_res.back().list_avg += results.back().list_avg;
        return;
    }

    if (sum_res.back().iteration < results.back().iteration) {
        long size_sum_res = sum_res.size() - 1;
        for (long i = sum_res.size(); i <= results.back().iteration; i++) {
            sum_res.push_back(sum_res[size_sum_res]);
            sum_res.back().iteration = i;
        }
        for (long i = 1; i < results.size(); i++)
            for (long j = results[i - 1].iteration; j < results[i].iteration; j++) {
                sum_res[j].best_tour_length += results[i - 1].best_tour_length;
                sum_res[j].list_avg += results[i - 1].list_avg;
            }
        sum_res.back().best_tour_length += results.back().best_tour_length;
        sum_res.back().list_avg += results.back().list_avg;
        return;
    }

    if (sum_res.back().iteration > results.back().iteration) {
        for (long i = results.back().iteration; i <= sum_res.back().iteration; i++) {
            sum_res[i].best_tour_length += results.back().best_tour_length;
            sum_res[i].list_avg += results.back().list_avg;
        }
        for (long i = 1; i < results.size(); i++)
            for (long j = results[i - 1].iteration; j < results[i].iteration; j++) {

                sum_res[j].best_tour_length += results[i - 1].best_tour_length;
                sum_res[j].list_avg += results[i - 1].list_avg;
            }
        return;
    }
}

void LAHC::copy_results(vector<lahc_results> **collective_results, int run) {
    for (int i = 0; i < results.size(); i++)
        collective_results[run]->push_back(results[i]);
}

long LAHC::num_iterations() {
    return iteration;
}

int LAHC::list_val(int index) {
    return list[index];
}

int LAHC::current_tour_length() {
    return tour_length;
}

int LAHC::best_tour_size() {
    return best_tour_length;
}
