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

#include "Cluster.h"
#include "recomendation_helping.h"


int readInput(list<dVector*>& dataList, const string& filename);
int readTweets(const string& filename);
int readVader_dict(map<string, double>& vader_dict, const string& filename);
int readCoins(vector<string>& coins, const string& filename);



#endif //RECOMENDATION_RECOMENDATION_HELPING_H
