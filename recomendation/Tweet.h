#ifndef RECOMENDATION_TWEET_H
#define RECOMENDATION_TWEET_H

#include <vector>
#include <string>
#include <map>

using namespace std;

class Tweet {
private:
    unsigned long tweetid;
    unsigned long userid;
    vector<string> words;
    double sentiment;

public:
    Tweet(unsigned long given_tweetid, unsigned long given_user_id, const vector<string>& given_words);

    unsigned long getTweetid () {
        return tweetid;
    }

    unsigned long getUserid () {
        return userid;
    }

    void setSentiment (double sent) {
        sentiment = sent;
    }

    double getSentiment () {
        return sentiment;
    }

    vector<string> getWords () {
        return words;
    }

    int calcSent (map<string, double>& vader_dict);

    bool containsCrypto (string crypto) ;

};
#endif //RECOMENDATION_TWEET_H
