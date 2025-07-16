#pragma once

template <class REAL, int k>
class Chi2_distribution{
    private:
        std::normal_distribution<REAL> N;

    public:
        Chi2_distribution(): N(0.0,1.0){}
        template <class RNG>
        REAL operator()(RNG & G){
            REAL sum = 0.0;
            for(int i=0; i<k; i++){
                REAL x = N(G);
                sum += x*x;
            }
            return sum;
        }
};
