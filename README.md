# MonteCarlo_Cpp
Monte Carlo methods and statistical simulations in C++

## Overview
This project implements several classic Monte Carlo techniques to estimate expectations, integrals, distributions, and equilibrium measures of stochastic models in C++. It showcases the use of templates to build general-purpose simulation tools.

The project includes:

π approximation via uniform random points in the unit square

Monte Carlo integration on [0, 1] and 
𝑅
+
×
[
0
,
1
]

Histogram construction for normal and chi-squared distributions

Markov chains and ergodic theorem demonstration

1D Ising model with Metropolis dynamics

## Features
Generic MonteCarlo template function

Accumulators for mean, variance (DoubleMeanVar)

Empirical histograms via Histogram class

Simulation of χ² distributions

Markov chains with 2 states and empirical visit statistics

1D Ising model observable tracking at position 500

## Project Structure

MonteCarlo_Cpp/

├── include/

│   ├── monte_carlo.hpp           # Generic Monte Carlo and statistical tools

│   ├── square_in_disk.hpp        # Uniform (x, y) ∈ [0, 1]^2 generator

│   ├── chi2_distribution.hpp     # Chi-squared distribution template

│   ├── markov2states.hpp         # 2-state Markov chain and statistics

│   └── ising1d.hpp               # 1D Ising model with Metropolis updates


├── src/

│   └── simulations.cpp           # Main file: all experiments and outputs

├── README.md 

├── .gitignore 


## Sample Outputs
π approximation with confidence interval

Approximated integrals via Monte Carlo

Histograms exported as .dat files for visualization

Proportions of visits in Markov chains

Estimation of 
𝐸
[
𝑥
500
]
​
in Ising1D

