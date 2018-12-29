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

//returns the the min distance of dvec_num point to some centroid
//returns 0 if dvec_num point is a center
double di (int i, set<int>& centers, vector<dVector*>& dataVector, int metric) {
    vector<double> current_vec = dataVector[i]->getVector();
    double di = numeric_limits<double>::max();

    for (auto center_num : centers) {
        vector<double> center = dataVector[center_num]->getVector();

        double t = distance(current_vec, center, metric);
        if (t<=di) {
            //cmin = i;
            di = t;
        }
    }
    return di;

}

double pr (int r, set<int>& centers, vector<dVector*>& dataVector, int metric) {
    double sum=0;
    for (int i=0; i<=r; i++) {
        if (centers.find(i) != centers.end()) continue; //if dvec is a centroid go to next dvec
        double current_di=di(i, centers, dataVector, metric);
        sum+=current_di*current_di;
    }
    return sum;
}

int kmeanspp_init (set<int>& centers, vector<dVector*>& dataVector, int metric, unsigned int k) {
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    int data_size = (int)dataVector.size();
    uniform_int_distribution<> center_dis(0, data_size-1);
    centers.insert(center_dis(gen)); //choose first centroid uniformly at random

    while (centers.size()<k) {
        double max_pr = pr(data_size-1, centers, dataVector, metric);

        uniform_real_distribution<> x_dis(0, max_pr);
        double x = x_dis(gen); //pick a uniformly distributed float at [0,max_pr]

        unsigned int dvec_num;
        double current_pr = 0; //P(0)=0
        //end the for loop when x<=current_pr and keep this dvec_num
        for (dvec_num = 0; current_pr < x && dvec_num < dataVector.size(); dvec_num++) {
            if (centers.find(dvec_num) != centers.end()) continue; //if dvec is a centroid go to next dvec
            double current_di = di(dvec_num, centers, dataVector, metric);
            current_pr += current_di * current_di;
        }
        centers.insert(dvec_num);
    }
    return 1;
}

void random_init (set<int>& centers, int k, vector<dVector*>& dataVector) {
    while ((int)centers.size() < k) {
        long num = rand() % dataVector.size();
        centers.insert(num);
    }
}

