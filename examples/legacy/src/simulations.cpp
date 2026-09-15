#include "monte_carlo.hpp"
#include "square_in_disk.hpp"
#include "chi2_distribution.hpp"
#include "markov2states.hpp"
#include "ising1d.hpp"

#include <ctime>
#include <fstream>

double fct(double x){
    return 4.0*x;
}

double lnfct(double x){
    return log(1+ x*x);
}

double fct2(double x, double y){
    return log(1+ x*y)*exp(-x);
}

int main() {
    std::mt19937 G(time(nullptr));

    //Approximation of pi
    SquareInDisk A;

    DoubleMeanVar stats;

    long unsigned n = 100000;

    MonteCarlo(stats, A, fct, G, n);

    std::cout << "Approximation of pi: " << stats.getMean() << std::endl;

    double Lbound = stats.getMean() - 1.96*sqrt(stats.getVar()/n);
    double Rbound = stats.getMean() + 1.96*sqrt(stats.getVar()/n);

    std::cout << "Confidence interval: [ " << Lbound << " ; " << Rbound << " ]" << std::endl;

    //Approximation of ln(1+x^2) between 0 and 1 

    DoubleMeanVar integral_1;

    std::uniform_real_distribution<double> U(0.0,1.0);

    MonteCarlo(integral_1, U, lnfct, G, n);

    std::cout << "Approximation of integral_1 : " << integral_1.getMean() << std::endl;

    //Approximation of ln(1+x^y)*exp(-x) with x in R+ and y between 0 and 1

    DoubleMeanVar integral_2;

    std::exponential_distribution<double> X(1.0);
    std::uniform_real_distribution<double> Y(0.0,1.0);

    auto CoupleXY = [&](std::mt19937& G){return std::make_pair(X(G),Y(G));};

    auto Function_to_evaluate = [](std::pair<double,double> p){return fct2(p.first,p.second);};

    MonteCarlo(integral_2, CoupleXY, Function_to_evaluate, G , n);

    std::cout << "Approximation of integral_2 : " << integral_2.getMean() << std::endl;

    // Compute an empirical histogram of the standard normal distribution on [-3, 3] 

    std::normal_distribution<double> normal_dist(0.0, 1.0);

    Histogram histo(-3.0, 3.0, 50);

    auto id = [](double x) { return x; }; 

    MonteCarlo(histo, normal_dist, id, G, n);
    histo /= n;

    std::ofstream file("histogram.dat");
    file << histo;
    file.close();

    std::cout << "Histogram exported to histogram.dat" << std::endl;

    // plot "histogramme.dat" using 1:2 with boxes

    // Build histograms for chi-squared distributions with 3 and 6 degrees of freedom on [0, 10].

    Chi2_distribution<double, 3> chi3;
    Chi2_distribution<double, 6> chi6;

    Histogram histo3(0.0, 10.0, 50);
    Histogram histo6(0.0, 10.0, 50);

    MonteCarlo(histo3, chi3, id, G, n);
    histo3 /= n;

    MonteCarlo(histo6, chi6, id, G, n);
    histo6 /= n;

    std::ofstream file3("histogramme_chi3.dat");
    file3 << histo3;
    file3.close();

    std::ofstream file6("histogramme_chi6.dat");
    file6 << histo6;
    file6.close();

    std::cout << "Histogram for chi2(3) exported to histogramme_chi3.dat" << std::endl;
    std::cout << "Histogram for chi2(6) exported to histogramme_chi6.dat" << std::endl;

    // Use MonteCarlo with Markov2states and Stat2states to verify the ergodic theorem.

    Markov2states M(1, 0.3, 0.7); 
    Stat2states stats1;

    MonteCarlo(stats1, M, id, G, n);

    stats1.print_results(n);

    // Estimate E[x500] for N = 1000 Ising1D model using Monte Carlo

    int N = 1000;
    double B = 0.5;
    double h = 0.1;
    Ising1D model(N, B, h);

    DoubleMeanVar statsIsing;

    MonteCarlo(statsIsing, model, id, G, n);

    std::cout << "Approximation of E[x500]: " << statsIsing.getMean() << std::endl;

    return 0;
}
