#include <iostream>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <list>
#include <fstream>
#include <set>
#include <random>
#include <string>
#include <unordered_map>

#include "dVector.h"
#include "parameter_values.h"
#include "Cluster.h"
#include "clustering_helping.h"
#include "hFunction.h"
#include "lsh.h"
#include "cube.h"
#include "assignment.h"

using namespace std;

int main(int argc, char *argv[]) {
    int opt;
    char *ivalue = nullptr, *cvalue = nullptr, *ovalue = nullptr, *dvalue=nullptr;
    bool avalue = false;
    if (argc!=9 && argc!=10) {
        cout << "Must give 4 or 5 arguments\n";
        return -1;
    }

    while ((opt = getopt(argc, argv, "i:c:o:d:a")) != -1) {
        switch (opt) {
            case 'i':
                ivalue = optarg;
                break;
            case 'c':
                cvalue = optarg;
                break;
            case 'o':
                ovalue = optarg;
                break;
            case 'd':
                dvalue = optarg;
                break;
            case 'a':
                avalue=true;
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


    cout << avalue << endl;

    printf("ivalue = %s; cvalue=%s; ovalue=%s, dvalue=%s\n", ivalue, cvalue, ovalue, dvalue);
    srand (time(NULL));
    if (!ivalue || !cvalue || !ovalue || !dvalue) {
        cout << "Arguments cannot be empty\n";
        return -1;
    }

    unsigned int metric, k=0, hash_functions=H_FUNCTIONS_DEFAULT, hash_tables=H_TABLES_DEFAULT;

    if (!readConfig(cvalue, k, hash_functions, hash_tables)) {
        cerr << "Error while reading configuration file\n";
        return -1;
    }

    if (k<=1) {
        cerr << "Number of clusters must be higher than 1\n";
        return -1;
    }

    if (((metric=readMetric(dvalue)))==UNKNOWN_METRIC) return -1;

    list<dVector*> vectorsList;
    if (!readInput(vectorsList, ivalue)) {
        cerr << "Error while reading input file\n";
        return -1;
    }
    //copy the contents of the list to a new vector for easier management
    vector<dVector*> dataVector{ std::make_move_iterator(std::begin(vectorsList)),
                      std::make_move_iterator(std::end(vectorsList)) };

    //clustering_algorithms(0, 0, 0, k, metric, dataVector, ovalue, avalue);
   //clustering_algorithms(1, 0, 0, k, metric, dataVector, ovalue);
    clustering_algorithms(0, 1, 0, k, metric, dataVector, ovalue, avalue);
    //clustering_algorithms(0, 2, 0, k, metric, dataVector, ovalue, avalue);
//    clustering_algorithms(1, 1, 0, k, metric, dataVector);
//    clustering_algorithms(1, 2, 0, k, metric, dataVector);
//    clustering_algorithms(0, 0, 1, k, metric, dataVector);
//    clustering_algorithms(1, 0, 1, k, metric, dataVector);
//    clustering_algorithms(1, 1, 1, k, metric, dataVector);
//    clustering_algorithms(0, 1, 1, k, metric, dataVector);
//    clustering_algorithms(0, 2, 1, k, metric, dataVector);
//    clustering_algorithms(1, 2, 1, k, metric, dataVector);

    for (auto i = dataVector.begin(); i != dataVector.end(); i++) delete (*i);
}
