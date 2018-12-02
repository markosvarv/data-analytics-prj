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

double euclideanDistance (const vector<double>& v1, const vector<double>& v2) {
    double diff_square_sum = 0.0;
    for (unsigned int i=0; i<v1.size(); i++) {
        diff_square_sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }
    return sqrt(diff_square_sum);
}

double cosineDistance (const vector<double>& v1, const vector<double>& v2) {
    return (1-(inner_product(v1.begin(), v1.end(), v2.begin(), 0.0) / (magnitude(v1) * magnitude(v2))));
}

double magnitude (const vector<double>& v) {
    double sum = 0.0;
    for (auto e : v) sum+=e*e;
    return sqrt(sum);
}

double distance (const vector<double>& v1, const vector<double>& v2, int metric) {
    unsigned long v1_size, v2_size;
    v1_size=v1.size(); v2_size=v2.size();

    if (v1_size!=v2_size) {
        cerr << "Error in distance. Vectors must have the same size" << endl;
        //cout << "v1 size = " << v1_size << " v2 size = " << v2_size << endl;
        return -1.0;
    }
    switch (metric) {
        case EUCLIDEAN:
            return euclideanDistance(v1, v2);
        case COSINE:
            return cosineDistance(v1, v2);
        default:
            return -1.0;
    }
}

//return the cluster id with the nearest center from the object obj
tuple<int, int, double> getNearestCluster (const vector<double>& obj, Cluster clusters[], int metric, unsigned int clusters_num) {
    //auto begin = chrono::high_resolution_clock::now();

    unsigned int cmin = clusters_num;
    unsigned int second_cmin = clusters_num;
    //dVector* b= nullptr;
    double dmin = numeric_limits<double>::max();
    double second_dmin = numeric_limits<double>::max();

    for (unsigned int i=0; i<clusters_num; i++) {
        vector<double> p = clusters[i].getCenter();

        double t = distance(obj, p, metric);
        //cout << "t = " << t << endl;
        if (t<=dmin) {
            second_cmin = cmin;
            second_dmin = dmin;
            cmin = i;
            dmin = t;
            //b=pVector;
        }else if (t<second_dmin) {
            second_cmin = i;
            second_dmin = dmin;
        }
    }
    //cout << "min distance = " << dmin;
    //auto end = chrono::high_resolution_clock::now();
    //auto dur = end - begin;
    //double ms = (chrono::duration_cast<std::chrono::nanoseconds>(dur).count())/1000000.0;
    //if (b) return make_tuple(b->getName(), dmin, ms);
    //else return make_tuple("", 0, 0);

    return make_tuple(cmin, second_cmin, dmin);
}

