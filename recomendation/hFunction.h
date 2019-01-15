#ifndef RECOMENDATION_HFUNCTION_H
#define RECOMENDATION_HFUNCTION_H

#include <vector>
#include "dVector.h"
#include "parameter_values.h"
#include <iostream>
#include <random>

class hFunction {
private:
    std::vector<double> v;
    //double t;
    //const int w=W_VALUE;

public:
    hFunction (unsigned long vector_size, std::default_random_engine& generator);
    double hValue (const std::vector<double>& p, int metric);
    static std::vector<hFunction> init_hFunctions (unsigned long vector_size, int k, std::default_random_engine& generator);
};


#endif //RECOMENDATION_HFUNCTION_H
