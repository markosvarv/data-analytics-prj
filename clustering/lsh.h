
#ifndef INC_2_LSH_H
#define INC_2_LSH_H


#include <iostream>
#include <unordered_map>
#include <tuple>
#include <list>

#include "parameter_values.h"

#include "hFunction.h"
#include "dVector.h"
#include "Cluster.h"

using namespace std;

//tuple<string, double, double> approximateNN (dVector& query, unordered_map<string, list<dVector*>> umap[], int L, int k, vector<hFunction> hF[], int metric);
string getHashString (vector<double>& pVec, int k, const vector<hFunction>& h, int metric);
int readQuery(list<dVector*>& dataList, const string& in, const string& out, int k, vector<hFunction> hF[], unordered_map<string, list<dVector*>> umap[], int L, int metric);
bool add_toHashTables (vector<dVector*>& dataVector, int k, const vector<hFunction> hF[], unordered_map<string, list<dVector*>> umap[], int L, int metric);
int rangeSearch (vector<double>& dVec, int cluster_num, unordered_map<string, list<dVector*>> umap[], const vector<hFunction> hF[], int k, int L, double R, int metric);
int lsh_range_search (unordered_map<string, list<dVector *>> umap[], const vector<hFunction> hF[], Cluster clusters[], int metric, int k, double range_dist);
bool lsh_range_assignment (unordered_map<string, list<dVector *>> umap[], const vector<hFunction> hF[], default_random_engine& generator,vector<dVector*>& dataVector, Cluster clusters[], int metric, int k);




#endif //INC_2_LSH_H
