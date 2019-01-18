#ifndef INC_1_SEARCH_CUBE_H
#define INC_1_SEARCH_CUBE_H

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

using namespace std;

void add_toCube (list<dVector*>& dataList, int k, vector<hFunction>& hF, list<dVector*> cube[], int metric);
int readQuery(const string& in, const string& out, int k, vector<hFunction>& hF, list<dVector*> cube[], int cubeSize, int probes_num, int metric, int M, list<dVector*>& dataList);
void rangeSearch (set<string>& rNN, dVector& query, list<dVector*> cube[], int cubeSize, vector<hFunction>& hF, int k, double R, int probes_num, int metric, int M);
tuple<string, double, double> approximateNN (dVector& query, list<dVector*> cube[], int cubeSize, vector<hFunction>& hF, int k, int probes_num, int metric, int M);
int getHammingDst (int a, int b);
int getCubeProbe (dVector& pValue, int k, std::vector<hFunction>& h, int metric);


#endif //INC_1_SEARCH_CUBE_H
