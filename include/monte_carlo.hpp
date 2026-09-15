#pragma once

#include <cstddef>
#include <cmath>
#include <stdexcept>


class MeanVar {
private:
    double m;
    double v;

public:
    MeanVar(double x = 0.0)
        : m(x), v(x * x)
    {}

    MeanVar& operator+=(double x) {
        m += x;
        v += x * x;
        return *this;
    }

    MeanVar& operator/=(std::size_t n) {
        if (n == 0) {
            throw std::invalid_argument("Sample count must be positive");
        }
        m /= n;
        v /= n;
        return *this;
    }

    double getMean() const {
        return m;
    }

    double getVar() const {
        return v - m * m;
    }
};


template <class RandomVariable,
          class Function,
          class RNG>
void MonteCarlo(
    MeanVar& res,
    RandomVariable& X,
    const Function& f,
    RNG& G,
    std::size_t n
) {
    if (n == 0) {
        throw std::invalid_argument("Sample count must be positive");
    }
    res = MeanVar{};
    for (std::size_t i = 0; i < n; ++i) {
        res += f(X(G));
    }

    res /= n;
}
