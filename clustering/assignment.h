#ifndef INC_2_ASSIGNMENT_H
#define INC_2_ASSIGNMENT_H

#include <vector>
#include "dVector.h"
#include "parameter_values.h"
#include "Cluster.h"
#include <iostream>
#include <random>
#include <vector>
#include <tuple>
#include <list>

bool kmeans_assignment (vector<dVector*>& dataVector, Cluster clusters[], int metric, int k);
void set_unassigned_vector (dVector* vec, Cluster clusters[], int metric, int k);
void init_range_assignment (vector<dVector*>& dataVector, int oldClusters[]);
bool assign_all_vectors(vector<dVector*> dataVector, Cluster clusters[], int metric, int k, const int oldClusters[]);
double min_distance_between_centers (Cluster clusters[], int k, int metric);


#endif //INC_2_ASSIGNMENT_H
