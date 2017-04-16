/* 
 * File:    TSP_mutation.cpp
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
 */

#include "TSP_mutation.h"

using namespace std;
extern Random RANDOM; // random number generator

void double_bridge(int *tour, tsp_mut_properties *tsp_mut, int **dis_matrix) {

    tsp_mut->mutation_type = "double_bridge";
    pick_k_without_replacement(2, tsp_mut->num_cities, tsp_mut->cuts);
    if (tsp_mut->cuts[0] > tsp_mut->cuts[1])
        std::swap(tsp_mut->cuts[0], tsp_mut->cuts[1]);
    int subtracted_cost = dis_matrix[tour[(tsp_mut->cuts[0] != 0) ?
            tsp_mut->cuts[0] - 1 : tsp_mut->num_cities - 1]]
            [tour[tsp_mut->cuts[0]]] + dis_matrix[tour[tsp_mut->cuts[1] - 1]]
            [tour[tsp_mut->cuts[1]]];
    int add_cost = dis_matrix[tour[tsp_mut->cuts[0]]][tour[tsp_mut->cuts[1]]] +
            dis_matrix[tour[(tsp_mut->cuts[0] != 0) ?
            tsp_mut->cuts[0] - 1 : tsp_mut->num_cities - 1]]
            [tour[tsp_mut->cuts[1] - 1]];
    tsp_mut->length = tsp_mut->length - subtracted_cost + add_cost;
}

void swap(int *tour, tsp_mut_properties *tsp_mut, int **dis_matrix) {
    tsp_mut->mutation_type = "swap";
    pick_k_without_replacement(2, tsp_mut->num_cities, tsp_mut->cuts);
    if (tsp_mut->cuts[0] > tsp_mut->cuts[1])
        std::swap(tsp_mut->cuts[0], tsp_mut->cuts[1]);
    int index_dis = tsp_mut->cuts[1] - tsp_mut->cuts[0];
    if (index_dis == tsp_mut->num_cities - 1)
        std::swap(tsp_mut->cuts[0], tsp_mut->cuts[1]);
    if (index_dis <= 2 or index_dis == tsp_mut->num_cities - 1) {
        int subtracted_cost = dis_matrix[tour[(tsp_mut->cuts[0] != 0) ?
                tsp_mut->cuts[0] - 1 : tsp_mut->num_cities - 1]]
                [tour[tsp_mut->cuts[0]]] + dis_matrix[tour[tsp_mut->cuts[1]]]
                [tour[(tsp_mut->cuts[1] + 1) % tsp_mut->num_cities]];
        int add_cost = dis_matrix[tour[(tsp_mut->cuts[0] != 0) ?
                tsp_mut->cuts[0] - 1 : tsp_mut->num_cities - 1]]
                [tour[tsp_mut->cuts[1]]] + dis_matrix[tour[tsp_mut->cuts[0]]]
                [tour[(tsp_mut->cuts[1] + 1) % tsp_mut->num_cities]];
        tsp_mut->length = tsp_mut->length - subtracted_cost + add_cost;
        return;
    }
    int subtracted_cost = dis_matrix[tour[(tsp_mut->cuts[0] != 0) ?
            tsp_mut->cuts[0] - 1 : tsp_mut->num_cities - 1]]
            [tour[tsp_mut->cuts[0]]] +
            dis_matrix[tour[tsp_mut->cuts[0]]][tour[tsp_mut->cuts[0] + 1]] +
            dis_matrix[tour[tsp_mut->cuts[1] - 1]][tour[tsp_mut->cuts[1]]] +
            dis_matrix[tour[tsp_mut->cuts[1]]]
            [tour[(tsp_mut->cuts[1] + 1) % tsp_mut->num_cities]];
    int add_cost = dis_matrix[tour[(tsp_mut->cuts[0] != 0) ?
            tsp_mut->cuts[0] - 1 : tsp_mut->num_cities - 1]]
            [tour[tsp_mut->cuts[1]]] +
            dis_matrix[tour[tsp_mut->cuts[1]]][tour[tsp_mut->cuts[0] + 1]] +
            dis_matrix[tour[tsp_mut->cuts[0]]][tour[tsp_mut->cuts[1] - 1]] +
            dis_matrix[tour[tsp_mut->cuts[0]]]
            [tour[(tsp_mut->cuts[1] + 1) % tsp_mut->num_cities]];
    tsp_mut->length = tsp_mut->length - subtracted_cost + add_cost;
}

