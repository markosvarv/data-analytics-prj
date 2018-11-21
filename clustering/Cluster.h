#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <assert.h>
#include "dVector.h"

#ifndef INC_2_CLUSTER_CLASS_H
#define INC_2_CLUSTER_CLASS_H

using namespace std;

class Cluster {
private:
    vector<double> center;
    set<int> vectors;
public:
    void setCenter(const vector<double> &new_center);
    vector<double>& getCenter();

    bool addObjToCluster (int obj);

    void updateCenter (const vector<dVector*>& dataVector);

    void addVectors (vector<double>& a, const vector<double>& b);

    void eraseVector (int vector_num);

    double vectorAverageDistance (vector<dVector*>& dataVector, int vector_num, int metric);

    };
#endif //INC_2_CLUSTER_CLASS_H
