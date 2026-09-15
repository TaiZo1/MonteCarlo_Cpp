#pragma once

#include <random>
#include <cmath>
#include <cstddef>
#include <stdexcept>


template <class State, class Function, class Proposal, class RNG>
State annealing_step(
    const State& x,
    const Function& H,
    const Proposal& P,
    double temp,
    RNG& G
) {
    // Metropolis acceptance here requires a symmetric proposal.
    if (!(temp > 0.0) || !std::isfinite(temp)) {
        throw std::invalid_argument("Temperature must be finite and positive");
    }
    State y = P.sample(x, G);

    double delta_H = H(y) - H(x);

    if (delta_H <= 0.0) {
        return y;
    }

    std::uniform_real_distribution<double> U(0.0, 1.0);

    if (U(G) < std::exp(-delta_H / temp)) {
        return y;
    }

    return x;
}


template <
    class State,
    class TemperatureFunction,
    class EnergyFunction,
    class Proposal,
    class RNG
>
State simulated_annealing(
    State x0,
    std::size_t n_steps,
    const TemperatureFunction& T,
    const EnergyFunction& H,
    const Proposal& P,
    RNG& G
) {
    State current_state = x0;
    State best_state = x0;

    for (std::size_t i = 1; i <= n_steps; ++i) {

        double T_n = T(i);

        State x = annealing_step(
            current_state,
            H,
            P,
            T_n,
            G
        );

        if (H(x) < H(best_state)) {
            best_state = x;
        }

        current_state = x;
    }

    return best_state;
}
