/* 
 * File:    Util.cpp
 * Author:  Mosab Bazargani
 * Lisence: BSD 3-Clause License
 *
 * Description: Collection of utilities not better suited for placement 
 * in other files.
 * 
 */

#include "Util.h"

using namespace std;

extern Random RANDOM; // random number generator


// outputs an error msg and stops the program
void error(string msg) {
    cout << msg << endl;
    exit(1);
}

// remove leading and trailing blanks (white spaces and tabs)  from string 's'
void trim_string_blanks(string &s) {
    if (s.empty()) return;

    int startIndex = s.find_first_not_of(" \t");
    int endIndex = s.find_last_not_of(" \t");
    string tempString = s;
    s.erase();

    s = tempString.substr(startIndex, (endIndex - startIndex + 1));
}

// remove all blanks (white spaces and tabs) from string 's'
void remove_spaces(string &s) {
    string temp;
    if (s.empty()) return;
    for (int i = 0; i < s.size(); i++)
        if (s.substr(i, 1) != " " && s.substr(i, 1) != "\t")
            temp.append(s.substr(i, 1));
    s.erase();
    s = temp;
}

// return true if 'elem' is a member of array 'arr'.
bool member(int elem, int *arr, int arr_size) {
    for (int i = 0; i < arr_size; i++)
        if (elem == arr[i]) return true;
    return false;
}

// on return, the array 'pick' will contain 'k' 
// random numbers without repetition from the set: 0,1,2,...,'n'-1
//
// NOTE: the implementation differs (for efficiency reasons) depending 
//       on whether 'k' is much smaller than 'n' or not.
void pick_k_without_replacement(int k, long n, int *arr) {
    assert(k <= n);
    if (k == n)
        makeshuffle(n, arr, n); // a random permutation
    else if (k > n / 3) // cutoff point at n/3
        makeshuffle(k, arr, n);
    else {
        int elem;
        int picked = 0;
        while (picked < k) {
            elem = RANDOM.uniform(0, n - 1); // a random number in 0..n-1
            // check that elem is not already picked
            if (!member(elem, arr, picked)) {
                arr[picked] = elem;
                picked++;
            }
        }
    }
}

// on return, the first 'k' elements of 'shufflearray' will
// be random numbers without repetition from the set: 0,1,2,...,n-1
// the case of 'k==n' corresponds to a random permutation
void makeshuffle(int k, int *arr, const int n) {
    long i;

    if (k == n) {
        // initialize
        for (i = 0; i < n; i++) arr[i] = i;
        // shuffle 
        for (i = 0; i < k; i++) {
            int other = RANDOM.uniform(i, n - 1);
            int temp = arr[other];
            arr[other] = arr[i];
            arr[i] = temp;
        }
    } else { // k < n
        // initialize
        int *shufflearray = new int[n];
        for (i = 0; i < n; i++) shufflearray[i] = i;
        // shuffle 
        for (i = 0; i < k; i++) {
            int other = RANDOM.uniform(i, n - 1);
            int temp = shufflearray[other];
            shufflearray[other] = shufflearray[i];
            shufflearray[i] = temp;
        }
        for (i = 0; i < k; i++)
            arr[i] = shufflearray[i];
        delete [] shufflearray;
    }
}

//write in the log file
void logging(string log_file, string message) {
    time_t t = time(0); // get time now
    struct tm * now = localtime(& t);
    char buffer [80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S. ", now);
    ofstream logfile(log_file.c_str(), ios_base::app);
    logfile << buffer << message << endl;
    logfile.close();
}

void logging(ofstream &logfile, string message) {
    time_t t = time(0); // get time now
    struct tm * now = localtime(& t);
    char buffer [80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S. ", now);
    logfile << buffer << message << endl;
}

string integer2string(long number, int num_digit) {
    assert(std::to_string(number).length() <= num_digit);
    if (std::to_string(number).length() == num_digit)
        return std::to_string(number);
    return "0" + integer2string(number, num_digit - 1);
}

string vector2string(std::vector <int> lists, string separator) {
    string str = "";
    for (long i = 0; i < lists.size(); i++) {
        if (str != "")
            str += separator + " ";
        str += std::to_string(lists[i]);
    }
    return str;
}

string vector2string(std::vector <long> lists, string separator) {
    string str = "";
    for (long i = 0; i < lists.size(); i++) {
        if (str != "")
            str += separator + " ";
        str += std::to_string(lists[i]);
    }
    return str;
}

string vector2string(std::vector <double> lists, string separator) {
    string str = "";
    for (long i = 0; i < lists.size(); i++) {
        if (str != "")
            str += separator + " ";
        str += std::to_string(lists[i]);
    }
    return str;
}

std::vector<double> devide_vector(std::vector<int> vec, std::vector<int> dev) {
    std::vector <double> result;
    result.reserve(vec.size());
    for (int i = 0; i < vec.size(); i++)
        result.push_back(double(vec[i]) / double(dev[i]));
    return result;
}

std::vector<double> devide_vector(std::vector<long> vec, std::vector<int> dev) {
    std::vector <double> result;
    result.reserve(vec.size());
    for (int i = 0; i < vec.size(); i++)
        result.push_back(double(vec[i]) / double(dev[i]));
    return result;
}

std::vector<double> devide_vector(std::vector<double> vec, std::vector<int> dev) {
    std::vector <double> result;
    result.reserve(vec.size());
    for (int i = 0; i < vec.size(); i++)
        result.push_back(double(vec[i]) / double(dev[i]));
    return result;
}
