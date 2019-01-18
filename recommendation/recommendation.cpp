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
    srand (time(NULL));
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
    map<string, double> vader_dict;
    if (!readVader_dict(vader_dict, LEXICON)){
        cerr << "Error while reading vader lexicon\n";
        return -1;
    }
    unordered_map<int, Tweet*> tweets;

    //fill tweets map
    unsigned long users_count = 0;
    for (auto& it : user_tweets) {
        users_count++;
        for (Tweet* tweet : it.second) {
            tweet->calcSent(vader_dict);
            tweets[tweet->getTweetid()]=tweet;
        }
    }

    vector<string> coins;
    if (!readCoins(coins, COINS_FILE)) {
        cerr << "Error while reading from coin queries\n";
        return -1;
    }
    cout << "Creating uVectors...\n";
    list<dVector*> uVectors;
    fill_uVectors(uVectors, user_tweets, coins);
    cout << "Creating cVectors...\n";
    list<dVector*> cVectors;
    fill_cVectors(cVectors, tweets, coins);

    ofstream output;
    output.open (ovalue);

    output << "Cosine LSH\n"; cout << "Cosine LSH\n";
    output << "A: 5 recommendations\n"; cout << "A: 5 recommendations\n";
    recommendationAlgorithms(uVectors, uVectors, coins, 1, 5, output);
    output << "B: 2 recommendations\n"; cout << "B: 2 recommendations\n";
    recommendationAlgorithms(cVectors, uVectors, coins, 1, 2, output);

    output << "Clustering\n"; cout << "Clustering\n";
    output << "A: 5 recommendations\n"; cout << "A: 5 recommendations\n";
    recommendationAlgorithms(uVectors, uVectors, coins, 2, 5, output);
    output << "B: 2 recommendations\n"; cout << "B: 2 recommendations\n";
    recommendationAlgorithms(cVectors, uVectors, coins, 2, 2, output);

    output.close();


    for (auto i = vectorsList.begin(); i != vectorsList.end(); i++) delete (*i);
    for (auto it = tweets.begin(); it != tweets.end(); ++it ) delete(it->second);
    for (auto i = uVectors.begin(); i != uVectors.end(); i++) delete (*i);
    for (auto i = cVectors.begin(); i != cVectors.end(); i++) delete (*i);
}
