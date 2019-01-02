#include <iostream>
#include <vector>
#include <chrono>

#include "clustering.h"
#include "Cluster.h"
#include "recomendation_helping.h"


void clustering_algorithms (int k, int metric, vector<dVector*>& dataVector, Cluster clusters[]) {
    auto begin = chrono::high_resolution_clock::now();

    set<int> centers;

    //initializing

    random_init(centers, k, dataVector);

    //kmeanspp_init(centers, dataVector, metric, k);
    int cluster_num = 0;
    for (auto c : centers) {
        //cout << "center = " << c << endl;
        clusters[cluster_num].setCenter(dataVector[c]->getVector());
        cluster_num++;
    }

    bool change;

    double current_obj = 0, previous_obj;

    do {
        kmeans_assignment(dataVector, clusters, metric, k);

        previous_obj = current_obj;
        current_obj = objective_function(dataVector, clusters, metric);

        double obj_fraction = previous_obj / current_obj;

        cout << obj_fraction << endl;

        if (previous_obj != 0) change = (obj_fraction > 1.01 || obj_fraction <1);
        else change = true;

        //update
        //update only if has been a change in clusters
        if (change) {
            for (int i = 0; i < k; i++) clusters[i].updateCenter(dataVector);
        }
    } while (change);

    auto end = chrono::high_resolution_clock::now();
    auto dur = end - begin;
    long sec = (chrono::duration_cast<std::chrono::seconds>(dur).count());

    //evaluation
    cout << "eimai prin tin silouet\n";
    //print_results(out, clusters, dataVector, k, sec, init, assignment, update, metric, complete);

    //double ev = silhouette(dataVector, clusters, metric);
    //cout << "ev swsto = " << ev << endl;
}

void random_init (set<int>& centers, int k, vector<dVector*>& dataVector) {
    while ((int)centers.size() < k) {
        long num = rand() % dataVector.size();
        centers.insert(num);
    }
}


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

double objective_function (vector<dVector*>& dataVector, Cluster clusters[], int metric) {
    double obj_value=0;
    for (auto vec : dataVector) {
        vector<double> center = clusters[vec->getCluster_num()].getCenter();
        vector<double> current_vec = vec->getVector();
        double dist=distance(center, current_vec, metric);
        obj_value+=dist*dist;
    }
    return obj_value;
}

bool kmeans_assignment (vector<dVector*>& dataVector, Cluster clusters[], int metric, int k) {
    bool change = false;
    //int obj_num=0;

    for (auto vec : dataVector) {

        int cluster_num=vec->getCluster_num();

        double distance;
        int new_cluster_num, second_best;
        tie(new_cluster_num, second_best, distance) = getNearestCluster(vec->getVector(), clusters, metric, k);

        //cout << distance << endl;

        if (cluster_num!=new_cluster_num) {
            //if cluster has changed erase vector from previous cluster and add it to the new cluster
            change=true;
            if (cluster_num!=-1) clusters[cluster_num].eraseVector(vec->getID());
            clusters[new_cluster_num].addObjToCluster(vec->getID());
            vec->setCluster_num(new_cluster_num);
        }
        vec->setSecond_best_cluster(second_best);
        //obj_num++;
    }
    return change;
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
