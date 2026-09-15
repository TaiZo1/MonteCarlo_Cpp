#pragma once

#include <random>
#include <utility>

class SquareInDisk {
    private :
        std::uniform_real_distribution<double> U;
    public :
        SquareInDisk() : U(0.0,1.0) {}

        template <class RNG>
        double operator()(RNG & G){
            double x = U(G);
            double y = U(G);
            if(x*x + y*y <= 1.0){
                return 1.0;
            }
            else{
                return 0.0;
            }
        }
};
