#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>

#include "monte_carlo.hpp"
#include "mcmc.hpp"
#include "simulated_annealing.hpp"

#include "tsp.hpp"

int main() {
    std::mt19937_64 G(42);

    const std::size_t n_cities = 30;

    std::uniform_real_distribution<double> U(0.0, 1.0);

    std::vector<Point> cities(n_cities);

    for (auto& city : cities) {
        city.x = U(G);
        city.y = U(G);
    }

    std::vector<int> x0(n_cities);

    for (std::size_t i = 0; i < n_cities; ++i) {
        x0[i] = static_cast<int>(i);
    }

    auto H = [&](const std::vector<int>& path) {
        return path_length(path, cities);
    };

    RandomPermutation random_path{x0};

    MeanVar mc_result(0.0);

    MonteCarlo(
        mc_result,
        random_path,
        H,
        G,
        10000
    );

    std::cout << "Random paths mean length: "
              << mc_result.getMean()
              << '\n';

    double temp = 0.1;

    auto mu = [&](const std::vector<int>& path) {
        return std::exp(-H(path) / temp);
    };

    SegSwapProposal proposal;

    MCMCsampler<
        std::vector<int>,
        decltype(mu),
        SegSwapProposal
    > sampler(
        mu,
        proposal,
        x0,
        1000
    );

    MeanVar mcmc_result(0.0);

    MonteCarlo(
        mcmc_result,
        sampler,
        H,
        G,
        1000
    );

    std::cout << "MCMC mean length: "
              << mcmc_result.getMean()
              << '\n';

    auto T = [](std::size_t n) {
        return 0.1 / std::log(static_cast<double>(n) + 1.0);
    };

    std::vector<int> best_path =
        simulated_annealing(
            x0,
            100000,
            T,
            H,
            proposal,
            G
        );

    std::cout << "Initial path length: "
              << H(x0)
              << '\n';

    std::cout << "Annealing best length: "
              << H(best_path)
              << '\n';

    return 0;
}