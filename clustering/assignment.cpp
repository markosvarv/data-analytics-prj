#include "dVector.h"
#include <iostream>
#include <random>
#include <iterator>
#include <sstream>
#include <bitset>
#include <chrono>
#include <tuple>
#include <list>
#include <fstream>
#include <algorithm>

#include "Cluster.h"

#include "clustering_helping.h"

using namespace std;

bool kmeans_assignment (vector<dVector*>& dataVector, Cluster clusters[], int metric, int k) {
    bool change = false;
    int obj_num=0;

    for (auto vec : dataVector) {

        int cluster_num=vec->getCluster_num();

        double distance;
        int new_cluster_num, second_best;
        tie(new_cluster_num, second_best, distance) = getNearestCluster(vec->getVector(), clusters, metric, k);

        //cout << distance << endl;

        if (cluster_num!=new_cluster_num) {
            //if cluster has changed erase vector from previous cluster and add it to the new cluster
            change=true;
            if (cluster_num!=-1) clusters[cluster_num].eraseVector(obj_num);
            clusters[new_cluster_num].addObjToCluster(obj_num);
            vec->setCluster_num(new_cluster_num);
        }
        vec->setSecond_best_cluster(second_best);
        obj_num++;
    }
    return change;
}

void set_unassigned_vector (dVector* vec, Cluster clusters[], int metric, int k) {
    double dist;
    int new_cluster_num, second_best;
    tie(new_cluster_num, second_best, dist) = getNearestCluster(vec->getVector(), clusters, metric, k);
    //cout << dist << endl;

    vec->setCluster_num(new_cluster_num);
    vec->setSecond_best_cluster(second_best);
    vec->set_cluster_dist(dist);
}

void init_range_assignment (vector<dVector*>& dataVector, int oldClusters[]) {
    int obj_num=0;
    for (auto vec : dataVector) {
        oldClusters[obj_num] = vec->getCluster_num();
        obj_num++;
        vec->set_assigned(false);
        vec->setCluster_num(-1);
        vec->setSecond_best_cluster(-1);
        vec->set_cluster_dist(numeric_limits<double>::max());
        vec->set_second_cluster_dist(numeric_limits<double>::max());
    }
}

bool assign_all_vectors(vector<dVector*> dataVector, Cluster clusters[], int metric, int k, const int oldClusters[]) {
    int obj_num=0;
    bool change=false;
    int count=0;
    for (auto vec : dataVector) {
        //assign all unassigned vectors
        if (!vec->is_assigned()) {
            //cout << "mphka sto if\n";
            count++;
            set_unassigned_vector(vec, clusters, metric, k);
        }

        //second best cluster is the same cluster, if not assigned
        if (vec->getSecond_best_cluster()==-1) {
            //cout << "mphka sto second best\n";
            vec->setSecond_best_cluster(vec->getCluster_num());
        }

        int new_cluster_num=vec->getCluster_num();
        //cout << "second_cluster_num = " << vec->getSecond_best_cluster() << endl;

        if (oldClusters[obj_num]!=new_cluster_num) {
            //if cluster has changed erase vector from previous cluster and add it to the new cluster
            //cout << "CHANGE!\n";
            change=true;
            if (oldClusters[obj_num]!=-1) clusters[oldClusters[obj_num]].eraseVector(obj_num);
            //if (oldClusters[obj_num]!=-1) clusters[new_cluster_num].eraseVector(obj_num);

            clusters[new_cluster_num].addObjToCluster(obj_num);
        }
        obj_num++;
    }
    //cout << "count = " << count << endl;
    return change;
};

double min_distance_between_centers (Cluster clusters[], int k, int metric) {
    double dmin = numeric_limits<double>::max();
    for (int i=0; i<k; i++) {
        vector<double> vec_i = clusters[i].getCenter();
        for (int j=0; j<k; j++) {
            if (i>=j) continue; //the centers matrix is symmetric
            vector<double> vec_j = clusters[j].getCenter();
            double t = distance (vec_i, vec_j, metric);
            if (t<dmin) dmin=t;
        }
    }
    return dmin;
};