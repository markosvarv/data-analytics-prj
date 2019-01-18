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
#include <bitset>
#include <sstream>


#include "parameter_values.h"

#include "hFunction.h"
#include "dVector.h"
#include "cube_helping.h"
#include "mutual_helping.h"

using namespace std;

void add_toCube (list<dVector*>& dataList, int k, vector<hFunction>& hF, list<dVector*> cube[], int metric) {
    for (dVector* pValue: dataList) {
        int probe = getCubeProbe(*pValue, k, hF, metric);
        cube[probe].push_back(pValue);
    }
}

int readQuery(const string& in, const string& out, int k, vector<hFunction>& hF, list<dVector*> cube[], int cubeSize, int probes_num, int metric, int M, list<dVector*>& dataList) {
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
        dVector *qValue;
        double approx_distance, real_distance=0, approx_time, real_time=0;
        string real_name, approx_name;
        set<string> rNN; //use a set to have unique vector names

        qValue = dVector::readVector(str, count++);

        rangeSearch(rNN, *qValue, cube, cubeSize, hF, k, R, probes_num, metric, M);
        output << "Query: " << qValue->getName() << "\nR-near neighbors:\n";
        if (R>0) {
            for(auto it = rNN.begin(); it != rNN.end(); it++) output << *it << endl;
        }

        tie(real_name, real_distance, real_time) = realDistance(qValue->getVector(), dataList, metric);
        tie(approx_name, approx_distance, approx_time) = approximateNN(*qValue, cube, cubeSize, hF, k, probes_num, metric, M);

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



void rangeSearch (set<string>& rNN, dVector& query, list<dVector*> cube[], int cubeSize, vector<hFunction>& hF, int k, double R, int probes_num, int metric, int M) {
    const int probe = getCubeProbe(query, k, hF, metric);
    vector<double> q = query.getVector();

    int countM = 0;
    for (dVector *pVector : cube[probe]) {
        if (countM>=M) break;
        vector<double> p = pVector->getVector();
        if (distance(p, q, metric) < R * C_VALUE) rNN.insert(pVector->getName());
        countM++;
    }

    int hamming_dst=1, current_num=0, probes_count=1;
    //get vectors in adjacent probes
    while (probes_count<=probes_num) {
        int current_probe=-1;

        while(current_num < cubeSize ) {
            if (getHammingDst(current_num, probe)==hamming_dst) {
                current_probe = probe;
                break;
            }
            current_num++;
        }
        if (current_num>=cubeSize) { //no other adjacent probes with hamming_dst
            hamming_dst++;
            current_num=0;
            continue;
        }

        int countM = 0;
        if (cube[current_probe].empty()) {
            current_num++;
            continue;
        }

        for (dVector *pVector : cube[current_probe]) {
            if (countM>=M) break;
            vector<double> p = pVector->getVector();
            if (distance(p, q, metric) < R * C_VALUE) rNN.insert(pVector->getName());
            probes_count++;
            countM++;
        }
    }
}

tuple<string, double, double> approximateNN (dVector& query, list<dVector*> cube[], int cubeSize, vector<hFunction>& hF, int k, int probes_num, int metric, const int M) {
    auto begin = chrono::high_resolution_clock::now();

    double dmin = numeric_limits<double>::max();
    dVector* b= nullptr;
    vector<double> q = query.getVector();
    double t;

    const int probe = getCubeProbe(query, k, hF, metric);

    int countM = 0;
    for (dVector *pVector : cube[probe]) {
        if (countM>=M) break;
        vector<double> p = pVector->getVector();
        if (((t=distance(q, p, metric)))<dmin) {
                b=pVector;
                dmin = t;
        }
        countM++;
    }

    int hamming_dst=1, current_num=0, probes_count=1;
    //get vectors in adjacent probes
    while (probes_count<=probes_num) {
        int current_probe=-1;

        while(current_num < cubeSize ) {
            if (getHammingDst(current_num, probe)==hamming_dst) {
                current_probe = probe;
                break;
            }
            current_num++;
        }
        if (current_num>=cubeSize) { //no other adjacent probes with hamming_dst
            hamming_dst++;
            current_num=0;
            continue;
        }

        int countM = 0;
        if (cube[current_probe].empty()) {
            current_num++;
            continue;
        }

        for (dVector *pVector : cube[current_probe]) {
            if (countM>=M) break;
            vector<double> p = pVector->getVector();
            if (((t=distance(q, p, metric)))<dmin) {
                b=pVector;
                dmin = t;
            }
            probes_count++;
            countM++;
        }
    }

    if (b) {
        auto end = chrono::high_resolution_clock::now();
        auto dur = end - begin;
        double ms = (chrono::duration_cast<std::chrono::nanoseconds>(dur).count())/1000000.0;
        return make_tuple(b->getName(), dmin, ms);
    }
    else cout << "ApproximateNN didn't find query " << query.getName() << " in cube\n";
    return make_tuple("", 0, 0);
}

int getHammingDst (int a, int b) {
    bitset<32> bsetA(a), bsetB(b);
    int r = ((int)(bsetA^bsetB).count());
    //cout << "hamming distance = " << r << endl;
    return r;
}

int getCubeProbe (dVector& pValue, int k, vector<hFunction>& h, int metric) {
    vector<int> gValue(k);
    bitset<32> bset;

    int i;
    for (i=0; i<k; i++) {

        gValue[i] = (int)h[i].hValue(pValue.getVector(), metric);

        if (metric==EUCLIDEAN) gValue[i] = abs(gValue[i]%2);
        //cout << (int)gValue[i] << " ";
        int bit = gValue[i];
        //cout << bit << " ";
        bset[31-i] = bit;
    }
    //cout << endl << i << endl;
    bset>>=(32-k);
    //cout << bset << endl;

    //cout << endl << endl;
    int value = (int)bset.to_ulong();
    //cout << bset.to_ulong() << endl;
    return value;
}


