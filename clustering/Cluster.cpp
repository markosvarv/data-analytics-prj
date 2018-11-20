#include "Cluster.h"

void Cluster::setCenter (const vector<double>& new_center) {
    center = new_center;
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

void Cluster::addVectors (vector<double>& a, const vector<double>& b) {
    assert(a.size() == b.size());

    for (unsigned int i=0; i<a.size(); i++) {
        a[i]+=b[i];
    }

}

void Cluster::eraseVector (int vector_num) {
    vectors.erase(vector_num);
}