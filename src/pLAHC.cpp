/* 
 * File:    pLAHC.cpp
 * Author:  Mosab Bazargani
 * License: BSD 3-Clause License
 * 
 * Description: This is an implementation of Parameter-less Late Acceptance 
 * Hill-Climbing (pLAHC) algorithm (the work is published in GECCO'17).
 *
 */
#include "pLAHC.h"

using namespace std;

extern Random RANDOM; // random number generator

pLAHC::pLAHC(TSPLIB& tsp, stopping_criteria stop_criteria,
        int base_lsize, int list_ssize) {
    stp_criteria = stop_criteria;
    base_list_size = base_lsize;
    list_scaling_size = list_ssize;
    tsp_instance = &tsp;
    current_index = 0;
    total_iteration = 0;
    total_effective_iteration = 0;
    initial_new_list(base_list_size);
    best_ever_tour_length = race_track[0]->best_tour_size();
    best_ever_tour_index = 0;
}

pLAHC::~pLAHC() {
    for (int i = 0; i < race_track.size(); i++)
        delete race_track[i];
    race_track.resize(0);
    race_track.clear();
    race_track.shrink_to_fit();
    special_events.resize(0);
    special_events.clear();
    special_events.shrink_to_fit();
}

void pLAHC::run() {
    //    push_improvement();
    while (stop()) {
        race_track[current_index]->run();
        total_iteration += race_track[current_index]->num_iterations();
        total_effective_iteration += race_track[current_index]->results.size();
        // update special events
        update_special_even();
        // update the best tour info
        if (race_track[current_index]->best_tour_size() <
                best_ever_tour_length) {
            best_ever_tour_length = race_track[current_index]->best_tour_size();
            best_ever_tour_index = current_index;
        }
        // active the next running individual
        current_index++;
        initial_new_list(base_list_size * pow(list_scaling_size, current_index));
    }
    special_events.pop_back();
}

void pLAHC::initial_new_list(int lsize) {
    race_track.push_back(new LAHC(*tsp_instance, stp_criteria, lsize));
    initial_special_event.lsize = lsize;
    initial_special_event.starting_iteration = total_iteration;
    initial_special_event.ending_iteration = -1;
    initial_special_event.num_iteration = -1;
    initial_special_event.num_effective_iteration = -1;
    initial_special_event.best_tour_length = -1;
    initial_special_event.last_list_avg = -1.;
    special_events.push_back(initial_special_event);
}

void pLAHC::update_special_even() {
    special_events[current_index].ending_iteration = total_iteration;
    special_events[current_index].num_iteration =
            race_track[current_index]->num_iterations();
    special_events[current_index].num_effective_iteration =
            race_track[current_index]->results.size();
    special_events[current_index].best_tour_length =
            race_track[current_index]->best_tour_size();
    special_events[current_index].last_list_avg =
            race_track[current_index]->list_average();
}

bool pLAHC::stop() {
    if (total_iteration == 0)
        return true;
    if (stp_criteria.solution_quality)
        return best_ever_tour_length > stp_criteria.solution_quality_par;
    error("The `stopping_criterion` is not implemented --> pLAHC::stop()");
    return false;
}

string pLAHC::report_best_result() {
    string msg = "";
    msg += ">> total num iterations:         ";
    msg += std::to_string(total_iteration) + "\n";
    msg += std::string(21, ' ') + ">> best ever tour length:        ";
    msg += std::to_string(best_ever_tour_length) + "\n";
    msg += std::string(21, ' ') + ">> best ever tour list size:     ";
    msg += std::to_string(int(base_list_size *
            pow(list_scaling_size, best_ever_tour_index))) + "\n";
    msg += std::string(21, ' ') + ">> best tour num iterations:     ";
    msg += std::to_string(
            race_track[best_ever_tour_index]->num_iterations()) + "\n";
    msg += std::string(21, ' ') + ">> total effective iterations:   ";
    msg += std::to_string(number_total_imrovements()) + "\n";
    msg += std::string(21, ' ') + ">> list sizes:                   ";
    msg += vector2string(activated_lsizes(), ",") + "\n";
    msg += std::string(21, ' ') + ">> lists bests:                  ";
    msg += vector2string(best_tours_lsizes(), ",") + "\n";
    msg += std::string(21, ' ') + ">> lists effective iterations:   ";
    msg += vector2string(lsizes_number_improvments(), ",") + "\n";
    msg += std::string(21, ' ') + ">> lists iterations:             ";
    msg += vector2string(lsizes_number_iteration(), ",") + "\n";
    msg += std::string(21, ' ') + ">> lists avgs.:                  ";
    msg += vector2string(lsizes_avgs(), ",") + "\n";
    msg += std::string(21, ' ') + "--------------------------------------";
    return msg;
}

inline std::vector<int> pLAHC::activated_lsizes() {
    std::vector<int> activated;
    for (int i = 0; i < special_events.size(); i++)
        activated.push_back(special_events[i].lsize);
    return activated;
}

inline std::vector<int> pLAHC::best_tours_lsizes() {
    std::vector<int> bests;
    for (int i = 0; i < special_events.size(); i++)
        bests.push_back(special_events[i].best_tour_length);
    return bests;
}

inline std::vector<long> pLAHC::lsizes_number_iteration() {
    std::vector<long> iters;
    for (int i = 0; i < special_events.size(); i++)
        iters.push_back(special_events[i].num_iteration);
    return iters;
}

