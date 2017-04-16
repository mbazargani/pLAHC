/* 
 * File:    Statistic.cpp
 * Author:  Mosab Bazargani
 * License: BSD 3-Clause License
 *
 * Description: This is a collection of several methods to do the statistical 
 * analysis over several individual runs.
 * 
 */

#include "Statistic.h"

using namespace std;

void lahc_runs_avg(vector<lahc_results> **collective_results,
        vector<lahc_results> &sum_res, int freq, int num_runs) {
    lahc_results item;
    item.best_tour_length = 0;
    item.list_avg = 0;
    long max_num_iteration = 0;
    for (int i = 0; i < num_runs; i++)
        if (collective_results[i]->back().iteration > max_num_iteration)
            max_num_iteration = collective_results[i]->back().iteration;
    long step = long(max_num_iteration / freq);
    for (int i = 0; i <= freq; i++) {
        sum_res.push_back(item);
        sum_res.back().iteration = i * step;
    }
    if (max_num_iteration % freq != 0) {
        sum_res.push_back(item);
        sum_res.back().iteration = max_num_iteration;
    }
    for (int run = 0; run < num_runs; run++) {
        long i = 0;
        for (long iter = 0; iter < collective_results[run]->size(); iter++) {
            if ((*collective_results[run])[iter].iteration ==
                    sum_res[i].iteration) {
                sum_res[i].best_tour_length +=
                        (*collective_results[run])[iter].best_tour_length;
                sum_res[i].list_avg +=
                        (*collective_results[run])[iter].list_avg;
                i++;
            }
            if ((*collective_results[run])[iter].iteration >
                    sum_res[i].iteration) {
                sum_res[i].best_tour_length +=
                        (*collective_results[run])[iter - 1].best_tour_length;
                sum_res[i].list_avg +=
                        (*collective_results[run])[iter - 1].list_avg;
                i++;
            }
        }
        for (; i < sum_res.size(); i++) {
            sum_res[i].best_tour_length +=
                    (*collective_results[run]).back().best_tour_length;
            sum_res[i].list_avg +=
                    (*collective_results[run]).back().list_avg;
        }
    }
}

void store_lahc_average(string avg_file, vector<lahc_results> &sum_res,
        int runs) {
    string msg = "# iteration bets_tour list_avg\n";
    for (long i = 0; i < sum_res.size(); i++) {
        msg += std::to_string(sum_res[i].iteration) + " " +
                std::to_string(double(sum_res[i].best_tour_length) /
                double(runs))
                + " " + std::to_string(double(sum_res[i].list_avg) /
                double(runs))
                + "\n";
    }
    ofstream averages(avg_file.c_str(), ios_base::out);
    averages << msg;
    averages.close();
}

