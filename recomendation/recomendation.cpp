#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <list>
#include <fstream>
#include <set>
#include <random>
#include <string>

#include "recomendation_helping.h"

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
    //readTweets(dvalue);
    readVader_dict("data/vader_lexicon.csv");
}