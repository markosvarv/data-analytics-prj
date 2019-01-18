#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>

#include <stdlib.h>
#include <unordered_map>
#include <list>
#include <limits>
#include <cstddef>
#include <sstream>
#include <random>
#include <set>
#include <chrono>
#include <tuple>

#include "parameter_values.h"

#include "hFunction.h"
#include "dVector.h"
#include "lsh_helping.h"
#include "mutual_helping.h"

using namespace std;

int main(int argc, char *argv[]) {
    int opt, Lvalue = L_DEFAULT, kvalue = K_DEFAULT;

    char *dvalue = nullptr, *qvalue = nullptr, *ovalue = nullptr;

    while ((opt = getopt(argc, argv, "d:q:k:L:o:")) != -1) {
        switch (opt) {
            case 'd':
                dvalue = optarg;
                break;
            case 'q':
                qvalue = optarg;
                break;
            case 'k':
                kvalue = stoi(optarg);
                break;
            case 'L':
                Lvalue = stoi(optarg);
                break;
            case 'o':
                ovalue = optarg;
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
    //printf("dvalue = %s; qvalue=%s; kvalue=%d, Lvalue=%d, ovalue=%s\n", dvalue, qvalue, kvalue, Lvalue, ovalue);
    if (kvalue<=0 || Lvalue<=0) {
        cout << "k and L must be positive numbers";
        return 1;
    }

    default_random_engine generator;

    unordered_map<string, list<dVector *>> umap[Lvalue];
    list<dVector*> vectorsList;
    vector<hFunction> hF[Lvalue];

    for (int i = 0; i < Lvalue; i++) {
        hF[i] = hFunction::init_hFunctions(128, kvalue, generator);
    }

    string input(dvalue), query(qvalue), output(ovalue);
    int metric = readInput(vectorsList, dvalue);
    if (metric==-1) {
        cerr <<"Error while reading data\n";
        return 1;
    }
    add_toHashTable (vectorsList, kvalue, hF, umap, Lvalue, metric);
    unsigned long total_size=0;
    for (int i=0; i<Lvalue; i++) {
        for (auto it : umap[i]) total_size += sizeof(it);
    }
    cout << "Size of hash tables: " << total_size << endl;

    do {
        readQuery(vectorsList, qvalue, ovalue, kvalue, hF, umap, Lvalue, metric);
        cout << "\nEnd of query data, type the name of another query file, or 'exit'\n";
        cin >> output;
    }while(output!="exit");

//    int n;
//    for (int i=0; i<Lvalue; i++) {
//        n = umap[i].bucket_count();
//        std::cout << "mymap has " << n << " buckets.\n";
//    }

    for (auto i = vectorsList.begin(); i != vectorsList.end(); i++) delete (*i);

    return 0;
}