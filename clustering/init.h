#ifndef INC_2_INIT_H
#define INC_2_INIT_H

#include <vector>
#include "dVector.h"
#include "parameter_values.h"
#include "Cluster.h"
#include <iostream>
#include <random>
#include <vector>
#include <tuple>
#include <list>

double di (int i, set<int>& centers, vector<dVector*>& dataVector, int metric);
double pr (int r, set<int>& centers, vector<dVector*>& dataVector, int metric);
int kmeanspp_init (set<int>& centers, vector<dVector*>& dataVector, int metric, unsigned int k);
void random_init (set<int>& centers, int k, vector<dVector*>& dataVector);

#endif //INC_2_INIT_H
