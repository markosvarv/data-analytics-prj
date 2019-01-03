#include "Cluster.h"
#include "recomendation_helping.h"

using namespace std;

void Cluster::setCenter (const vector<double>& new_center) {
    center = new_center;
    //center_num = new_center_num;
}

vector<double>& Cluster::getCenter() {
    return center;
}

bool Cluster::addObjToCluster (dVector* obj) {
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

    for (dVector* object : vectors) {
        vector<double> vec = object->getVector();
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
    for (dVector* vec : vectors) {
        //for every vector in cluster compute the min total distance from the other vectors
        double d = vectorDistanceSum(vec, metric);
        if (d<dmin) {
            dmin = d;
            t = vec->getID();
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
    for (dVector* vec : vectors) {
        int second_best_cluster = vec->getSecond_best_cluster();

        //cout << "cluster = " << cluster << "\tsecond_best = " << second_best_cluster << endl;

        double ai=0;
        //cout << "eimai prin to if\n";
        if (this->getItemsNum()>1)
            ai = vectorDistanceSum(vec, metric)/(getItemsNum()-1); //-1 because cluster contains the object
        double bi = clusters[second_best_cluster].vectorDistanceSum(vec, metric)/clusters[second_best_cluster].getItemsNum();
        //cout << "eimai meta to if\n";

        double max;
        ai > bi ? max=ai : max=bi;
        sil_sum += (bi-ai)/max;
    }
    return sil_sum;
}

double Cluster::vectorDistanceSum (dVector* vec, int metric) {
    vector<double> given_vector = vec->getVector();
    double dsum = 0;
    //bool same_vector = false;
    for (dVector* object : vectors) {
        //cout << object << endl;
        if (object->getID()==vec->getID()) {
            //cout << "MPHKA STO IF\n";
            //same_vector = true;
            continue;
        }
        vector<double> current_vec = object->getVector();
        dsum += distance(given_vector, current_vec, metric);
    }

    //int size = vectors.size();
    //if (same_vector) size--;
    return dsum;
}

void Cluster::eraseVector (dVector* vector) {
    vectors.erase(vector);
}

set<dVector*>& Cluster::getVectors () {
    return vectors;
}