void displacement(int *tour, tsp_mut_properties *tsp_mut, int **dis_matrix) {
    tsp_mut->mutation_type = "displacement";
    pick_k_without_replacement(2, tsp_mut->num_cities, tsp_mut->cuts);
    int index_dis = tsp_mut->cuts[0] - tsp_mut->cuts[1];
    while (index_dis == tsp_mut->num_cities - 1 || index_dis == -1) {
        pick_k_without_replacement(2, tsp_mut->num_cities, tsp_mut->cuts);
        index_dis = tsp_mut->cuts[0] - tsp_mut->cuts[1];
    }
    int subtracted_cost = dis_matrix[tour[(tsp_mut->cuts[0] != 0) ?
            tsp_mut->cuts[0] - 1 : tsp_mut->num_cities - 1]]
            [tour[tsp_mut->cuts[0]]] + dis_matrix[tour[tsp_mut->cuts[0]]]
            [tour[(tsp_mut->cuts[0] + 1) % tsp_mut->num_cities]] +
            dis_matrix[tour[(tsp_mut->cuts[1] != 0) ?
            tsp_mut->cuts[1] - 1 : tsp_mut->num_cities - 1]]
            [tour[tsp_mut->cuts[1]]];
    int add_cost = dis_matrix[tour[(tsp_mut->cuts[0] != 0) ?
            tsp_mut->cuts[0] - 1 : tsp_mut->num_cities - 1]]
            [tour[(tsp_mut->cuts[0] + 1) % tsp_mut->num_cities]] +
            dis_matrix[tour[(tsp_mut->cuts[1] != 0) ?
            tsp_mut->cuts[1] - 1 : tsp_mut->num_cities - 1]]
            [tour[tsp_mut->cuts[0]]] + dis_matrix[tour[tsp_mut->cuts[0]]]
            [tour[tsp_mut->cuts[1]]];
    tsp_mut->length = tsp_mut->length - subtracted_cost + add_cost;
}

void update_tour(int *tour, tsp_mut_properties *tsp_mut) {
    // update the tour based on the applied mutation
    if (tsp_mut->mutation_type == "double_bridge") {
        int j = tsp_mut->cuts[1];
        int stop = tsp_mut->cuts[0] +
                int((tsp_mut->cuts[1] - tsp_mut->cuts[0] + 1) / 2);
        for (int i = tsp_mut->cuts[0]; i < stop; i++)
            std::swap(tour[i], tour[--j]);
        return;
    }
    if (tsp_mut->mutation_type == "swap") {
        std::swap(tour[tsp_mut->cuts[0]], tour[tsp_mut->cuts[1]]);
        return;
    }
    if (tsp_mut->mutation_type == "displacement") {
        int tmp;
        if (tsp_mut->cuts[0] < tsp_mut->cuts[1]) {
            tmp = tour[tsp_mut->cuts[0]];
            for (int i = tsp_mut->cuts[0]; i < tsp_mut->cuts[1] - 1; i++)
                tour[i] = tour[i + 1];
            tour[tsp_mut->cuts[1] - 1] = tmp;

        } else {
            tmp = tour[tsp_mut->cuts[0]];
            for (int i = tsp_mut->cuts[0]; i > tsp_mut->cuts[1]; i--)
                tour[i] = tour[i - 1];
            tour[tsp_mut->cuts[1]] = tmp;
        }
        return;
    }
}
