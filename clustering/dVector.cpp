
#include <iostream>
#include <vector>
#include <sstream>
#include "dVector.h"

using namespace std;

dVector::dVector(const string given_name, const vector<double> given_p) {
    name = given_name;
    p = given_p;
    cluster_num = -1;
};

std::vector<double> dVector::getVector () const {
    return p;
}

std::string dVector::getName() const {
    return name;
}


dVector* dVector::readVector (string str, unsigned long i) {
    string delimiter = ",";
    vector<double> p;

    size_t pos = 0;
    string token;
    //cout << str << endl;

    if (str.empty() || str.compare(delimiter)==0) {
        cout << str << " NULLLLLL\n";
        return nullptr;
    }

    pos = str.find(delimiter);
    token = str.substr(0, pos);
    if (token.empty()) {
        cout << "2NULLLLLL\n";
        return nullptr;
    }

    string name = token;

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

    auto dValue = new dVector(name, p);
    return dValue;
}