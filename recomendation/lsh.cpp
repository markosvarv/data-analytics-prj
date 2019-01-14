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
#include "recommendation_helping.h"
#include "hFunction.h"
#include "dVector.h"
#include "lsh.h"

using namespace std;

void add_toHashTable (list<dVector*>& dataList, int k, vector<hFunction> hF[], unordered_map<string, list<dVector*>> umap[], int L, int metric) {
    for (dVector* pValue: dataList) {
        if (!pValue->hasInfo()) continue;

        for (int i = 0; i < L; i++) {
            string hashString = getHashString(*pValue, k, hF[i], metric);
            cout << hashString << endl;
            umap[i][hashString].push_back(pValue);
        }
    }
}

void rangeSearch (set<int>& rNN, dVector& query, unordered_map<string, list<dVector*>> umap[], vector<hFunction> hF[], int k, int L, double R, int metric) {
    vector<double> q = query.getVector();
    for (int i = 0; i < L; i++) {
        string queryHashString = getHashString(query, k, hF[i], metric);
        for (dVector *pVector : umap[i][queryHashString]) {
            vector<double> p = pVector->getVector();
            if (distance(p, q, metric) < R * C_VALUE)
                rNN.insert(pVector->getID());
        }
    }
}

void keep_k_Best (set<dVector*>& vectors, vector<double>& given_vec, int k, int metric) {
    double min[k];
    dVector* worst[k];
    double t;
    for (int i=0; i<k; i++) {
        min[i] = numeric_limits<double>::max();
        worst[k] = nullptr;
    }
    for (dVector* vec : vectors) {
        for (int i=0; i<k; i++) {
            vector<double> current_vector = vec->getVector();
            if ((t=distance(current_vector, given_vec, metric)) < min[i]) {
                for (int j=k-1; j>i ; j--) min[j] = min[j-1]; //shift array
                for (int j=k-1; j>i ; j--) worst[j] = worst[j-1];
                min[i] = t;
                worst[i] = vec;
                break;
            }
        }
    }
}

string getHashString (dVector& pValue, int k, vector<hFunction>& h, int metric) {
    vector<double> gValue(k);

    for (int i=0; i<k; i++) {
        gValue[i] = h[i].hValue(pValue.getVector(), metric);
    }

    stringstream result;
    copy(gValue.begin(), gValue.end(), ostream_iterator<int>(result, ""));
    string r = result.str();
    return r;
}

