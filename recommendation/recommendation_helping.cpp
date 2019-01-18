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
#include "clustering.h"

using namespace std;

int recommendationAlgorithms (list<dVector*> vectors, list <dVector*> uvectors, const vector<string>& coins, int alg_num, int k, ofstream& output) {
    //hashing
    //for every user, find the P better similar users
    auto begin = chrono::high_resolution_clock::now();

    unordered_map<string, list<dVector *>> umap[L_DEFAULT];
    vector<hFunction> hF[L_DEFAULT];
    Cluster clusters[W_DEFAULT];

    int metric;
    if (alg_num==1) {
        metric = COSINE;
        default_random_engine generator;
        for (int i = 0; i < L_DEFAULT; i++) {
            hF[i] = hFunction::init_hFunctions(coins.size(), K_DEFAULT, generator);
        }
        add_toHashTable(vectors, K_DEFAULT, hF, umap, L_DEFAULT, metric);
    }
    else if (alg_num==2) {
        metric = EUCLIDEAN;
        //copy the contents of the list to a new vector for easier management
        vector<dVector*> dataVector{ std::make_move_iterator(std::begin(vectors)),
                                      std::make_move_iterator(std::end(vectors)) };

        clustering_algorithms (W_DEFAULT, metric, dataVector, clusters);
    }
    else return 0;

    for (dVector* vec : uvectors) {
        //cout << "mphka sto uVectors\n";
        set<dVector*> rNN;
        vector<double> dvec = vec->getVector();
        if (!vec->hasInfo()) { //if vector has no info, recommend random cryptocurrencies
            output << "<u" << vec->getID() << "> ";
            for (int i=0; i<k; i++) {
                string cryptocur = coins[rand()%coins.size()];
                string firstWord = cryptocur.substr(0, cryptocur.find('\t'));
                output << firstWord << '\t';
            }
            output << endl;
            continue;
        }
        //cout << "eimai prin to range search\n";
        if (alg_num==1) rangeSearch(rNN, vec, umap, hF, K_DEFAULT, L_DEFAULT, R_DEFAULT, metric);
        else getClusterVectors(rNN, vec, clusters);

        //cout << "set size = " << rNN.size() << endl;
        //cout << counter++ << endl;
        const dVector* PVectors[P_DEFAULT]; //P better similar users
        keep_P_Best(rNN, dvec, P_DEFAULT, PVectors, metric);
        if (!vec->normalization(PVectors, P_DEFAULT, metric)) {
            cerr << "ERROR IN NORMALIZATION\n";
            return 0;
        }
        int kBest[k];
        vec->recommend_K_Best(k, kBest);

//        vector<double> temp = vec->getVector();
//        for (double tempvalue : temp) cout << tempvalue << ' ';
//        cout << endl;

        output << "<u" << vec->getID() << "> ";
        for (int i=0; i<k; i++) {
            string cryptocur = coins[kBest[i]];
            string firstWord = cryptocur.substr(0, cryptocur.find('\t'));
            output << firstWord << '\t';
        }
        output << endl;
    }
    auto end = chrono::high_resolution_clock::now();
    auto dur = end - begin;
    long msec = (chrono::duration_cast<std::chrono::milliseconds>(dur).count());
    output << "Execution Time: " << msec << " ms\n\n\n";
    return 1;
}

void fill_uVectors (list<dVector*>& uVectors, const map <unsigned long, list<Tweet*>>& user_tweets, const vector<string>& coins) {
    for (auto& it : user_tweets) { //for every user
        vector<double> uVec;
        uVec.reserve(coins.size());

        int id=0;
        for (const string& coin : coins) { //for every coin
            double user_coin_sent=numeric_limits<double>::max();
            double user_coin_sent_sum = 0;
            for (Tweet* tweet : it.second) {
                if (tweet->containsCrypto(coin)) {
                    user_coin_sent_sum+=tweet->getSentiment();
                    user_coin_sent = user_coin_sent_sum;
                }
                id = int(tweet->getUserid());
            }
            uVec.emplace_back(user_coin_sent);
        }
        auto dvec = new dVector(uVec, id);
        dvec->setUnknownElements();
        uVectors.push_back(dvec);
    }
}

void fill_cVectors (list<dVector*>& cVectors, unordered_map<int, Tweet*>& tweets, const vector<string>& coins) {
    list<dVector*> dataList;
    if (!readVectors(dataList, DATASET)) {
        cerr << "Error while reading input file\n";
        return;
    }
    //copy the contents of the list to a new vector for easier management
    vector<dVector*> dataVector{ std::make_move_iterator(std::begin(dataList)),
                                 std::make_move_iterator(std::end(dataList)) };

    Cluster clusters[VU_NUM];
    clustering_algorithms (VU_NUM, COSINE, dataVector, clusters);

    //for every cluster
    for (unsigned long cluster_num=0; cluster_num<VU_NUM; cluster_num++) {
        vector<double> cVec;
        cVec.reserve(coins.size());

        int coin_num=0;
        int id=0;
        set<dVector*> vectors = clusters[cluster_num].getVectors();
        for (const string& coin : coins) { //for every coin
            double cluster_coin_sent=numeric_limits<double>::max();
            double cluster_coin_sent_sum=0;
            for (dVector* tweet : vectors) {
                if (tweets[tweet->getID()]) {
                    if (tweets[tweet->getID()]->containsCrypto(coin)) {
                        cluster_coin_sent_sum += tweets[tweet->getID()]->getSentiment();
                        cluster_coin_sent = cluster_coin_sent_sum;
                    }
                }
                else cout << "PROBLEMA tweet_id = " << tweet->getID() << endl;
                id = int(tweets[tweet->getID()]->getUserid());
            }
            cVec.emplace_back(cluster_coin_sent);
            coin_num++;
        }
        auto dvec = new dVector(cVec, id);
        dvec->setUnknownElements();
        cVectors.push_back(dvec);
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




