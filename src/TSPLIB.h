/* 
 * File:    TSPLIB.h
 * Author:  Mosab Bazargani
 * License: BSD 3-Clause License
 * 
 * Description: Collection of methods to read TSP instances in some of the 
 * formats used in TSPLIB. It supports TSP instances with the following TSPLIB 
 * formats:
 *      1- 2D Euclidean istances:           EDGE_WEIGHT_TYPE = EUC_2D
 *	2- 2D Euclidean instances with ceilings used in distance calculation:
 *                                          EDGE_WEIGHT_TYPE = CEIL_2D
 *      3- Upper triangular distance matrices (including the diagonal):
 *                                          EDGE_WEIGHT_TYPE = EXPLICIT,
 *                                          EDGE_WEIGHT_FORMAT = UPPER_DIAG_ROW
 * NOTE:
 *      This code is based on the code for DIMACS -- developed by Lyle McGeoch.
 *
 * Created on April 1, 2016, 7:44 PM
 */

#ifndef TSPLIB_H
#define TSPLIB_H

#include "Util.h"
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <iostream>

using namespace std;

typedef struct {
    string NAME;
    string TYPE;
    string COMMENT;
    int DIMENSION;
    string FORMAT;
    string EDGE_WEIGHT_TYPE;
    string EDGE_WEIGHT_FORMAT;
} tsplib_properties;

class TSPLIB {
public:
    string tsp_instance_name;
    int **distance_matrix;
    double **cities;
    tsplib_properties properties;
    TSPLIB(string tsp_instance_file);
    ~TSPLIB();
    long tour_length(int *tour);
    bool check_tour(int *tour);

private:
    void read_tsp_instance();
    void read_node_coord_section(ifstream &inputfile);
    void read_edge_weight_section(ifstream &inputfile);
    void set_property(string &key, string &value);
    void set_distance_matrix();
    int euc_2d(int city1, int city2);
    int ceil_2d(int city1, int city2);
};

#endif /* TSPLIB_H */

