# Monte Carlo, MCMC & Simulated Annealing in C++

A compact C++20 project exploring the progression from **random sampling** to
**biased sampling** to **optimization**, using the travelling salesperson problem
(TSP). The implementation uses only the C++ standard library.

## Approach

The example generates 30 cities in the unit square. A state is a permutation of
city indices, and its energy `H(x)` is the Euclidean length of the closed tour,
including the return to the first city.

1. **Random Monte Carlo:** average the lengths of 10,000 uniformly shuffled tours
   to establish a baseline.
2. **MCMC:** use Metropolis-Hastings with target weight
   `mu(x) = exp(-H(x) / T)` at fixed `T = 0.1`. Accept a proposal `y` with probability
   `min(1, mu(y) q(y,x) / (mu(x) q(x,y)))`, favouring shorter tours while allowing
   uphill moves. Report the mean endpoint length of 1,000 chains, each run for
   1,000 steps **from the same initial tour**.
3. **Simulated annealing:** decrease the temperature with
   `T(n) = 0.1 / log(n + 1)` for steps 1 through 100,000. Accept downhill moves and
   accept uphill moves with probability `exp(-(H(y) - H(x)) / T(n))`.
   Return the best tour visited, including the initial tour.

Two symmetric proposals are provided: `SwapProposal` exchanges two positions
(possibly the same position), while `SegSwapProposal` reverses a segment between
two distinct positions. The example uses segment reversal for both MCMC and
annealing. Algorithms are templates over states, objectives, proposals and random
number generators.

## Build and run

Requirement: a C++20 compiler. Run from the repository root.

**Windows / PowerShell** (GCC, e.g. MSYS2 UCRT64, available on `PATH`):

```powershell
New-Item -ItemType Directory -Force build | Out-Null
g++ -std=c++20 -O2 -Wall -Wextra -Wpedantic -Iinclude src/main.cpp -o build/tsp_demo.exe
.\build\tsp_demo.exe
```

**Linux / macOS** (GCC or replace `g++` with `clang++`):

```sh
mkdir -p build
g++ -std=c++20 -O2 -Wall -Wextra -Wpedantic -Iinclude src/main.cpp -o build/tsp_demo
./build/tsp_demo
```

Example output with GCC 16.2 on Windows and seed `42`:

```text
Random paths mean length: 14.4587
MCMC mean length: 6.61666
Initial path length: 13.5934
Annealing best length: 4.7249
```

The seed and experimental parameters are in `src/main.cpp`. A single generator
is shared across stages. Standard-library implementations may produce different
shuffles and distributions, so exact output is not portable across toolchains.

## Tests

```powershell
g++ -std=c++20 -O2 -Wall -Wextra -Wpedantic -Iinclude tests/test_algorithms.cpp -o build/algorithm_tests.exe
.\build\algorithm_tests.exe
```

On Linux/macOS, omit `.exe` and run `./build/algorithm_tests`.
Tests cover moments, proposal normalization and symmetry, permutation preservation,
a known two-state MCMC target, closed-tour distances, annealing on a square and
invalid inputs. They have no external dependencies.

Alternatively, with CMake 3.15 or later:

```sh
cmake -S . -B build/cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build/cmake --config Release
ctest --test-dir build/cmake -C Release --output-on-failure
```

Run `build/cmake/tsp_demo` for single-configuration generators, or
`build/cmake/Release/tsp_demo.exe` with Visual Studio on Windows.

## Structure

```text
include/
  monte_carlo.hpp           Monte Carlo estimator and empirical moments
  mcmc.hpp                  Metropolis-Hastings and permutation proposals
  simulated_annealing.hpp    Cooling loop and best-state tracking
  tsp.hpp                   Cities, closed-tour length and random permutations
src/main.cpp                Three-stage TSP experiment
tests/test_algorithms.cpp   Algorithm checks
examples/legacy/            Original statistical exercises and their README
CMakeLists.txt             Optional CMake build and test integration
```

## Scope and limitations

This is an educational experiment, not a benchmark or a proof of optimality.
The stages use different computational budgets. Finite MCMC runs may retain
initialization bias; no convergence diagnostic is claimed. Annealing provides
no finite-run guarantee of finding the global optimum.

Permutation proposals assume distinct entries; tours must contain valid city
indices. The generic MCMC sampler expects finite, strictly positive target
weights on visited states and valid proposal probabilities. Direct exponential
weights can underflow for larger energies or lower temperatures; such regimes
would need a log-weight implementation. Annealing requires symmetric proposals
and finite, positive temperatures. `MeanVar` reports the empirical population
variance (divisor `n`), using raw moments; it is not an uncertainty estimate for
correlated MCMC samples. Each `MonteCarlo` call resets its output accumulator.

## Earlier exercises

The repository's original integration, histogram, Markov-chain and Ising exercises
are preserved unchanged in [`examples/legacy`](examples/legacy/README.md), with
their own headers. Their original README describes the historical layout;
enable `-DBUILD_LEGACY_EXAMPLES=ON` in CMake to build the `simulations` executable.
These historical exercises are separate from the tested TSP implementation.

## License

[MIT](LICENSE).