inline std::vector<double> pLAHC::lsizes_avgs() {
    std::vector<double> avgs;
    for (int i = 0; i < special_events.size(); i++)
        avgs.push_back(special_events[i].last_list_avg);
    return avgs;
}

inline std::vector<long> pLAHC::lsizes_number_improvments() {
    std::vector<long> improves;
    for (int i = 0; i < special_events.size(); i++)
        improves.push_back(special_events[i].num_effective_iteration);
    return improves;
}

inline long pLAHC::number_total_imrovements() {
    long total_improves = 0;
    for (int i = 0; i < special_events.size(); i++)
        total_improves += special_events[i].num_effective_iteration;
    return total_improves;
}

long pLAHC::num_iterations() {
    return total_iteration;
}

void pLAHC::report_progress(string prog_file, string run_num) {
    // it will store the progress in a file with name <output>.progress
    ofstream progress(prog_file.c_str(), ios_base::app);
    progress << "## Run number " << run_num << endl;
    long effective_iter = 0;
    long total_iter = 1;
    int best_so_far = race_track[0]->results[0].best_tour_length;
    int best_so_far_lsize = special_events[0].lsize;
    for (int lsize_index = 0; lsize_index < race_track.size(); lsize_index++) {
        for (long l_indicator = 0; l_indicator <
                race_track[lsize_index]->results.size(); l_indicator++) {
            if (best_so_far > race_track[lsize_index]->results[l_indicator].best_tour_length) {
                best_so_far = race_track[lsize_index]->results[l_indicator].best_tour_length;
                best_so_far_lsize = special_events[lsize_index].lsize;
            }
            progress << "(" << std::to_string(total_iter +
                    race_track[lsize_index]->results[l_indicator].iteration) <<
                    ", " << std::to_string(++effective_iter) << ", " <<
                    std::to_string(best_so_far) << ", " <<
                    std::to_string(best_so_far_lsize) << ", " <<
                    std::to_string(special_events[lsize_index].lsize) << ", " <<
                    std::to_string(race_track[lsize_index]->
                    results[l_indicator].best_tour_length) << ", " <<
                    std::to_string(race_track[lsize_index]->
                    results[l_indicator].iteration + 1) << ", " <<
                    std::to_string(l_indicator + 1) << ", " <<
                    std::to_string(race_track[lsize_index]->
                    results[l_indicator].list_avg) << ")";
        }
        total_iter += special_events[lsize_index].num_iteration;
    }
    progress << endl;
    progress.close();
}

void pLAHC::report_special_events(string events_file, string run_num) {
    // it will store the special events in a file with name <output>.events
    ofstream events(events_file.c_str(), ios_base::app);
    events << "## Run number " << run_num << endl;
    for (long i = 0; i < special_events.size(); i++)
        events << "(" << std::to_string(special_events[i].lsize) << ", " <<
        std::to_string(special_events[i].starting_iteration) << ", " <<
        std::to_string(special_events[i].ending_iteration) << ", " <<
        std::to_string(special_events[i].num_iteration) << ", " <<
        std::to_string(special_events[i].num_effective_iteration) << ", " <<
        std::to_string(special_events[i].best_tour_length) << ", " <<
        std::to_string(special_events[i].last_list_avg) << ")" << endl;
    events.close();
}

void pLAHC::report_best_results(string best_results_file, string run_num) {
    // it will store best results obtained in each run in a file 
    ofstream bests(best_results_file.c_str(), ios_base::app);
    bests << run_num << " " << std::to_string(total_iteration) << " " <<
            std::to_string(best_ever_tour_length) << " " << std::to_string(
            int(base_list_size * pow(list_scaling_size,
            best_ever_tour_index))) << " " << std::to_string(
            race_track[best_ever_tour_index]->num_iterations())
            << " " << std::to_string(race_track[best_ever_tour_index]->
            list_average()) << endl;
    bests.close();
}

void pLAHC::copy_special_events(vector<explahc_special_events>
        **collective_special_events, int run) {
    for (int i = 0; i < special_events.size(); i++)
        collective_special_events[run]->push_back(special_events[i]);
}

void pLAHC::copy_results(vector<explahc_results>
        **collective_results, int run) {
    long total_effect_iter = 0;
    int best_tour_length = race_track[0]->results[0].best_tour_length;
    int best_tour_lsize = special_events[0].lsize;
    double best_tour_list_avg = race_track[0]->results[0].list_avg;
    for (int l_ind = 0; l_ind < special_events.size(); l_ind++)
        for (long j = 0; j < race_track[l_ind]->results.size(); j++) {
            if (race_track[l_ind]->results[j].best_tour_length <=
                    best_tour_length) {
                best_tour_length =
                        race_track[l_ind]->results[j].best_tour_length;
                best_tour_lsize = special_events[l_ind].lsize;
                best_tour_list_avg = race_track[l_ind]->results[j].list_avg;
            }
            collective_results[run]->push_back({
                special_events[l_ind].starting_iteration +
                race_track[l_ind]->results[j].iteration,
                race_track[l_ind]->results[j].best_tour_length,
                special_events[l_ind].lsize,
                race_track[l_ind]->results[j].list_avg,
                ++total_effect_iter, best_tour_length, best_tour_lsize,
                best_tour_list_avg
            });
        }
}
