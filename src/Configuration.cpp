/*
 * File:    Configuration.cpp
 * Author:  Mosab Bazargani
 * License: BSD 3-Clause License
 *
 * Description: The Configuration class reads the given input file.
 * 
 */

#include "Configuration.h"

using namespace std;

Configuration::Configuration(string input) {
    inputfile = input;
    std::ifstream in(inputfile);
    stopping.solution_quality = stopping.max_iteration =
            stopping.point_of_convergence = false;
    read_inputfile(in);
    in.close();
}

void Configuration::read_inputfile(ifstream &in) {
    if (in.good() != 1)
        error("The inputfile `" + inputfile + "` does NOT exist!");
    string line, key, value;
    int lineNo = 0;
    while (!in.eof()) {
        getline(in, line);
        remove_spaces(line);
        if (!line.empty()) // ignore empty lines
        {
            if (line[0] != '#') //ignore lines starting with '#'
            {
                size_t firstColomn = line.find_first_of(":");
                if (firstColomn >= line.size()) {
                    cout << "line number: " << lineNo << " --> ";
                    cout << line << endl;
                    error("missing colomn in the given inputfile!"); // error
                }
                key = line.substr(0, firstColomn);
                // and obtain its value
                value = line.substr(firstColomn + 1, line.size());
                trim_string_blanks(value);
                set_parameter(key, value);
            }
        }
    }
    //     create the output file name
    if (output == "" && algorithm == "LAHC") {
        size_t filename = problem_name.find_first_of(".");
        output = problem_name.substr(0, filename) + "_" + algorithm + "_lsize" +
                to_string(list_size);
    } else if (output == "" && (algorithm == "pLAHC" ||
            algorithm == "pLAHC-s")) {
        size_t filename = problem_name.find_first_of(".");
        output = problem_name.substr(0, filename) + "_" + algorithm;
        if (stopping.solution_quality == true)
            output = output + "_sq" + to_string(
                    int(stopping.solution_quality_par));
        if (stopping.max_iteration == true)
            output = output + "_miter" + to_string(stopping.max_iteration_par);
        if (stopping.point_of_convergence == true)
            output = output + "_poc" +
                to_string(int(stopping.point_of_convergence_par));
    }

    // remove log, progress, and avg files, if they exist.
    string file_name = output + ".log";
    remove(file_name.c_str());
    file_name = output + ".progress";
    remove(file_name.c_str());
    file_name = output + ".avg";
    remove(file_name.c_str());

    // update the stopping criterion parameter
    if (stopping.point_of_convergence == true)
        stopping.point_of_convergence_par = 100.0 /
            stopping.point_of_convergence_par;
}

void Configuration::set_parameter(string& key, string& value) {
    if (key == "inputfile") {
        inputfile = value;
    } else if (key == "problem_name") {
        problem_name = value;
    } else if (key == "algorithm") {
        algorithm = value;
    } else if (key == "seed") {
        seed = atoi(value.c_str());
    } else if (key == "number_run") {
        num_run = atoi(value.c_str());
    } else if (key == "list_size") {
        list_size = atoi(value.c_str());
    } else if (key == "base_list_size") {
        base_list_size = atoi(value.c_str());
    } else if (key == "list_scaling_size") {
        list_scaling_size = atoi(value.c_str());
    } else if (key == "stopping_criterion") {
        set_stopping_criterion_parameter(value);
    } else if (key == "restore_progress") {
        restore_progress = value;
    } else if (key == "restore_special_events") {
        restore_special_events = value;
    } else if (key == "restore_best_results") {
        restore_best_results = value;
    } else if (key == "output") {
        output = value;
    } else {
        error("This should never happen --> set_parameter()");
    }
}

inline void Configuration::set_stopping_criterion_parameter(string& value) {
    size_t firstColomn = value.find_first_of(",");
    if (firstColomn >= value.size())
        error("stopping_criterion and its parameter is not "
            "given correctly!"); // error
    string criterion, parameter;
    criterion = value.substr(0, firstColomn);
    parameter = value.substr(firstColomn + 1, value.size());
    trim_string_blanks(criterion);
    trim_string_blanks(parameter);
    if (criterion == "solution_quality") {
        stopping.solution_quality = true;
        stopping.solution_quality_par = stod(parameter.c_str());
    }
    if (criterion == "point_of_convergence") {
        stopping.point_of_convergence = true;
        stopping.point_of_convergence_par = stod(parameter.c_str());
    }
    if (criterion == "max_iteration") {
        stopping.max_iteration = true;
        stopping.max_iteration_par = atoi(parameter.c_str());
    }
}

string Configuration::dump_parameters() {
    string parameters = "Application parameters set as the following:\n";
    parameters += std::string(10, ' ') + "inputfile:                      ";
    parameters += inputfile + "\n";
    parameters += std::string(10, ' ') + "problem_name:                   ";
    parameters += problem_name + "\n";
    parameters += std::string(10, ' ') + "algorithm:                      ";
    parameters += algorithm + "\n";
    parameters += std::string(10, ' ') + "seed:                           ";
    parameters += std::to_string(seed) + "\n";
    parameters += std::string(10, ' ') + "num_run:                        ";
    parameters += std::to_string(num_run) + "\n";
    parameters += std::string(10, ' ') + "list_size:                      ";
    parameters += std::to_string(list_size) + "\n";
    parameters += std::string(10, ' ') + "base_list_size:                 ";
    parameters += std::to_string(base_list_size) + "\n";
    parameters += std::string(10, ' ') + "list_scaling_size:              ";
    parameters += std::to_string(list_scaling_size) + "\n";
    if (stopping.solution_quality == true) {
        parameters += std::string(10, ' ') + "stopping_criterion:             ";
        parameters += "solution_quality, " +
                std::to_string(stopping.solution_quality_par) + "\n";
    }
    if (stopping.max_iteration == true) {
        parameters += std::string(10, ' ') + "stopping_criterion:             ";
        parameters += "max_iteration, " +
                std::to_string(stopping.max_iteration_par) + "\n";
    }
    if (stopping.point_of_convergence == true) {
        parameters += std::string(10, ' ') + "stopping_criterion:             ";
        parameters += "point_of_convergence, " +
                std::to_string(100. / stopping.point_of_convergence_par) + "\n";
    }
    parameters += std::string(10, ' ') + "restore_progress:               ";
    parameters += restore_progress + "\n";
    parameters += std::string(10, ' ') + "log_file:                       ";
    parameters += output + ".log\n";
    parameters += std::string(10, ' ') + "progress_file:                  ";
    parameters += output + ".progress\n";
    parameters += std::string(10, ' ') + "best_obtained_results_file:     ";
    parameters += output + ".bests\n";
    parameters += std::string(10, ' ') + "avg_file:                       ";
    parameters += output + ".avg\n";
    return parameters;
}
