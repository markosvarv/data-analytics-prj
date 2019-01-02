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
#include "recomendation_helping.h"


double euclideanDistance (const std::vector<double>& v1, const std::vector<double> &v2);
double cosineDistance (const std::vector<double>& v1, const std::vector<double>& v2);
double magnitude (const std::vector<double>& v);
double distance (const std::vector<double>& v1, const std::vector<double>& v2, int metric);
int readVectors(list<dVector*>& dataList, const string& filename);
int readTweets(unordered_map <unsigned long, list<Tweet*>>& user_tweets, const string& filename);
int readVader_dict(map<string, double>& vader_dict, const string& filename);
int readCoins(vector<string>& coins, const string& filename);



#endif //RECOMENDATION_RECOMENDATION_HELPING_H
