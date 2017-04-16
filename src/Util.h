/* 
 * File:    Util.h
 * Author:  Mosab Bazargani
 * Lisence: BSD 3-Clause License
 *
 * Description: Collection of utilities not better suited for placement 
 * in other files.
 * 
 * Created on March 31, 2016, 3:25 PM
 */

#ifndef UTIL_H
#define UTIL_H

#include "Random.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <random>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <vector>
using namespace std;

typedef struct {
    long iteration;
    int best_tour_length;
    double list_avg;
} lahc_results;

typedef struct {
    int lsize;
    long starting_iteration;
    long ending_iteration; // default should be as starting_iteration
    long num_iteration;
    long num_effective_iteration;
    int best_tour_length;
    double last_list_avg;
} explahc_special_events;

typedef struct {
    long iteration;
    long currect_best_tour_length;
    int current_best_tour_lsize;
    double current_best_tour_list_avg;
    long effecticve_iteration;
    long best_tour_length;
    int best_tour_lsize;
    double best_tour_list_avg;
} explahc_results;

// outputs an error msg and stops the program 
void error(string msg);

// remove leading and trailing blanks (white spaces and tabs)  from string 's'
void trim_string_blanks(string &s);

// remove all blanks (white spaces and tabs) from string 's'
void remove_spaces(string &s);

// for random function
void makeshuffle(int k, int *shufflearray, const int n);
void pick_k_without_replacement(int k, long n, int *arr);

//write in the log file
void logging(string log_file, string message);
void logging(ofstream &log_file, string message);

// transfer an integer number to a string with a given size
string integer2string(long number, int num_digit);

// transfer a vector to an string with a given splitter
// default for separator is an space
string vector2string(std::vector <int> lists, string separator = "");
string vector2string(std::vector <long> lists, string separator = "");
string vector2string(std::vector <double> lists, string separator = "");

std::vector<double> devide_vector(std::vector<int> vec, std::vector<int> dev);
std::vector<double> devide_vector(std::vector<long> vec, std::vector<int> dev);
std::vector<double> devide_vector(std::vector<double> vec, std::vector<int> dev);

#endif /* UTIL_H */

