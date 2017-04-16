/* 
 * File:    TSPLIB.cpp
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
 */
#include "TSPLIB.h"

using namespace std;

TSPLIB::TSPLIB(string tsp_instance_file) {
    tsp_instance_name = tsp_instance_file;
    read_tsp_instance();
    if (properties.FORMAT == "NODE_COORD_SECTION")
        set_distance_matrix();
}

TSPLIB::~TSPLIB() {
    for (int i = properties.DIMENSION - 1; i >= 0; i--)
        delete [] distance_matrix[i];
    delete [] distance_matrix;
    if (properties.FORMAT == "NODE_COORD_SECTION") {
        for (int i = properties.DIMENSION - 1; i >= 0; i--)
            delete [] cities[i];
        delete [] cities;
    }
}

void TSPLIB::read_tsp_instance() {
    std::ifstream in(tsp_instance_name);
    if (in.good() != 1)
        error("Couldn't open the given TSP instance file!");
    string line, key, value;
    int lineNo = 0;
    while (!in.eof() && properties.FORMAT == "") {
        getline(in, line);
        lineNo++;
        trim_string_blanks(line);
        if (!line.empty()) // ignore empty lines
        {
            if (line[0] != '#') //ignore lines starting with '#'
            {
                if (properties.FORMAT == "") {
                    if (line == "NODE_COORD_SECTION" or
                            line == "EDGE_WEIGHT_SECTION") {
                        properties.FORMAT = line;
                        break;
                    }
                    size_t firstColomn = line.find_first_of(":");
                    if (firstColomn >= line.size()) {
                        cout << "line number: " << lineNo << " --> ";
                        cout << line << endl;
                        error("missing colomn in the given inputfile!"); // error
                    }
                    key = line.substr(0, firstColomn);
                    value = line.substr(firstColomn + 1, line.size());
                    trim_string_blanks(key);
                    trim_string_blanks(value);
                    set_property(key, value);
                }
            }
        }
    }
    if (properties.FORMAT == "NODE_COORD_SECTION")
        read_node_coord_section(in);
    else if (properties.FORMAT == "EDGE_WEIGHT_SECTION")
        read_edge_weight_section(in);
    else
        error("Unexpected input line: " + line);
    in.close();
}

void TSPLIB::set_property(string& key, string& value) {
    if (key == "NAME") {
        properties.NAME = value;
    } else if (key == "TYPE") {
        properties.TYPE = value;
    } else if (key == "COMMENT") {
        if (properties.COMMENT != "")
            properties.COMMENT.append("\n" + value);
        else
            properties.COMMENT.append(value);
    } else if (key == "DIMENSION") {
        properties.DIMENSION = atoi(value.c_str());
    } else if (key == "FORMAT") {
        properties.FORMAT = value;
    } else if (key == "EDGE_WEIGHT_TYPE") {
        properties.EDGE_WEIGHT_TYPE = value;
    } else if (key == "EDGE_WEIGHT_FORMAT") {
        properties.EDGE_WEIGHT_FORMAT = value;
    }
}

void TSPLIB::read_edge_weight_section(ifstream& inputfile) {
    string in;
    distance_matrix = new int*[properties.DIMENSION];
    for (int i = 0; i < properties.DIMENSION; ++i)
        distance_matrix[i] = new int[properties.DIMENSION];
    for (int i = 0; i < properties.DIMENSION; i++) {
        for (int j = i; j < properties.DIMENSION; j++) {
            inputfile >> in;
            distance_matrix[i][j] = distance_matrix[j][i] = atoi(in.c_str());
        }
    }
    return;
}

void TSPLIB::read_node_coord_section(ifstream& inputfile) {
    double x, y;
    int k;
    string line;
    cities = new double*[properties.DIMENSION];
    for (int i = 0; i < properties.DIMENSION; ++i)
        cities[i] = new double[2];
    while (!inputfile.eof()) {
        getline(inputfile, line);
        trim_string_blanks(line);
        if (!line.empty() && line != "EOF") // ignore empty lines
        {
            sscanf(line.c_str(), "%d %lf %lf", &k, &x, &y);
            cities[k - 1][0] = x;
            cities[k - 1][1] = y;
        }
    }
    return;
}

void TSPLIB::set_distance_matrix() {
    // here we calculate all distances and fill the matrix distance_matrix
    distance_matrix = new int*[properties.DIMENSION];
    for (int i = 0; i < properties.DIMENSION; ++i)
        distance_matrix[i] = new int[properties.DIMENSION];
        
    if (properties.EDGE_WEIGHT_TYPE == "EUC_2D") {
        for (int i = 0; i < properties.DIMENSION; i++)
            for (int j = 0; j < properties.DIMENSION; j++)
                distance_matrix[i][j] = euc_2d(i, j);
        return;
    }
    if (properties.EDGE_WEIGHT_TYPE == "CEIL_2D") {
        for (int i = 0; i < properties.DIMENSION; i++)
            for (int j = 0; j < properties.DIMENSION; j++)
                distance_matrix[i][j] = ceil_2d(i, j);
        return;
    }
}

long TSPLIB::tour_length(int* tour) {
    long length = 0;
    for (int i = 0; i < properties.DIMENSION - 1; i++)
        length += distance_matrix[tour[i]][tour[i + 1]];
    length += distance_matrix[tour[properties.DIMENSION - 1]][tour[0]];
    return length;
}

int TSPLIB::euc_2d(int city1, int city2) {
    double xd = cities[city1][0] - cities[city2][0];
    double yd = cities[city1][1] - cities[city2][1];
    double dist = sqrt(xd * xd + yd * yd) + 0.5;
    return (int) dist;
}

int TSPLIB::ceil_2d(int city1, int city2) {
    double xd = cities[city1][0] - cities[city2][0];
    double yd = cities[city1][1] - cities[city2][1];
    double dist = ceil(sqrt(xd * xd + yd * yd));
    return (int) dist;
}

bool TSPLIB::check_tour(int* tour) {
    for (int i = 0; i < properties.DIMENSION; i++)
        if (tour[i] < 0 || tour[i] >= properties.DIMENSION)
            error("There is/are city number in the given tour out of "
                "correct range!");
    long tour_sum = 0;
    for (int i = 0; i < properties.DIMENSION; i++)
        tour_sum += tour[i];
    if (tour_sum != (properties.DIMENSION * (properties.DIMENSION - 1) / 2.))
        error("The given tour has duplication!");
    return true;
}