string report_explahc_result_summary(vector<explahc_special_events>
        **collective_special_events, int num_run) {
    long total_iteration = 0;
    long total_effective_iteration = 0;
    long sum_best_tour_length = 0;
    int biggest_list_run = 0;
    std::vector <explahc_special_events> sum_special_events;
    std::vector<int> lsizes;
    std::vector<int> int_tmp;
    std::vector<long> long_tmp;
    std::vector<double> double_tmp;
    for (int run = 0; run < num_run; run++) {
        for (int l_ind = 0; l_ind < (*collective_special_events[run]).size();
                l_ind++) {
            total_iteration += (*collective_special_events[run])
                    [l_ind].num_iteration;
            total_effective_iteration += (*collective_special_events[run])
                    [l_ind].num_effective_iteration;
            if (l_ind >= sum_special_events.size()) {
                sum_special_events.push_back({(*collective_special_events[run])
                    [l_ind].lsize, 0, 0, 0, 0, 0, 0.});
                lsizes.push_back(0);
            }
            lsizes[l_ind]++;
            sum_special_events[l_ind].best_tour_length +=
                    (*collective_special_events[run])[l_ind].best_tour_length;
            sum_special_events[l_ind].ending_iteration +=
                    (*collective_special_events[run])[l_ind].ending_iteration;
            sum_special_events[l_ind].last_list_avg +=
                    (*collective_special_events[run])[l_ind].last_list_avg;
            sum_special_events[l_ind].num_effective_iteration +=
                    (*collective_special_events[run])[l_ind].num_effective_iteration;
            sum_special_events[l_ind].num_iteration +=
                    (*collective_special_events[run])[l_ind].num_iteration;
            sum_special_events[l_ind].starting_iteration +=
                    (*collective_special_events[run])[l_ind].starting_iteration;

        }
        // compute the biggest list size through all runs
        if ((*collective_special_events[run]).size() > biggest_list_run)
            biggest_list_run = run;
        // compute the sum of all best tour lengths
        sum_best_tour_length +=
                collective_special_events[run]->back().best_tour_length;
    }
    string msg = "";
    msg += "======================================\n";
    msg += std::string(21, ' ') + ">>> avg. best tour:              ";
    msg += std::to_string(double(sum_best_tour_length) / double(num_run)) + "\n";
    msg += std::string(21, ' ') + ">>> avg. iterations:             ";
    msg += std::to_string(double(total_iteration) / double(num_run)) + "\n";
    msg += std::string(21, ' ') + ">>> avg. effective iterations:   ";
    msg += std::to_string(double(total_effective_iteration) / double(num_run))
            + "\n";
    msg += std::string(21, ' ') + ">>> all ran lists:               ";
    int_tmp.reserve(sum_special_events.size());
    for (explahc_special_events& item : sum_special_events)
        int_tmp.push_back(item.lsize);
    msg += vector2string(int_tmp, ",") + "\n";
    int_tmp.reserve(0);
    int_tmp.clear();
    msg += std::string(21, ' ') + ">>> lists num run:               ";
    msg += vector2string(lsizes, ",") + "\n";
    msg += std::string(21, ' ') + ">>> avg. lists iterations:       ";
    long_tmp.reserve(sum_special_events.size());
    for (explahc_special_events& item : sum_special_events)
        long_tmp.push_back(item.num_iteration);
    msg += vector2string(devide_vector(long_tmp, lsizes)) + "\n";
    long_tmp.reserve(0);
    long_tmp.clear();
    msg += std::string(21, ' ') + ">>> avg. lists effec. iters:     ";
    long_tmp.reserve(sum_special_events.size());
    for (explahc_special_events& item : sum_special_events)
        long_tmp.push_back(item.num_effective_iteration);
    msg += vector2string(devide_vector(long_tmp, lsizes)) + "\n";
    long_tmp.reserve(0);
    long_tmp.clear();
    msg += std::string(21, ' ') + ">>> avg. lists bests:            ";
    int_tmp.reserve(sum_special_events.size());
    for (explahc_special_events& item : sum_special_events)
        int_tmp.push_back(item.best_tour_length);
    msg += vector2string(devide_vector(int_tmp, lsizes)) + "\n";
    int_tmp.reserve(0);
    int_tmp.clear();
    msg += std::string(21, ' ') + ">>> avg. last lists avg:         ";
    double_tmp.reserve(sum_special_events.size());
    for (explahc_special_events& item : sum_special_events)
        double_tmp.push_back(item.last_list_avg);
    msg += vector2string(devide_vector(double_tmp, lsizes)) + "\n";
    double_tmp.reserve(0);
    double_tmp.clear();
    msg += std::string(21, ' ') + ">>> avg. lists starting iters:   ";
    long_tmp.reserve(sum_special_events.size());
    for (explahc_special_events& item : sum_special_events)
        long_tmp.push_back(item.starting_iteration);
    msg += vector2string(devide_vector(long_tmp, lsizes)) + "\n";
    long_tmp.reserve(0);
    long_tmp.clear();
    msg += std::string(21, ' ') + ">>> avg. lists ending iters:     ";
    long_tmp.reserve(sum_special_events.size());
    for (explahc_special_events& item : sum_special_events)
        long_tmp.push_back(item.ending_iteration);
    msg += vector2string(devide_vector(long_tmp, lsizes)) + "\n";
    long_tmp.reserve(0);
    long_tmp.clear();
    msg += std::string(21, ' ') + ">>> best result lsize (last):    ";
    int_tmp.reserve(num_run);
    for (int i = 0; i < num_run; i++)
        int_tmp.push_back((*collective_special_events[i]).back().lsize);
    msg += vector2string(int_tmp) + "\n";
    int_tmp.reserve(0);
    int_tmp.clear();

    sum_special_events.resize(0);
    sum_special_events.clear();
    sum_special_events.shrink_to_fit();

    return msg;
}

