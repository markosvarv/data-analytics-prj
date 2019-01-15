#ifndef RECOMENDATION_LSH_H
#define RECOMENDATION_LSH_H

#include <iostream>
#include <unordered_map>
#include <tuple>
#include <list>

#include "parameter_values.h"

#include "hFunction.h"
#include "dVector.h"

using namespace std;

string getHashString (const dVector* pValue, int k, std::vector<hFunction>& h, int metric);
void add_toHashTable (list<dVector*>& dataList, int k, vector<hFunction> hF[], unordered_map<string, list<dVector*>> umap[], int L, int metric);
void rangeSearch (set<dVector*>& rNN, const dVector* query, unordered_map<string, list<dVector*>> umap[], vector<hFunction> hF[], int k, int L, double R, int metric);
void keep_P_Best (const set<dVector*>& vectors, const vector<double>& given_vec, int k, const dVector* best[], int metric);


#endif //RECOMENDATION_LSH_H
