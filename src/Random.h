/* 
 * File:                Random.hpp
 * Author:              Fernando Lobo (fernando.lobo@gmail.com)
 * 
 * Description:         header file of random number generator.
 *                      this class is taken from Prof. Fernando Lobo's framework
 *
 * Created on:          1996
 * Last modified:       April 8, 2011, 2:29 PM
 */

#ifndef UNSET
#define UNSET -1
#endif //UNSET

#ifndef RANDOM_HPP
#define	RANDOM_HPP

class Random {
private:
    long M; // modulus of PMMLCG (the default is 2147483647 = 2^31 - 1)
    long A; // the default is 16807
    long Q; // M div A
    long R; // M mod A
    long Seed; // a number between 1 and m-1

public:

    Random(double m = 2147483647, long a = 16807) {
        M = m;
        A = a;
        Q = M / A;
        R = M % A;
    };

    ~Random() {
    };
    void randomize(long seed = UNSET);
    void randomize(double seed);
    double uniform01();
    double uniform(double a, double b);
    long uniform(long a, long b);
    double exponential(double mu);
    bool bernoulli(double p);

    bool flip(double p) {
        return bernoulli(p);
    }
    long geometric(double p);
    double normal01();
    double normal(double mean, double variance);
    double normal_stddev(double mean, double stddev);
    double lognormal(double tau);
};

#endif	/* RANDOM_HPP */


