/* 
 * File:    TSP_mutation.h
 * Author:  Mosab Bazargani
 * License: BSD 3-Clause License
 * 
 * Description: Collection of different mutation operators for TSP problem with
 * 'path representation'.
 * All mutation operators in this file use a method -- named
 * 'remove_common_edges' to remove all common edges in subtractive
 * and add-in edges after applying one of them. It is designed for
 * SYMMETRIC TSP.
 * All operators return a mutated tour and a mutated edges.
 * NOTE:
 *      * This operators are elaborated in the publication by Larranaga, 1999.
 *      * We only use 'double_bridge' operator as Burke and Bykov only used that
 *        operator in their 2017 version of LAHC.
 *
 * Created on April 3, 2016, 5:23 PM
 */

#ifndef TSP_MUTATION_H
#define TSP_MUTATION_H

#include "Util.h"
#include "Random.h"
#include <stdlib.h> 
#include <algorithm>
#include <iostream>

using namespace std;

typedef struct {
    string mutation_type;
    int *cuts;
    int length;
    int num_cities;
} tsp_mut_properties;

void double_bridge(int *tour, tsp_mut_properties *tsp_mut, int **dis_matrix);
void swap(int *tour, tsp_mut_properties *tsp_mut, int **dis_matrix);
void displacement(int *tour, tsp_mut_properties *tsp_mut, int **dis_matrix);
void update_tour(int *tour, tsp_mut_properties *tsp_mut);

#endif /* TSP_MUTATION_H */

