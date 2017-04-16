/* 
 * File:                Random.cpp
 * Author:              Fernando Lobo (fernando.lobo@gmail.com)
 * 
 * Description:         C++ implementation for the class Random.
 *                      Random number generator.
 *                      Prime Modulus Multiplicative Linear Congruential Generator (PMMLCG).
 *                    
 *
 * Created on:          1996
 * Last modified:       April 8, 2011, 2:29 PM
 */


#include "Random.h"

#include <iostream>
#include <cmath>
#include <cassert>


using namespace std;


//
// if a seed is not given, ask for one.
// seed must be an integer between 1 and m-1
//

void Random::randomize(long seed) {
    if (seed == UNSET) {
        cout << "Enter seed: ";
        cin >> Seed;
    } else
        Seed = seed;
    assert(seed > 0 && seed < M);
}

//
// seed must be a real number between 0 and 1
//

void Random::randomize(double seed) {
    long longSeed = long( 1 + seed * (M - 2));
    Seed = longSeed;
    assert(Seed > 0 && Seed < M);
}

//
// generate a random number distributed uniformly in 0..1
//

double Random::uniform01() {
    long lo, hi, test;

    hi = Seed / Q;
    lo = Seed % Q;
    test = A * lo - R * hi;
    if (test > 0)
        Seed = test;
    else
        Seed = test + M;
    return double(Seed) / M;
}

//
// generate a random number distributed uniformly in a..b
//

double Random::uniform(double a, double b) {
    assert(b >= a);
    return (b - a) * uniform01() + a;
}

long Random::uniform(long a, long b) {
    assert(b >= a);
    //if (a == b) return a;
    int i = int( (b - a + 1) * uniform01() + a);
    if (i > b) i = b;
    return i;
}

//
// generate a random number with an exponential distribution with mean 'mu'
// note: log(x) gives the natural logarithm of x.
//

double Random::exponential(double mu) {
    double U = uniform01();
    return -mu * log(U);
}

//
// generate a Bernoulli trial with probability of success 'p'
//

bool Random::bernoulli(double p) {
    return (uniform01() < p);
}

//
// generate a sample from a Geometric random variable with parameter 'p'.
// returns how many successes not including the first failure. 
// 'p' is the probability of failure.
//

long Random::geometric(double p) {
    double U = uniform01();
    return int(log(U) / log(1 - p));
}

//
// generate a sample from the standard normal distribution (mean=0, var=1).
// see book "Simulation Modeling and Analysis" by Law & Kelton, pag. 490-491.
//

double Random::normal01() {
    double U1, U2, V1, V2, W, Y, X1, X2;

    do {
        U1 = uniform01();
        U2 = uniform01();
        V1 = 2.0 * U1 - 1.0;
        V2 = 2.0 * U2 - 1.0;
        W = pow(V1, 2.0) + pow(V2, 2.0);
    } while (W > 1);

    Y = sqrt((-2.0 * log(W)) / W);
    X1 = V1 * Y;
    X2 = V2 * Y;
    return X1;
}

//
// generate a sample from a normal distribution with 
// mean 'mean' and variance 'variance'.
//

double Random::normal(double mean, double variance) {
    double X = normal01();
    double stddev = sqrt(variance);
    return mean + stddev * X;
}


//
// generate a sample from a normal distribution with 
// mean 'mean' and variance 'standard deviation'.
//
double Random::normal_stddev(double mean, double stddev) {
    double X = normal01();
    return mean + stddev * X;
}

//
// generate a sample from a lognormal distribution with 
// mean 'mean' zero and standard deviation tau.
//
double Random::lognormal(double tau){
    return exp(tau * normal01());  
}