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
tuple<int, int> getNearestCluster (const vector<double>& obj, Cluster clusters[], int metric, unsigned int clusters_num);
int readInput(std::list<dVector*>& dataList, const std::string& filename);

#endif //INC_1_DISTANCE_H
