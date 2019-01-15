#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <list>
#include <fstream>
#include <set>
#include <random>
#include <string>
#include <map>
#include "Tweet.h"
#include "clustering.h"
#include "recommendation_helping.h"
#include "parameter_values.h"
#include "lsh.h"
#include "dVector.h"

using namespace std;

int main(int argc, char** argv) {
    int opt;
    char *dvalue=nullptr, *ovalue = nullptr;
    bool vvalue = false;
    if (argc!=5 && argc!=6) {
        cout << "Must give 2 or 3 arguments\n";
        return -1;
    }

    while ((opt = getopt(argc, argv, "d:o:v")) != -1) {
        switch (opt) {
            case 'd':
                dvalue = optarg;
                break;
            case 'o':
                ovalue = optarg;
                break;
            case 'v':
                vvalue=true;
                break;
            case '?':
                if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                exit(1);
            default:
                abort();
        }
    }

    printf("dvalue=%s ovalue=%s, ", dvalue, ovalue);
    cout << "vvalue = " << vvalue << endl;
    //srand (time(NULL));
    if (!ovalue || !dvalue) {
        cout << "Arguments cannot be empty\n";
        return -1;
    }
    list<dVector*> vectorsList;
    map <unsigned long, list<Tweet*>> user_tweets;
    if (!readTweets(user_tweets, dvalue)) {
        cerr << "Error while reading tweets\n";
        return -1;
    }
//    for (Tweet* t : user_tweets[45557]) {
//        cout << t->getTweetid() << endl;
//        vector<string> words = t->getWords();
//        for (string& word : words) {
//            cout << word;
//        }
//        cout << endl;
//    }
    map<string, double> vader_dict;
    if (!readVader_dict(vader_dict, "data/vader_lexicon.csv")){
        cerr << "Error while reading vader lexicon\n";
        return -1;
    }

    unordered_map<int, Tweet*> tweets;

    unsigned long users_count = 0;
    for (auto& it : user_tweets) {
        users_count++;
        for (Tweet* tweet : it.second) {
            tweet->calcSent(vader_dict);
            tweets[tweet->getTweetid()]=tweet;
        }
    }
    cout << "user_count = " << users_count << endl;

    vector<string> coins;
    if (!readCoins(coins, "data/coins_queries.csv")) {
        cerr << "Error while reading from coin queries\n";
        return -1;
    }
    list<dVector*> uVectors;

    for (auto& it : user_tweets) { //for every user
        vector<double> uVec;
        uVec.reserve(coins.size());

        int id;
        for (string& coin : coins) { //for every coin
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

    //normalization

    //dVector* vec = uVectors.front();
    //vec->normalize();


    //for (dVector* vec : uVectors) {
        //vec->normalize();



//        vector<double> cur_vec = vec->getVector();
//        vector<bool> info_el = vec->getKnownElements();
//        for (double cur : cur_vec) {
//            cout << cur << ' ';
//        }
//        cout << endl;
//        for (bool el : info_el) {
//            cout << el << ' ';
//        }
//        cout << "\nhasInfo = " << vec->hasInfo() << endl;
//        cout << "average = " << vec->getAverage() << endl;
//        cout << endl;
    //}




    //hashing
    default_random_engine generator;
    unordered_map<string, list<dVector *>> umap[L_DEFAULT];
    vector<hFunction> hF[L_DEFAULT];
    for (int i = 0; i < L_DEFAULT; i++) {
        hF[i] = hFunction::init_hFunctions(coins.size(), K_DEFAULT, generator);
    }
    add_toHashTable (uVectors, K_DEFAULT, hF, umap, L_DEFAULT, COSINE);


    //for every user, find the P better similar users


    cout << "size = " << uVectors.size() << endl;
    int counter=0;
    for (dVector* vec : uVectors) {
        //cout << "mphka sto uVectors\n";
        set<dVector*> rNN;
        vector<double> dvec = vec->getVector();
        if (!vec->hasInfo()) {
            cout << "vector has no info. Continue\n";
            continue;
        }
        //cout << "eimai prin to range search\n";
        rangeSearch(rNN, vec, umap, hF, K_DEFAULT, L_DEFAULT, R_DEFAULT, COSINE);
        //cout << "set size = " << rNN.size() << endl;
        //cout << counter++ << endl;
        const dVector* PVectors[P_DEFAULT]; //P better similar users
        keep_P_Best(rNN, dvec, P_DEFAULT, PVectors, COSINE);
        if (!vec->normalization(PVectors, P_DEFAULT, COSINE)) {
            cerr << "ERROR IN NORMALIZATION\n";
        }
        int kBest[5];
        vec->recommend_K_Best(5, kBest);

//        vector<double> temp = vec->getVector();
//        for (double tempvalue : temp) cout << tempvalue << ' ';
//        cout << endl;

        cout << "Best values are:\n";
        for (int i=0; i<5; i++) cout << kBest[i] << ' ';
        cout << endl;

    }



//    list<dVector*> dataList;
//    if (!readVectors(dataList, "data/twitter_dataset_small_v2.csv")) {
//        cerr << "Error while reading input file\n";
//        return -1;
//    }
//    //copy the contents of the list to a new vector for easier management
//    vector<dVector*> dataVector{ std::make_move_iterator(std::begin(dataList)),
//                                 std::make_move_iterator(std::end(dataList)) };
//
//    Cluster clusters[K_VALUE];
//    clustering_algorithms (K_VALUE, COSINE, dataVector, clusters);
//
//    double cVectors[K_VALUE][coins_count];
//
//    //for every cluster
//    for (unsigned long cluster_num=0; cluster_num<K_VALUE; cluster_num++) {
//        int coin_num=0;
//        set<dVector*> vectors = clusters[cluster_num].getVectors();
//        for (string& coin : coins) { //for every coin
//            double cluster_coin_sent_sum=0;
//            for (dVector* tweet : vectors) {
//                if (tweets[tweet->getID()]) {
//                    if (tweets[tweet->getID()]->containsCrypto(coin)) cluster_coin_sent_sum+=tweets[tweet->getID()]->getSentiment();
//                }
//                else cout << "PROBLEMA tweet_id = " << tweet->getID() << endl;
//            }
//            cVectors[cluster_num][coin_num] = cluster_coin_sent_sum;
//            coin_num++;
//        }
//    }
//    for (unsigned long i=0; i<K_VALUE; i++) {
//        for (unsigned long j=0; j<coins_count; j++) {
//            cout << cVectors[i][j] << " ";
//        }
//        cout << endl;
//    }
//    cout << endl;


//    map<string, double>::iterator it;
//    if((it = vader_dict.find("swearing")) != vader_dict.end())
//        cout << it->first<<" =>"<< it->second << '\n';
}
