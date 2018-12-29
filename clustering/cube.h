#ifndef INC_2_CUBE_H
#define INC_2_CUBE_H

#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <stdlib.h>
#include <unordered_map>
#include <list>
#include <limits>
#include <cstddef>
#include <random>
#include <set>
#include <chrono>
#include <tuple>
#include <bitset>

#include "parameter_values.h"

#include "hFunction.h"
#include "dVector.h"
#include "Cluster.h"

using namespace std;

void add_toCube (vector<dVector*>& dataList, int k, vector<hFunction>& hF, list<dVector*> cube[], int metric);
int readQuery(const string& in, const string& out, int k, const vector<hFunction>& hF, list<dVector*> cube[], int cubeSize, int probes_num, int metric, int M, list<dVector*>& dataList);
int cube_range_search (vector<double>& q, int cluster_num, list<dVector*> cube[], int cubeSize, vector<hFunction>& hF, int k, double R, int probes_num, int metric, int M);
//tuple<string, double, double> approximateNN (dVector& query, list<dVector*> cube[], int cubeSize, vector<hFunction>& hF, int k, int probes_num, int metric, int M);
int getHammingDst (int a, int b);
int getCubeProbe (vector<double>& q, int k, vector<hFunction>& h, int metric);
bool cube_range_assignment (list<dVector*> cube[], int cubeSize, vector<hFunction>& hF, default_random_engine& generator,
                            vector<dVector*>& dataVector, Cluster clusters[], int metric, int k);
int cube_range_query(dVector* pVector, int cluster_num, int& assignments_num, vector<double>& q, int countM, double R, int metric, int M);
int cube_range_queries (list<dVector*> cube[], int cubeSize, vector<hFunction>& hF, Cluster clusters[], int metric, int k, double range_dist);




#endif //INC_2_CUBE_H
