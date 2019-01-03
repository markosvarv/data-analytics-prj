#ifndef RECOMENDATION_CLUSTER_H
#define RECOMENDATION_CLUSTER_H


#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <assert.h>
#include "dVector.h"


using namespace std;

class Cluster {
private:
    int center_num;
    vector<double> center;
    set<dVector*> vectors;
public:
    void setCenter(const vector<double> &new_center);
    vector<double>& getCenter();

    bool addObjToCluster (dVector* obj);

    void updateCenter (const vector<dVector*>& dataVector);

    void addVectors (vector<double>& a, const vector<double>& b);

    void eraseVector (dVector* vector_num);

    int getItemsNum (void) {
        return vectors.size();
    }

    set<dVector*>& getVectors ();

    double silhouette_sum (vector<dVector*>& dataVector, Cluster clusters[], int cluster_num, int metric);

    double vectorDistanceSum (dVector* vec, int metric);

    void updatePAM_Lloyds(const vector<dVector*>& dataVector, int metric);

};

#endif //RECOMENDATION_CLUSTER_H
