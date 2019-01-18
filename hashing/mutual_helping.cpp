#include "hFunction.h"
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

#include "mutual_helping.h"

using namespace std;

double euclideanDistance (const vector<double>& v1, const vector<double>& v2) {
    double diff_square_sum = 0.0;
    for (unsigned int i=0; i<v1.size(); i++) {
        diff_square_sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }
    return sqrt(diff_square_sum);
}

double cosineDistance (const vector<double>& v1, const vector<double>& v2) {
    return (inner_product(v1.begin(), v1.end(), v2.begin(), 0.0) / (magnitude(v1) * magnitude(v2)));
}

double magnitude (const vector<double>& v) {
    double sum = 0.0;
    for (auto e : v) sum+=e*e;
    return sqrt(sum);
}

double distance (const vector<double>& v1, const vector<double>& v2, int metric) {
    if (v1.size()!=v2.size()) {
        cerr << "Error in EuclideanDistance. Vectors must have the same size" << endl;
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

tuple<string, double, double> realDistance (const vector<double>& query, list<dVector*>& dataList, int metric) {
    auto begin = chrono::high_resolution_clock::now();

    double dmin = numeric_limits<double>::max();
    dVector* b= nullptr;


    for (dVector* pVector : dataList) {
        vector<double> p = pVector->getVector();
        double t;
        if ((t=distance(query, p, metric))<dmin) {
            dmin = t;
            b=pVector;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto dur = end - begin;
    double ms = (chrono::duration_cast<std::chrono::nanoseconds>(dur).count())/1000000.0;
    if (b) return make_tuple(b->getName(), dmin, ms);
    else return make_tuple("", 0, 0);
}


int readInput(list<dVector*>& dataList, const string& filename) {
    //cout << "filename = " << filename;
    ifstream in(filename);
    int metric;

    if(!in) {
        cout << "Cannot open input file.\n";
        return -1;
    }
    string str, word1, word2;
    if (getline(in, str)) {
        istringstream iss(str);
        iss >> word1 >> word2;
        if (word1=="@metric") {
            if (word2 == "cosine") metric = COSINE;
            else if (word2 == "euclidean") metric = EUCLIDEAN;
            else {
                cerr << "There is no metric: " << word2 << endl;
                return -1;
            }
            if (!getline(in, str)) return -1;
        } else metric = EUCLIDEAN;
    }else return -1;

    //cout << "Starting lsh algorithm with metric: " << << endl;

    unsigned long count=0;

    do {
        dVector *pValue;
        pValue = dVector::readVector(str, count++);
        dataList.push_back(pValue);
    }while(getline(in, str));

    in.close();
    return metric;
}