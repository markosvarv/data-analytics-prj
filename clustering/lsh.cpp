#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <stdlib.h>
#include <unordered_map>
#include <list>
#include <limits>
#include <cstddef>
#include <random>
#include <set>
#include <chrono>
#include <tuple>
#include <iterator>
#include <sstream>
#include "parameter_values.h"

#include "clustering_helping.h"
#include "hFunction.h"
#include "dVector.h"
#include "assignment.h"

#include "lsh.h"

using namespace std;

bool add_toHashTables (vector<dVector*>& dataVector, int k, const vector<hFunction> hF[], unordered_map<string, list<dVector*>> umap[], int L, int metric) {
    for (dVector* pValue: dataVector) {
        for (int i = 0; i < L; i++) {
            vector<double> vec = pValue->getVector();
            string hashString = getHashString(vec, k, hF[i], metric);
            if (hashString.empty()) return false;
            umap[i][hashString].push_back(pValue);
        }
    }
    return true;
}

int lsh_range_search (unordered_map<string, list<dVector *>> umap[], const vector<hFunction> hF[], Cluster clusters[], int metric, int k, double range_dist) {
    int total_assignments_num = 0;
    for (int i = 0; i < k; i++) {
        //for every center make range queries
        vector<double> qValue = clusters[i].getCenter();

        int assignments_num = rangeSearch(qValue, i, umap, hF, K_DEFAULT, L_DEFAULT, range_dist, metric);
        //cout << assignments_num << endl;
        if (assignments_num == -1) {
            cout << "ERROR in assignment\n";
            return false;
        }
        total_assignments_num += assignments_num;
    }
    return total_assignments_num;
}

bool lsh_range_assignment (unordered_map<string, list<dVector *>> umap[], const vector<hFunction> hF[], default_random_engine& generator,
                           vector<dVector*>& dataVector, Cluster clusters[], int metric, int k) {

    int oldClusters[dataVector.size()];
    init_range_assignment(dataVector, oldClusters);

    double min_dist = min_distance_between_centers(clusters, k, metric);
    double range_dist = min_dist/2;

    //cout << "range_dist = " << range_dist << endl;

    long total_assignments_num;
    do {
        total_assignments_num=lsh_range_search(umap, hF, clusters, metric, k, range_dist);
        range_dist *= 2;
        //cout << "TOTAL:: " << total_assignments_num << endl;
    }while (total_assignments_num!=0 || range_dist<=2*min_dist);

    //int count = 0;
    return assign_all_vectors(dataVector, clusters, metric, k, oldClusters);
}


int rangeSearch (vector<double>& q, int cluster_num, unordered_map<string,list<dVector*>> umap[], const vector<hFunction> hF[], int k, int L, double R, int metric) {
    int assignments_num = 0;
    for (int i = 0; i < L; i++) {
        string queryHashString = getHashString(q, k, hF[i], metric);
        //cout << "queryHashString = " << queryHashString << endl;
        if (queryHashString.empty()) {
            cout << "ERROR in rangesearch\n";
            return -1;
        }


        for (dVector *pVector : umap[i][queryHashString]) {
            //if vector is assigned go to next vector
            //if (pVector->is_assigned()) continue;
            vector<double> p = pVector->getVector();

            //if (pVector->is_assigned() && pVector->getSecond_best_cluster()!=-1) continue;
            double t = distance(p, q, metric);
            
            if (t < R*C_VALUE) {
                if (t <= pVector->get_cluster_dist()) {
                    pVector->set_assigned(true);
                    if (t < pVector->get_cluster_dist()) assignments_num++;
                    pVector->setSecond_best_cluster(pVector->getCluster_num());
                    pVector->set_second_cluster_dist(pVector->get_cluster_dist());
                    pVector->setCluster_num(cluster_num);
                    pVector->set_cluster_dist(t);
                    //b=pVector;
                } else if (t < pVector->get_second_cluster_dist()) {
                    pVector->setSecond_best_cluster(cluster_num);
                    pVector->set_second_cluster_dist(pVector->get_cluster_dist());
                }
            }
        }
    }
    return assignments_num;
}

string getHashString (vector<double>& pVec, int k, const vector<hFunction>& h, int metric) {
    vector<double> gValue(k);

    for (int i=0; i<k; i++) {
        gValue[i] = h[i].hValue(pVec, metric);
    }

    stringstream result;
    copy(gValue.begin(), gValue.end(), ostream_iterator<int>(result, ""));
    string r = result.str();
    return r;
}
