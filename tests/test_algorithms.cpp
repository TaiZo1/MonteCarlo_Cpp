#include "mcmc.hpp"
#include "monte_carlo.hpp"
#include "simulated_annealing.hpp"
#include "tsp.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

template <class F>
void rejects(F f) {
    try { f(); }
    catch (const std::invalid_argument&) { return; }
    throw std::runtime_error("Expected invalid_argument");
}

template <class Proposal>
void check_proposal(const Proposal& p) {
    std::mt19937_64 rng(123);
    std::vector<int> x{0, 1, 2, 3};
    do {
        double total = 0.0;
        std::vector<int> y{0, 1, 2, 3};
        do {
            const double probability = p.prob(x, y);
            require(probability >= 0.0, "Negative transition probability");
            require(std::abs(probability - p.prob(y, x)) < 1e-12,
                    "Proposal must be symmetric");
            total += probability;
        } while (std::next_permutation(y.begin(), y.end()));
        require(std::abs(total - 1.0) < 1e-12, "Probabilities must sum to one");
        for (int i = 0; i < 100; ++i) {
            y = p.sample(x, rng);
            require(std::is_permutation(x.begin(), x.end(), y.begin(), y.end()),
                    "Proposal must preserve the tour");
            require(p.prob(x, y) > 0.0, "Sample must have positive probability");
        }
    } while (std::next_permutation(x.begin(), x.end()));
    for (const auto& small : {std::vector<int>{}, std::vector<int>{0}}) {
        require(p.sample(small, rng) == small, "Small states must be preserved");
        require(p.prob(small, small) == 1.0, "Small states have a self transition");
    }
}

int main() {
    try {
        std::mt19937_64 rng(42);
        auto sequence = [i = 0](auto&) mutable { return ++i; };
        auto identity = [](double x) { return x; };
        MeanVar stats;
        MonteCarlo(stats, sequence, identity, rng, 4);
        require(std::abs(stats.getMean() - 2.5) < 1e-12, "Incorrect MC mean");
        require(std::abs(stats.getVar() - 1.25) < 1e-12, "Incorrect population variance");
        auto constant = [](auto&) { return 3.0; };
        MonteCarlo(stats, constant, identity, rng, 5);
        require(stats.getMean() == 3.0 && stats.getVar() == 0.0, "MC must reset output");
        rejects([&] { MonteCarlo(stats, constant, identity, rng, 0); });
        require(MeanVar(2.0).getVar() == 0.0, "Constant has zero variance");

        check_proposal(SwapProposal{});
        check_proposal(SegSwapProposal{});
        require(SwapProposal{}.prob({0, 1}, {2, 3}) == 0.0, "Invalid swap accepted");

        const std::vector<Point> square{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
        const std::vector<int> tour{0, 1, 2, 3};
        auto energy = [&](const auto& x) { return path_length(x, square); };
        require(energy(tour) == 4.0, "Closed square perimeter must be four");
        require(path_length({}, square) == 0.0, "Empty tour must be safe");
        require(path_length({0}, square) == 0.0, "Single city tour has zero length");
        RandomPermutation random{tour};
        auto shuffled = random(rng);
        require(std::is_permutation(tour.begin(), tour.end(), shuffled.begin()),
                "Random sampling must preserve cities");

        auto target = [](const auto& x) { return x.front() == 0 ? 3.0 : 1.0; };
        MCMCsampler sampler(target, SwapProposal{}, std::vector<int>{0, 1}, 10);
        std::vector<int> state{0, 1};
        int visits = 0;
        for (int i = 0; i < 20000; ++i) {
            state = sampler.step(state, rng);
            if (state.front() == 0) ++visits;
        }
        require(std::abs(visits / 20000.0 - 0.75) < 0.03,
                "MCMC does not recover known two-state target");
        MCMCsampler no_steps(target, SwapProposal{}, state, 0);
        require(no_steps(rng) == state, "Zero MCMC steps must preserve initial state");

        auto temperature = [](std::size_t n) { return 0.1 / std::log(n + 1.0); };
        const std::vector<int> crossed{0, 2, 1, 3};
        const auto best = simulated_annealing(crossed, 2000, temperature,
                                              energy, SegSwapProposal{}, rng);
        require(std::is_permutation(tour.begin(), tour.end(), best.begin()),
                "Annealing must preserve the tour");
        require(energy(best) <= energy(crossed), "Best tour must not worsen");
        require(std::abs(energy(best) - 4.0) < 1e-12, "Square optimum not recovered");
        require(simulated_annealing(crossed, 0, temperature, energy,
                                    SwapProposal{}, rng) == crossed,
                "Zero annealing steps must preserve initial state");
        for (double t : {0.0, -1.0, std::numeric_limits<double>::infinity(),
                         std::numeric_limits<double>::quiet_NaN()}) {
            rejects([&] { annealing_step(tour, energy, SwapProposal{}, t, rng); });
        }
        std::cout << "All algorithm checks passed.\n";
    } catch (const std::exception& e) {
        std::cerr << "FAIL: " << e.what() << '\n';
        return 1;
    }
}
