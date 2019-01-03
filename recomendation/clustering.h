#ifndef RECOMENDATION_CLUSTERING_H
#define RECOMENDATION_CLUSTERING_H

#include <vector>
#include "dVector.h"
#include "parameter_values.h"
#include "Cluster.h"
#include <iostream>
#include <random>
#include <vector>
#include <tuple>
#include <list>


void clustering_algorithms (int k, int metric, vector<dVector*>& dataVector, Cluster clusters[]);
void random_init (set<int>& centers, int k, vector<dVector*>& dataVector);
double silhouette (vector<dVector*>& dataVector, Cluster clusters[], int metric);

double di (int i, set<int>& centers, vector<dVector*>& dataVector, int metric);
double pr (int r, set<int>& centers, vector<dVector*>& dataVector, int metric);
int kmeanspp_init (set<int>& centers, vector<dVector*>& dataVector, int metric, unsigned int k);
double objective_function (vector<dVector*>& dataVector, Cluster clusters[], int metric);
bool kmeans_assignment (vector<dVector*>& dataVector, Cluster clusters[], int metric, int k);

tuple<int, int, double> getNearestCluster (const vector<double>& obj, Cluster clusters[], int metric, unsigned int clusters_num);
#endif //RECOMENDATION_CLUSTERING_H
