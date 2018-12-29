#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <tuple>
#include <bitset>
#include <list>

#include "parameter_values.h"
#include "hFunction.h"
#include "dVector.h"
#include "cube.h"
#include "clustering_helping.h"
#include "assignment.h"

using namespace std;

void add_toCube (vector<dVector*>& dataList, int k, vector<hFunction>& hF, list<dVector*> cube[], int metric) {
    for (dVector* pValue: dataList) {
        vector<double> p = pValue->getVector();
        int probe = getCubeProbe(p, k, hF, metric);
        //cout << "probe = " << probe << endl;
        cube[probe].push_back(pValue);
    }
}

int getHammingDst (int a, int b) {
    bitset<32> bsetA(a), bsetB(b);
    int r = ((int)(bsetA^bsetB).count());
    //cout << "hamming distance = " << r << endl;
    return r;
}

int getCubeProbe (vector<double>& q, int k, vector<hFunction>& h, int metric) {
    vector<int> gValue(k);
    bitset<32> bset;

    int i;
    for (i=0; i<k; i++) {

        gValue[i] = (int)h[i].hValue(q, metric);

        h[i].add_value_to_cubeMap(gValue[i]);
        if (metric==EUCLIDEAN) gValue[i] = abs(h[i].get_value_from_cubeMap(gValue[i]));
        //cout << (int)gValue[i] << " ";
        int bit = gValue[i];
        //cout << bit << " ";
        bset[31-i] = bit;
    }
    //cout << endl << i << endl;
    bset>>=(32-k);
    //cout << bset << endl;

    //cout << endl << endl;
    int value = (int)bset.to_ulong();
    //cout << bset.to_ulong() << endl;
    return value;
}


int cube_range_queries (list<dVector*> cube[], int cubeSize, vector<hFunction>& hF, Cluster clusters[], int metric, int k, double range_dist) {
    int total_assignments_num = 0;
    for (int i = 0; i < k; i++) {
        //for every center make range queries
        vector<double> qValue = clusters[i].getCenter();

        int assignments_num = cube_range_search(qValue, i, cube, cubeSize, hF, K_DEFAULT, range_dist, P_DEFAULT, metric, M_DEFAULT);
        //cout << assignments_num << endl;
        if (assignments_num == -1) {
            cout << "ERROR in assignment\n";
            return false;
        }
        total_assignments_num += assignments_num;
    }
    return total_assignments_num;
}


bool cube_range_assignment (list<dVector*> cube[], int cubeSize, vector<hFunction>& hF, default_random_engine& generator,
                            vector<dVector*>& dataVector, Cluster clusters[], int metric, int k) {

    int oldClusters[dataVector.size()];
    init_range_assignment(dataVector, oldClusters);

    double min_dist = min_distance_between_centers(clusters, k, metric);
    double range_dist = min_dist/2;

    //cout << "range_dist = " << range_dist << endl;

    long total_assignments_num;
    do {
        total_assignments_num = cube_range_queries(cube, cubeSize, hF, clusters, metric, k, range_dist);
        range_dist *= 2;
        cout << "TOTAL:: " << total_assignments_num << endl;
    }while (total_assignments_num!=0 || range_dist<=2*min_dist);

    //int count = 0;
    return assign_all_vectors(dataVector, clusters, metric, k, oldClusters);
}

int cube_range_query(dVector* pVector, int cluster_num, int& assignments_num, vector<double>& q, int countM, double R, int metric, int M) {

    if (countM>=M) return -1;
    vector<double> p = pVector->getVector();
    //if (distance(p, q, metric) < R * C_VALUE) rNN.insert(pVector->getName());

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
    countM++;
    return countM;
}

int cube_range_search (vector<double>& q, int cluster_num, list<dVector*> cube[], int cubeSize, vector<hFunction>& hF, int k, double R, int probes_num, int metric, int M) {
    int assignments_num=0;
    const int probe = getCubeProbe(q, k, hF, metric);

    int countM = 0;
    for (dVector *pVector : cube[probe]) {
        cube_range_query(pVector, cluster_num, assignments_num, q, countM, R, metric, M);
    }


    int hamming_dst=1, current_num=0, probes_count=1;
    //get vectors in adjacent probes
    while (probes_count<=probes_num) {
        int current_probe=-1;

        while(current_num < cubeSize ) {
            if (getHammingDst(current_num, probe)==hamming_dst) {
                current_probe = probe;
                break;
            }
            current_num++;
        }
        if (current_num>=cubeSize) { //no other adjacent probes with hamming_dst
            hamming_dst++;
            current_num=0;
            continue;
        }

        int countM = 0;
        if (cube[current_probe].empty()) {
            current_num++;
            continue;
        }

        for (dVector *pVector : cube[current_probe]) {
            cube_range_query(pVector, cluster_num, assignments_num, q, countM, R, metric, M);
            probes_count++;
        }
    }
    return assignments_num;
}
