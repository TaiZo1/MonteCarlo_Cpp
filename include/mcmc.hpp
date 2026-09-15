#pragma once

#include <random>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstddef>


template <class State, class Target, class Proposal>
class MCMCsampler {
private:
    Target mu;
    Proposal P;
    State x0;
    std::size_t n_steps;

public:
    MCMCsampler(Target mu, Proposal P, State x0, std::size_t n_steps)
        : mu(mu), P(P), x0(x0), n_steps(n_steps)
    {}

    template <class RNG>
    State step(const State& x, RNG& G) const {
        State y = P.sample(x, G);

        double rho = std::min(
            (mu(y) * P.prob(y, x)) /
            (mu(x) * P.prob(x, y)),
            1.0
        );

        std::uniform_real_distribution<double> U(0.0, 1.0);

        double u = U(G);

        if (u < rho) {
            return y;
        } else {
            return x;
        }
    }

    template <class RNG>
    State operator()(RNG& G) const {
        State x = x0;

        for (std::size_t i = 0; i < n_steps; ++i) {
            x = step(x, G);
        }

        return x;
    }
};


struct SwapProposal {
    // Proposal probabilities assume states contain distinct entries.

    template <class RNG>
    std::vector<int> sample(std::vector<int> x, RNG& G) const {
        if (x.empty()) {
            return x;
        }

        std::uniform_int_distribution<std::size_t> U(0, x.size() - 1);

        std::size_t i = U(G);
        std::size_t j = U(G);

        std::swap(x[i], x[j]);

        return x;
    }

    double prob(
        const std::vector<int>& x,
        const std::vector<int>& y
    ) const {
        if (x.size() != y.size()) {
            return 0.0;
        }
        if (x.empty()) {
            return 1.0;
        }

        std::size_t count = 0;
        std::size_t first = 0;
        std::size_t second = 0;
        const std::size_t n = x.size();

        for (std::size_t i = 0; i < n; ++i) {
            if (x[i] != y[i]) {
                if (count == 0) first = i;
                if (count == 1) second = i;
                ++count;
            }
        }

        if (count == 0) {
            return 1.0 / n;
        }

        if (count == 2 && x[first] == y[second] && x[second] == y[first]) {
            return 2.0 / (n * n);
        }

        return 0.0;
    }
};


struct SegSwapProposal {
    // Reverse an inclusive segment of a permutation (distinct entries).

    template <class RNG>
    std::vector<int> sample(std::vector<int> x, RNG& G) const {
        if (x.size() < 2) {
            return x;
        }

        std::uniform_int_distribution<std::size_t> U(0, x.size() - 1);

        std::size_t i = U(G);
        std::size_t j = U(G);

        while (j == i) {
            j = U(G);
        }

        std::size_t min_i = std::min(i, j);
        std::size_t max_j = std::max(i, j);

        std::reverse(
            x.begin() + min_i,
            x.begin() + max_j + 1
        );

        return x;
    }

    double prob(
        const std::vector<int>& x,
        const std::vector<int>& y
    ) const {
        if (x.size() != y.size()) {
            return 0.0;
        }
        if (x.size() < 2) {
            return x == y ? 1.0 : 0.0;
        }

        const std::size_t n = x.size();

        auto [first_x, first_y] =
            std::mismatch(x.begin(), x.end(), y.begin());

        if (first_x == x.end()) {
            return 0.0;
        }

        std::size_t i =
            std::distance(x.begin(), first_x);

        auto [last_x, last_y] =
            std::mismatch(x.rbegin(), x.rend(), y.rbegin());

        std::size_t j =
            n - 1 - std::distance(x.rbegin(), last_x);

        for (std::size_t k = i; k <= j; ++k) {
            if (y[k] != x[i + j - k]) {
                return 0.0;
            }
        }

        return 2.0 / (n * (n - 1));
    }
};
