#include <limits>
#include <iostream>
#include <vector>
#include <sstream>
#include <limits>
#include <cmath>
#include <numeric>
#include "dVector.h"
#include "parameter_values.h"

using namespace std;

dVector::dVector(const vector<double> given_p, int given_id) {
    p = given_p;
    cluster_num = -1;
    id = given_id;
};

std::vector<double> dVector::getVector () const {
    return p;
}


bool dVector::hasInfo () const {
    for (unsigned long cur=0; cur<p.size(); cur++) {
        if (p[cur]!=0 && known_elements[cur]) return true;
    }
    return false;
}

void dVector::setUnknownElements () {
    known_elements.reserve(p.size());
    //cout << info_elements.size() << endl;

    for (unsigned long cur=0; cur < p.size(); cur++) {
        known_elements.push_back(p[cur]!=numeric_limits<double>::max());

    }
    double average = getAverage();
    for (unsigned long cur=0; cur<p.size(); cur++) {
        if (!known_elements[cur]) p[cur] = average;

    }
}


double dVector::getAverage() const {
    int count=0;
    double sum=0;
    for (unsigned long cur=0; cur<p.size(); cur++) {
        if (known_elements[cur]) {
            sum+=p[cur];
            count++;
        }
    }
    if (count) return sum/count;
    else return 0;
}

double euclideanDistance (const vector<double>& v1, const vector<double>& v2) {
    double diff_square_sum = 0.0;
    for (unsigned int i=0; i<v1.size(); i++) {
        diff_square_sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }
    return sqrt(diff_square_sum);
}

double magnitude (const vector<double>& v) {
    double sum = 0.0;
    for (auto e : v) sum+=e*e;
    return sqrt(sum);
}

double cosineDistance (const vector<double>& v1, const vector<double>& v2) {
    return (1-(inner_product(v1.begin(), v1.end(), v2.begin(), 0.0) / (magnitude(v1) * magnitude(v2))));
}

double similarity (const vector<double>& v1, const vector<double>& v2) {
    return ((inner_product(v1.begin(), v1.end(), v2.begin(), 0.0) / (magnitude(v1) * magnitude(v2))));
}

double distance (const vector<double>& v1, const vector<double>& v2, int metric) {
    unsigned long v1_size, v2_size;
    v1_size=v1.size(); v2_size=v2.size();

    if (v1_size!=v2_size) {
        cerr << "Error in distance. Vectors must have the same size" << endl;
        //cout << "v1 size = " << v1_size << " v2 size = " << v2_size << endl;
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


dVector* dVector::readVector (string str) {
    string delimiter = ",";
    vector<double> p;

    size_t pos = 0;
    string token;
    //cout << str << endl;

    if (str.empty() || str==delimiter) {
        cout << str << " NULLLLLL\n";
        return nullptr;
    }

    pos = str.find(delimiter);
    token = str.substr(0, pos);
    if (token.empty()) {
        cout << "2NULLLLLL\n";
        return nullptr;
    }

    int id = stoi(token);

    str.erase(0, pos + delimiter.length());

    //cout << token << endl;

    while ((pos = str.find(delimiter)) != string::npos) {
        token = str.substr(0, pos);

        if (token.empty()) continue;

        double d = stod(token);
        p.push_back(d);
        //cout << d << endl;

        str.erase(0, pos + delimiter.length());
    }

    if (!str.empty() && str!=" " && str!="\n" && str!="\r") {
        //cout << "str = " << str << endl;
        double d = stod(str);
        p.push_back(d);
    }

    auto dValue = new dVector(p, id);
    return dValue;
}