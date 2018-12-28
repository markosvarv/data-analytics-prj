#ifndef INC_1_DISTANCE_H
#define INC_1_DISTANCE_H

#include <vector>
#include "dVector.h"
#include "parameter_values.h"
#include "Cluster.h"
#include <iostream>
#include <random>
#include <vector>
#include <tuple>
#include <list>

double euclideanDistance (const std::vector<double>& v1, const std::vector<double> &v2);
double cosineDistance (const std::vector<double>& v1, const std::vector<double>& v2);
double magnitude (const std::vector<double>& v);
double distance (const std::vector<double>& v1, const std::vector<double>& v2, int metric);
tuple<int, int, double> getNearestCluster (const vector<double>& obj, Cluster clusters[], int metric, unsigned int clusters_num);
int readInput(list<dVector*>& dataList, const string& filename);
int readConfig(const string& filename, unsigned int& clusters, unsigned int& hash_functions, unsigned int& hash_tables);
unsigned int readMetric (const string& metric);
void print_results (const string& out, Cluster clusters[], vector<dVector*>& dataVector, int clusters_num, long sec, int init, int assignment, int update, int metric, bool complete);
void clustering_algorithms (int init, int assignment, int update, int k, int metric, vector<dVector*>& dataVector, const string& out, bool complete);
double objective_function (vector<dVector*>& dataVector, Cluster clusters[], int metric);


#endif //INC_1_DISTANCE_H
