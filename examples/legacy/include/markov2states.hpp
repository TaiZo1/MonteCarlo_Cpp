#pragma once

#include <random>
#include <iostream>

class Markov2states {
    protected:
        int x; //Current state
        std::bernoulli_distribution Ua; // Probability to switch from 1 -> 2 with probability a
        std::bernoulli_distribution Ub; // Probability to switch from 2 -> 1 with probability b
    
    public:
        Markov2states(int x0=1, double a0=0.5, double b0=0.5): x(x0), Ua(a0), Ub(b0) {}

        template <class RNG>
        int operator()(RNG& G) {
        if (x == 1) {
            if (Ua(G)) {
                x = 2;
            } 
            else {
                x = 1;
            }
        } 
        else { 
            if (Ub(G)) {
                x = 1;
            } else {
                x = 2;
            }
        }
        return x;
        }
};

class Stat2states {
protected:
    unsigned long visit1;
    unsigned long visit2;

public:
    Stat2states() : visit1(0), visit2(0) {}

    void operator+=(int state) {
        if (state == 1) {
            visit1++;
        } else if (state == 2) {
            visit2++;
        }
    }

    void operator/=(unsigned long n) {
        visit1 /= n;
        visit2 /= n;
    }

    void print_results(unsigned long n) const {
        std::cout << "Proportion of visits to state 1: " << (double)visit1 / n << std::endl;
        std::cout << "Proportion of visits to state 2: " << (double)visit2 / n << std::endl;
    }
};
