#include <cmath>
#include <iostream>
#include <algorithm>

#include "Tweet.h"
#include "parameter_values.h"

using namespace std;

Tweet::Tweet(unsigned long given_tweetid, unsigned long given_user_id, const vector<string>& given_words) {
    tweetid = given_tweetid;
    userid = given_user_id;
    words = given_words;
}

int Tweet::calcSent(map<string, double> &vader_dict) {
    double totalscore = 0.0;

    for (const string& token : words) {
        totalscore+=vader_dict[token];
    }
    sentiment = totalscore / (sqrt(totalscore*totalscore+ALPHA));
    //cout << "sentiment = " << sentiment << endl;
    return 1;
}

bool Tweet::containsCrypto (string crypto) {
    string delimiter = "\t";
    size_t pos = 0;
    string token;

    while ((pos = crypto.find(delimiter)) != string::npos) {
        token = crypto.substr(0, pos);

        if (token.empty()) {
            cout << "EMPTY TOKEN\n";
            continue;
        }

        if (find(words.begin(), words.end(), token) != words.end())  {
            return true;
        }

        crypto.erase(0, pos + delimiter.length());
    }

    if ((pos = crypto.find('\r')) != string::npos) {
        token = crypto.substr(0, pos);
    }
    else token = crypto;

    if (!token.empty() && token!=" " && token!="\n" && token!="\r") {
        if (find(words.begin(), words.end(), token) != words.end()) return true;
    }
    return false;
}