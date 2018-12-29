#include "Cluster.h"
#include "clustering_helping.h"

using namespace std;

void Cluster::setCenter (const vector<double>& new_center) {
    center = new_center;
    //center_num = new_center_num;
}

vector<double>& Cluster::getCenter() {
    return center;
}

bool Cluster::addObjToCluster (int obj) {
    if (vectors.find(obj)==vectors.end()) {
        //cout << "ENDDDDD\n";
        vectors.insert(obj);
        return true;
    }
    else return false;
}

void Cluster::updateCenter (const vector<dVector*>& dataVector) {
    if (vectors.empty()) return;

    int size = dataVector[0]->getVector().size();
    vector<double> new_center(size, 0.0);

    for (auto object : vectors) {
        vector<double> vec = dataVector[object]->getVector();
        addVectors(new_center, vec);
    }


    for (int i=0; i<size; i++) {
        new_center[i]/=vectors.size();
    }

    center = new_center;
}

void Cluster::updatePAM_Lloyds(const vector<dVector*>& dataVector, int metric) {

    double dmin = numeric_limits<double>::max();
    int t = (int)dataVector.size();

    cout << vectors.size() << endl;
    for (int vector_num : vectors) {
        //for every vector in cluster compute the min total distance from the other vectors
        double d = vectorDistanceSum(dataVector, vector_num, metric);
        if (d<dmin) {
            dmin = d;
            t = vector_num;
        }
    }
    center = dataVector[t]->getVector();  //the new center is the vector with the min distance
}

void Cluster::addVectors (vector<double>& a, const vector<double>& b) {
    assert(a.size() == b.size());

    for (unsigned int i=0; i<a.size(); i++) {
        a[i]+=b[i];
    }
}

double Cluster::silhouette_sum (vector<dVector*>& dataVector, Cluster clusters[], int cluster_num, int metric) {
    double sil_sum = 0;

    //for every vector compute the average distance from its cluster
    //and the average distance from the second best cluster
    for (auto vec : vectors) {
        int second_best_cluster = dataVector[vec]->getSecond_best_cluster();

        //cout << "cluster = " << cluster << "\tsecond_best = " << second_best_cluster << endl;

        double ai=0;
        //cout << "eimai prin to if\n";
        if (this->getItemsNum()>1)
            ai = vectorDistanceSum(dataVector, vec, metric)/(getItemsNum()-1); //-1 because cluster contains the object
        double bi = clusters[second_best_cluster].vectorDistanceSum(dataVector, vec, metric)/clusters[second_best_cluster].getItemsNum();
        //cout << "eimai meta to if\n";

        double max;
        ai > bi ? max=ai : max=bi;
        sil_sum += (bi-ai)/max;
    }
    return sil_sum;
}

double Cluster::vectorDistanceSum (const vector<dVector*>& dataVector, int vector_num, int metric) {
    vector<double> given_vector = dataVector[vector_num]->getVector();
    double dsum = 0;
    //bool same_vector = false;
    for (int object : vectors) {
        //cout << object << endl;
        if (object==vector_num) {
            //cout << "MPHKA STO IF\n";
            //same_vector = true;
            continue;
        }
        vector<double> vec = dataVector[object]->getVector();
        dsum += distance(given_vector, vec, metric);
    }

    //int size = vectors.size();
    //if (same_vector) size--;
    return dsum;
}

void Cluster::eraseVector (int vector_num) {
    vectors.erase(vector_num);
}

set<int> Cluster::getVectors () {
    return vectors;
}