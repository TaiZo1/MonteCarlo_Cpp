#pragma once

#include <vector>
#include <iostream>


template <class Statistique, class RandomVariable, class Measurement, class RNG>
void MonteCarlo(Statistique & res, RandomVariable & X, const Measurement & f, RNG & G, long unsigned n){
    for(unsigned long i = 0; i<n; i++){
        res += f(X(G));
    }
    res/=n;
}


class DoubleMeanVar{
    protected:
        double m; //Mean
        double v; //useful for variance
    public:
        DoubleMeanVar(double x=0.0) : m(x), v(0) {}

        void operator+=(double x){
            m += x;
            v += x*x;
        }

        void operator/=(long unsigned n){
            m /= n;
            v /= n;
        }

        double getMean() const{
            return m;
        }

        double getVar() const{
            return v - m*m;
        }
};

class Histogram{
    protected:
        std::vector<double> sample;
        unsigned int nb_boxes; 
        double lbound;
        double ubound;
        double box_width;

    public:
        Histogram(double min_intervalle, double max_intervalle, unsigned int n): 
        sample(n,0), nb_boxes(n), lbound(min_intervalle), ubound(max_intervalle),
        box_width((max_intervalle-min_intervalle)/n){}

        void operator+=(double x) {
            for (unsigned int i = 0; i < nb_boxes; i++) {
                double a_i = lbound + i * box_width;
                double a_ip1 = a_i + box_width;

                if (x >= a_i && x < a_ip1) {
                    sample[i] += 1.0;
                    break;
                }
            }
            if (x == ubound) {
                sample[nb_boxes - 1] += 1.0;
            }
        }

        void operator/=(unsigned long n) {
            for (auto& val : sample) {
                val /= n;
            }
        }

        friend std::ostream& operator<<(std::ostream& o, const Histogram& H) {
            for (unsigned int i = 0; i < H.nb_boxes; i++) {
                double a_i = H.lbound + i * H.box_width;
                o << a_i << " " << H.sample[i] << std::endl;
            }
            return o;
        }
};
