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
#include "recommendation_helping.h"
#include "parameter_values.h"
#include "hFunction.h"
#include "lsh.h"

using namespace std;


//int recommend_to_allUsers (list<dVector*>& vectors, unordered_map<string, list<dVector*>> umap[], vector<hFunction> hF[]) {
//    cout << "size = " << vectors.size() << endl;
//    int counter=0;
//    for (dVector* vec : vectors) {
//        //cout << "mphka sto uVectors\n";
//        set<dVector*> rNN;
//        vector<double> dvec = vec->getVector();
//        if (!vec->hasInfo()) {
//            cout << "vector has no info. Continue\n";
//            continue;
//        }
//        //cout << "eimai prin to range search\n";
//        rangeSearch(rNN, vec, umap, hF, K_DEFAULT, L_DEFAULT, R_DEFAULT, COSINE);
//        //cout << "set size = " << rNN.size() << endl;
//        //cout << counter++ << endl;
//        const dVector* PVectors[P_DEFAULT]; //P better similar users
//        keep_P_Best(rNN, dvec, P_DEFAULT, PVectors, COSINE);
//        if (!vec->normalization(PVectors, P_DEFAULT, COSINE)) {
//            cerr << "ERROR IN NORMALIZATION\n";
//        }
//        int kBest[5];
//        vec->recommend_K_Best(5, kBest);
//
////        vector<double> temp = vec->getVector();
////        for (double tempvalue : temp) cout << tempvalue << ' ';
////        cout << endl;
//
//        cout << "Best values are:\n";
//        for (int i=0; i<5; i++) cout << kBest[i] << ' ';
//        cout << endl;
//
//    }
//}


int readVectors(list<dVector*>& dataList, const string& filename) {
    //cout << "filename = " << filename;
    ifstream in(filename);

    if (!in) {
        cout << "Cannot open input file.\n";
        return 0;
    }
    string str;

    while (getline(in, str)) {
        dVector *pValue;
        pValue = dVector::readVector(str);
        dataList.push_back(pValue);
        //if (pValue->getVector().size()!=VECTOR_SIZE) exit(1);
    }

    in.close();
    return 1;
}

int readTweets(map <unsigned long, list<Tweet*>>& user_tweets, const string& filename) {
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
            //cout << token << ' ';
            words.emplace_back(token);

            str.erase(0, pos + delimiter.length());
        }
        //cout << endl;

        if ((pos = str.find('\r')) != string::npos) {
            token = str.substr(0, pos);
        }
        else token = str;

        if (!token.empty() && token!=" " && token!="\n" && token!="\r") {
            words.emplace_back(token);
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




