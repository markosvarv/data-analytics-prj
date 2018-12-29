#ifndef INC_1_PVECTOR_H
#define INC_1_PVECTOR_H


#include <vector>
#include "dVector.h"
#include "parameter_values.h"
#include <iostream>
#include <random>
#include <map>

class hFunction {
private:
    std::vector<double> v;
    double t;
    const int w=W_VALUE;
    std::map<int,int> cube_map;

public:
    hFunction (unsigned long vector_size, std::default_random_engine& generator);
    double hValue (const std::vector<double>& p, int metric) const;
    void add_value_to_cubeMap (int hvalue);
    int get_value_from_cubeMap (int hvalue);


    static std::vector<hFunction> init_hFunctions (unsigned long vector_size, int k, std::default_random_engine& generator);
};




#endif //INC_1_PVECTOR_H

