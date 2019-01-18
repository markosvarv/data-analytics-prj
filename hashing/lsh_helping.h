#ifndef INC_1_SEARCH_H
#define INC_1_SEARCH_H

#include <iostream>
#include <unordered_map>
#include <tuple>
#include <list>

#include "parameter_values.h"

#include "hFunction.h"
#include "dVector.h"

using namespace std;

tuple<string, double, double> approximateNN (dVector& query, unordered_map<string, list<dVector*>> umap[], int L, int k, vector<hFunction> hF[], int metric);
string getHashString (dVector& pValue, int k, std::vector<hFunction>& h, int metric);
int readQuery(list<dVector*>& dataList, const string& in, const string& out, int k, vector<hFunction> hF[], unordered_map<string, list<dVector*>> umap[], int L, int metric);
void add_toHashTable (list<dVector*>& dataList, int k, vector<hFunction> hF[], unordered_map<string, list<dVector*>> umap[], int L, int metric);
void rangeSearch (set<string>& rNN, dVector& query, unordered_map<string, list<dVector*>> umap[], vector<hFunction> hF[], int k, int L, double R, int metric);



#endif //INC_1_SEARCH_H
