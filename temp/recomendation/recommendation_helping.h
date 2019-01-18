#ifndef RECOMENDATION_RECOMENDATION_HELPING_H
#define RECOMENDATION_RECOMENDATION_HELPING_H

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

#include "Tweet.h"
#include "Cluster.h"
#include "recommendation_helping.h"


int readVectors(list<dVector*>& dataList, const string& filename);
int readTweets(map <unsigned long, list<Tweet*>>& user_tweets, const string& filename);
int readVader_dict(map<string, double>& vader_dict, const string& filename);
int readCoins(vector<string>& coins, const string& filename);



#endif //RECOMENDATION_RECOMENDATION_HELPING_H
