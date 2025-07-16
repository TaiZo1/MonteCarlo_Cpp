#pragma once
#include <vector>
#include <random>

class Ising1D {
protected:
    int N;
    double B;
    double h;
    std::vector<int> E; // spin configuration: +1 or -1
    std::bernoulli_distribution Bern;
    std::uniform_int_distribution<int> U;

public:
    Ising1D(int N0, double B0, double h0)
        : N(N0), B(B0), h(h0), E(N0, 1), U(0, N0 - 1) {}

    template <typename RNG>
    int operator()(RNG& G) {
        int k = U(G);
        int x = E[k];

        double p = 0.0;

        if (k == 0 || k == N - 1) {
            p = std::min(1.0, std::exp(-2 * B * x - 2 * h * x));
        } else {
            int x_left = E[k - 1];
            int x_right = E[k + 1];
            p = std::min(1.0, std::exp(-2 * B * (x_left + x_right) * x - 2 * h * x));
        }

        std::bernoulli_distribution flip(p);
        if (flip(G)) {
            E[k] = -E[k];
        }

        return E[N / 2]; 
    }
};
