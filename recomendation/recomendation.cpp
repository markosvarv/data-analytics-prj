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
#include "recomendation_helping.h"
#include "parameter_values.h"

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
    unordered_map <unsigned long, list<Tweet*>> user_tweets;
    if (!readTweets(user_tweets, dvalue)) {
        cerr << "Error while reading tweets\n";
        return -1;
    }
//    for (Tweet* t : user_tweets[45479]) {
//        cout << t->getTweetid() << endl;
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
    unsigned long coins_count = coins.size();
    //double uVectors[users_count][coins_count];

//    unsigned long user_num=0;
//    for (auto& it : user_tweets) { //for every user
//        int coin_num=0;
//        for (string& coin : coins) { //for every coin
//            double user_coin_sent_sum=0;
//            for (Tweet* tweet : it.second) {
//                if (tweet->containsCrypto(coin)) {
//                    user_coin_sent_sum+=tweet->getSentiment();
//                }
//            }
//            uVectors[user_num][coin_num] = user_coin_sent_sum;
//            coin_num++;
//        }
//        user_num++;
//    }
//
//    for (unsigned long i=0; i<users_count; i++) {
//        for (unsigned long j=0; j<coins_count; j++) {
//            cout << uVectors[i][j] << " ";
//        }
//        cout << endl;
//    }
//    cout << endl;

    list<dVector*> dataList;
    if (!readVectors(dataList, "data/twitter_dataset_small_v2.csv")) {
        cerr << "Error while reading input file\n";
        return -1;
    }
    //copy the contents of the list to a new vector for easier management
    vector<dVector*> dataVector{ std::make_move_iterator(std::begin(dataList)),
                                 std::make_move_iterator(std::end(dataList)) };

    Cluster clusters[K_VALUE];
    clustering_algorithms (K_VALUE, COSINE, dataVector, clusters);

    double cVectors[K_VALUE][coins_count];

    //for every cluster
    for (unsigned long cluster_num=0; cluster_num<K_VALUE; cluster_num++) {
        int coin_num=0;
        set<dVector*> vectors = clusters[cluster_num].getVectors();
        for (string& coin : coins) { //for every coin
            double cluster_coin_sent_sum=0;
            for (dVector* tweet : vectors) {
                if (tweets[tweet->getID()]) {
                    if (tweets[tweet->getID()]->containsCrypto(coin)) cluster_coin_sent_sum+=tweets[tweet->getID()]->getSentiment();
                }
                else cout << "PROBLEMA tweet_id = " << tweet->getID() << endl;
            }
            cVectors[cluster_num][coin_num] = cluster_coin_sent_sum;
            coin_num++;
        }
    }
    for (unsigned long i=0; i<K_VALUE; i++) {
        for (unsigned long j=0; j<coins_count; j++) {
            cout << cVectors[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;


//    map<string, double>::iterator it;
//    if((it = vader_dict.find("swearing")) != vader_dict.end())
//        cout << it->first<<" =>"<< it->second << '\n';
}