void store_explahc_runs_average(string avg_file,
        vector<explahc_results> **collective_results, int freq, int runs) {
    vector<explahc_results> sum_results; // to calculate the avg of several runs
    long max_num_iteration = 0;
    for (int i = 0; i < runs; i++)
        if (collective_results[i]->back().iteration > max_num_iteration)
            max_num_iteration = collective_results[i]->back().iteration;
    long step = long(max_num_iteration / freq);
    for (int i = 0; i <= freq; i++)
        sum_results.push_back({i * step, 0, 0, 0., 0, 0, 0, 0.});
    if (max_num_iteration % freq != 0)
        sum_results.push_back({max_num_iteration, 0, 0, 0., 0, 0, 0, 0.});

    for (int run = 0; run < runs; run++) {
        long i = 0;
        for (long iter = 0; iter < collective_results[run]->size(); iter++) {
            if ((*collective_results[run])[iter].iteration ==
                    sum_results[i].iteration) {
                sum_results[i].currect_best_tour_length +=
                        (*collective_results[run])[iter].currect_best_tour_length;
                sum_results[i].current_best_tour_list_avg +=
                        (*collective_results[run])[iter].current_best_tour_list_avg;
                sum_results[i].effecticve_iteration +=
                        (*collective_results[run])[iter].effecticve_iteration;
                sum_results[i].best_tour_length +=
                        (*collective_results[run])[iter].best_tour_length;
                sum_results[i].best_tour_list_avg +=
                        (*collective_results[run])[iter].best_tour_list_avg;
                i++;
            }
            if ((*collective_results[run])[iter].iteration >
                    sum_results[i].iteration) {
                sum_results[i].currect_best_tour_length +=
                        (*collective_results[run])[iter - 1].currect_best_tour_length;
                sum_results[i].current_best_tour_list_avg +=
                        (*collective_results[run])[iter - 1].current_best_tour_list_avg;
                sum_results[i].effecticve_iteration +=
                        (*collective_results[run])[iter - 1].effecticve_iteration;
                sum_results[i].best_tour_length +=
                        (*collective_results[run])[iter - 1].best_tour_length;
                sum_results[i].best_tour_list_avg +=
                        (*collective_results[run])[iter - 1].best_tour_list_avg;
                i++;
            }
        }
        for (; i < sum_results.size(); i++) {
            sum_results[i].currect_best_tour_length +=
                    (*collective_results[run]).back().currect_best_tour_length;
            sum_results[i].current_best_tour_list_avg +=
                    (*collective_results[run]).back().current_best_tour_list_avg;
            sum_results[i].effecticve_iteration +=
                    (*collective_results[run]).back().effecticve_iteration;
            sum_results[i].best_tour_length +=
                    (*collective_results[run]).back().best_tour_length;
            sum_results[i].best_tour_list_avg +=
                    (*collective_results[run]).back().best_tour_list_avg;
        }
    }
    string msg = "# iteration best_ever_length bets_ever_list_average "
            "effective_iteration current_best_tour_length "
            "current_best_tour_list_avg\n";
    for (long i = 0; i < sum_results.size(); i++) {
        msg += std::to_string(sum_results[i].iteration) + " " +
                std::to_string(double(sum_results[i].best_tour_length) /
                double(runs)) + " " +
                std::to_string(double(sum_results[i].best_tour_list_avg) /
                double(runs)) + " " +
                std::to_string(double(sum_results[i].effecticve_iteration) /
                double(runs)) + " " +
                std::to_string(double(sum_results[i].currect_best_tour_length) /
                double(runs)) + " " +
                std::to_string(double(sum_results[i].current_best_tour_list_avg)
                / double(runs)) + "\n";
    }
    ofstream averages(avg_file.c_str(), ios_base::out);
    averages << msg;
    averages.close();
    // free memory
    sum_results.resize(0);
    sum_results.clear();
    sum_results.shrink_to_fit();
}
