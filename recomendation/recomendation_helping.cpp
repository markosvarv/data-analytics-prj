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
#include <algorithm>
#include <unordered_map>
#include <map>

#include "Cluster.h"
#include "Tweet.h"
#include "recomendation_helping.h"
#include "parameter_values.h"

using namespace std;


double euclideanDistance (const vector<double>& v1, const vector<double>& v2) {
    double diff_square_sum = 0.0;
    for (unsigned int i=0; i<v1.size(); i++) {
        diff_square_sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }
    return sqrt(diff_square_sum);
}

double cosineDistance (const vector<double>& v1, const vector<double>& v2) {
    return (1-(inner_product(v1.begin(), v1.end(), v2.begin(), 0.0) / (magnitude(v1) * magnitude(v2))));
}

double magnitude (const vector<double>& v) {
    double sum = 0.0;
    for (auto e : v) sum+=e*e;
    return sqrt(sum);
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


int readVectors(list<dVector*>& dataList, const string& filename) {
    //cout << "filename = " << filename;
    ifstream in(filename);

    if (!in) {
        cout << "Cannot open input file.\n";
        return 0;
    }
    string str;
    int count = 0;

    while (getline(in, str)) {
        dVector *pValue;
        pValue = dVector::readVector(str, count++);
        dataList.push_back(pValue);
        //if (pValue->getVector().size()!=VECTOR_SIZE) exit(1);
    }

    in.close();
    return 1;
}

int readTweets(unordered_map <unsigned long, list<Tweet*>>& user_tweets, const string& filename) {
    string delimiter = "\t";

    size_t pos = 0;
    string token;

    ifstream in(filename);

    if (!in) {
        cout << "Cannot open input file.\n";
        return 0;
    }
    string str;

    while (getline(in, str)) {
        vector<string> words;

        if (str.empty() || str==delimiter) {
            cout << str << " NULLLLLL\n";
            //return nullptr;
        }

        unsigned long tweetid, userid;

        if ((pos = str.find(delimiter)) != string::npos) {
            token = str.substr(0, pos);

            if (token.empty()) {
                cout << "EMPTY TOKEN\n";
                return 0;
            }
            userid = stoul (token);

            str.erase(0, pos + delimiter.length());
        } else return 0;
        if ((pos = str.find(delimiter)) != string::npos) {
            token = str.substr(0, pos);

            if (token.empty()) {
                cout << "EMPTY TOKEN\n";
                return 0;
            }
            tweetid = stoul (token);

            str.erase(0, pos + delimiter.length());
        } else return 0;
        while ((pos = str.find(delimiter)) != string::npos) {
            token = str.substr(0, pos);

            if (token.empty()) {
                cout << "EMPTY TOKEN\n";
                continue;
            }
            words.emplace_back(token);

            str.erase(0, pos + delimiter.length());
        }

        if (!str.empty() && str!=" " && str!="\n" && str!="\r") {
            words.emplace_back(str);
        }
        else {
            cout << "PROBLEMAA\n";
            return 0;
        }

        //cout << "tweetid = " << tweetid << " userid = " << userid << endl;
        auto tweet = new Tweet (tweetid, userid, words);
        user_tweets[userid].push_back(tweet);

    }

    in.close();
    return 1;
}

int readVader_dict(map<string, double>& vader_dict, const string& filename) {
    string token;
    size_t pos = 0;

    string delimiter = "\t";

    ifstream in(filename);

    if (!in) {
        cout << "Cannot open input file.\n";
        return 0;
    }
    string str;
    //int count = 0;

    while (getline(in, str)) {

        if (str.empty() || str==delimiter) {
            cout << str << " NULLLLLL\n";
            return 0;
        }

        if ((pos = str.find(delimiter)) != string::npos) {
            token = str.substr(0, pos);

            if (token.empty()) {
                cout << "EMPTY TOKEN\n";
                continue;
            }
            str.erase(0, pos + delimiter.length());
        }

        if (!str.empty() && str!=" " && str!="\n" && str!="\r") {
            double sent = stod(str);
            vader_dict[token] = sent;
        }
        else {
            cout << "PROBLEMAA\n";
            return 0;
        }

    }
    in.close();
    return 1;
}

int readCoins(vector<string>& coins, const string& filename) {
    string token;
    ifstream in(filename);

    if (!in) {
        cout << "Cannot open input file.\n";
        return 0;
    }
    string str;
    while (getline(in, str)) {
        if (str.empty()) {
            cout << str << " NULLLLLL\n";
            return 0;
        }
        coins.emplace_back(str);
    }

    in.close();
    return 1;
}




