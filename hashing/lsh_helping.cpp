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
#include "mutual_helping.h"
#include "hFunction.h"
#include "dVector.h"
#include "lsh_helping.h"

using namespace std;

void add_toHashTable (list<dVector*>& dataList, int k, vector<hFunction> hF[], unordered_map<string, list<dVector*>> umap[], int L, int metric) {
    for (dVector* pValue: dataList) {
        for (int i = 0; i < L; i++) {
            string hashString = getHashString(*pValue, k, hF[i], metric);
            umap[i][hashString].push_back(pValue);
        }
    }
}

int readQuery(list<dVector*>& dataList, const string& in, const string& out, int k, vector<hFunction> hF[], unordered_map<string, list<dVector*>> umap[], int L, int metric) {
    ifstream input(in);
    ofstream output;
    output.open (out);

    if(!input || !output) {
        cerr  << "Cannot open file.\n";
        return 0;
    }

    double R=0, max_approx_fraction = 0.0, sum_approx_time = 0.0;
    string str, word1, word2;
    if (getline(input, str)) {
        istringstream iss(str);
        iss >> word1 >> word2;
        if (word1=="Radius:") {
            R = stod(word2);
            if (!getline(input, str)) return -1;
        } else R=0;
    }else return -1;

    cout << "Starting queries with R = " << R << endl;
    unsigned long count=0;
    do {
        //if(in) cout << str << endl;
        dVector *qValue;
        double approx_distance, real_distance, approx_time, real_time;
        string real_name, approx_name;
        set<string> rNN; //use a set to have unique vector names

        qValue = dVector::readVector(str, count++);

        rangeSearch(rNN, *qValue, umap, hF, k, L, R, metric);
        output << "Query: " << qValue->getName() << "\nR-near neighbors:\n";
        if (R>0) {
            for(auto it = rNN.begin(); it != rNN.end(); it++) output << *it << endl;
        }

        tie(real_name, real_distance, real_time) = realDistance(qValue->getVector(), dataList, metric);
        tie(approx_name, approx_distance, approx_time) = approximateNN(*qValue, umap, L, k, hF, metric);

        output << "Nearest LSH neighbor:\t" << approx_name << "\nNearest real neighbor:\t" << real_name << endl;
        output <<"distanceLSH:\t" << approx_distance << "\n" << "distanceTrue:\t" << real_distance << endl;
        output << "tLSH:\t" << approx_time << " ms\ntTrue:\t" << real_time << " ms\n\n";

        double current_approx_fraction = approx_distance/real_distance;
        if (current_approx_fraction > max_approx_fraction) max_approx_fraction = current_approx_fraction;

        sum_approx_time+=approx_time;

        delete (qValue);
    }while(getline(input, str));

    cout << "max approximate fraction: " << max_approx_fraction << endl;
    cout << "average approximate time: " << sum_approx_time/count << " ms\n";

    input.close();
    output.close();

    return 1;
}


tuple<string, double, double> approximateNN (dVector& query, unordered_map<string, list<dVector*>> umap[], int L, int k, vector<hFunction> hF[], int metric) {
    auto begin = chrono::high_resolution_clock::now();

    double dmin = numeric_limits<double>::max();
    dVector* b= nullptr;
    vector<double> q = query.getVector();
    double t;
    string minstring;

    for (int i=0; i<L; i++) {
        string queryHashString = getHashString(query, k, hF[i], metric);
        for (dVector* pVector : umap[i][queryHashString]) {
            vector<double> p = pVector->getVector();
            if (((t=distance(q, p, metric)))<dmin) {
                minstring = queryHashString;
                b=pVector;
                dmin = t;
            }
        }
    }
    if (b) {
        auto end = chrono::high_resolution_clock::now();
        auto dur = end - begin;
        double ms = (chrono::duration_cast<std::chrono::nanoseconds>(dur).count())/1000000.0;
        return make_tuple(b->getName(), dmin, ms);
    }
    else cout << "ApproximateNN didn't find query " << query.getName() << " in hash table\n";
    return make_tuple("", 0, 0);
}

void rangeSearch (set<string>& rNN, dVector& query, unordered_map<string, list<dVector*>> umap[], vector<hFunction> hF[], int k, int L, double R, int metric) {
    vector<double> q = query.getVector();
    for (int i = 0; i < L; i++) {
        string queryHashString = getHashString(query, k, hF[i], metric);
        for (dVector *pVector : umap[i][queryHashString]) {
            vector<double> p = pVector->getVector();
            if (distance(p, q, metric) < R * C_VALUE)
                rNN.insert(pVector->getName());
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
