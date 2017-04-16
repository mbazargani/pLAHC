/* 
 * File:    Statistic.h
 * Author:  Mosab Bazargani
 * License: BSD 3-Clause License
 *
 * Description: This is a collection of several methods to do the statistical 
 * analysis over several individual runs.
 * 
 * Created on April 18, 2016, 12:03 AM
 */

#ifndef STATISTIC_H
#define STATISTIC_H

#include "Util.h"
#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

void lahc_runs_avg(vector<lahc_results> **collective_results,
        vector<lahc_results> &sum_res, int freq, int num_runs);
void store_lahc_average(string avg_file, vector<lahc_results> &sum_res,
        int runs);
string report_explahc_result_summary(vector<explahc_special_events>
            **collective_special_events, int num_run);
void store_explahc_runs_average(string avg_file, 
        vector<explahc_results> **collective_results, int freq, int runs);

#endif /* STATISTIC_H */