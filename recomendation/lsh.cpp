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
            string hashString = getHashString(pValue, k, hF[i], metric);
            //cout << hashString << endl;
            umap[i][hashString].push_back(pValue);
            //if (hashString == "11001") cout << "TO VRIKAAAA\n";
            //cout << umap[i]["11001"].size() << endl;
        }
    }
}

void rangeSearch (set<dVector*>& rNN, const dVector* query, unordered_map<string, list<dVector*>> umap[], vector<hFunction> hF[], int k, int L, double R, int metric) {
    vector<double> q = query->getVector();

    for (int i = 0; i < L; i++) {
        string queryHashString = getHashString(query, k, hF[i], metric);
        //cout << "i = " << i << endl;
        cout << queryHashString << endl;
        //cout << umap[i] << endl;
        //if (queryHashString=="11001") return;
        for (dVector *pVector : umap[i][queryHashString]) {
            cout << "mphka sto dvector\n";
            vector<double> p = pVector->getVector();
            for (double cur : p) {
                cout << cur << ' ';
            }
            cout << endl;
//            for (double cur : q) {
//                cout << cur << ' ';
//            }
//            cout << endl;
            double t;
            if (((t=distance(p, q, metric))) < R * C_VALUE)
                rNN.insert(pVector);
            //cout << "t = " << t << endl;
        }
        //cout << "feugwwww\n";
    }
}

void keep_P_Best (const set<dVector*>& vectors, const vector<double>& given_vec, int k, const dVector* best[], int metric) {
    double min[k];
    //dVector* worst[k];
    double t;
    for (int i=0; i<k; i++) {
        min[i] = numeric_limits<double>::max();
        best[i] = nullptr;
    }

    //cout << "eimai prin to for\n";
    for (const dVector* vec : vectors) {
        //cout << "mphka sto for\n";
        for (int i=0; i<k; i++) {
            //cout << "perasa to for\n";

            vector<double> current_vector = vec->getVector();
//            for (double cur : current_vector) {
//                cout << cur << ' ';
//            }
//            cout << endl;
//            for (double cur : given_vec) {
//                cout << cur << ' ';
//            }
//            cout << endl;
            if ((t=distance(current_vector, given_vec, metric)) < min[i]) {
                cout << "t = " << t << endl;
                for (int j=k-1; j>i ; j--) min[j] = min[j-1]; //shift array
                for (int j=k-1; j>i ; j--) best[j] = best[j-1];
                min[i] = t;
                best[i] = vec;
                break;
            }
            //cout << "vgainw apo to for\n";
        }
        //cout << "vgainw apo to eksoteriko for\n";
    }
    //cout << "vgainw apo tin synartisi\n";
}

string getHashString (const dVector* pValue, int k, vector<hFunction>& h, int metric) {
    vector<double> gValue(k);

    for (int i=0; i<k; i++) {
        gValue[i] = h[i].hValue(pValue->getVector(), metric);
    }

    stringstream result;
    copy(gValue.begin(), gValue.end(), ostream_iterator<int>(result, ""));
    string r = result.str();
    return r;
}

