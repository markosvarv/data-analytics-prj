#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <assert.h>
#include "dVector.h"

#ifndef RECOMENDATION_CLUSTER_H
#define RECOMENDATION_CLUSTER_H

using namespace std;

class Cluster {
private:
    int center_num;
    vector<double> center;
    set<int> vectors;
public:
    void setCenter(const vector<double> &new_center);
    vector<double>& getCenter();

    bool addObjToCluster (int obj);

    void updateCenter (const vector<dVector*>& dataVector);

    void addVectors (vector<double>& a, const vector<double>& b);

    void eraseVector (int vector_num);

    int getItemsNum (void) {
        return vectors.size();
    }

    set<int> getVectors (void);

    double silhouette_sum (vector<dVector*>& dataVector, Cluster clusters[], int cluster_num, int metric);

    double vectorDistanceSum (const vector<dVector*>& dataVector, int vector_num, int metric);

    void updatePAM_Lloyds(const vector<dVector*>& dataVector, int metric);

};

#endif //RECOMENDATION_CLUSTER_H
