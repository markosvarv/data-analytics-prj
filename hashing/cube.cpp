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
#include <cmath>       /* log10 */
#include <bitset>

#include "parameter_values.h"

#include "hFunction.h"
#include "dVector.h"
#include "cube_helping.h"
#include "mutual_helping.h"

using namespace std;

int main(int argc, char *argv[]) {
    int opt, pvalue=P_DEFAULT, Mvalue = M_DEFAULT, kvalue=-1;

    char *dvalue = nullptr, *qvalue = nullptr, *ovalue = nullptr;

    while ((opt = getopt(argc, argv, "d:q:k:M:p:o:")) != -1) {
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
            case 'M':
                Mvalue = stoi(optarg);
                break;
            case 'p':
                pvalue = stoi(optarg);
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
    //printf("dvalue = %s; qvalue=%s; kvalue=%d, Mvalue=%d, pvalue=%d, ovalue=%s\n", dvalue, qvalue, kvalue, Mvalue, pvalue, ovalue);
    if (Mvalue<=0 || pvalue<=0) {
        cout << "Numbers in input must be positive";
        return 1;
    }

    default_random_engine generator;
    list<dVector*> vectorsList;
    string input(dvalue), query(qvalue), output(ovalue);
    int metric = readInput(vectorsList, dvalue);
    if (metric==-1) {
        cerr <<"Error while reading data\n";
        return 1;
    }

    if (kvalue<=0) kvalue=round(log10(vectorsList.size()));
    int arraylen = int(pow(2, kvalue));

    list<dVector*> cube[arraylen];
    vector<hFunction> hF;
    hF = hFunction::init_hFunctions(128, kvalue, generator);

    add_toCube (vectorsList, kvalue, hF, cube, metric);
    //cout << "Size of cube: " << arraylen*sizeof(int) << endl;

    int size=0;
    for (int i=0; i<arraylen; i++) {
        size+=sizeof(cube[arraylen]);
    }
    cout << "Size of cube: " << size << endl;

    do {
        readQuery(qvalue, ovalue, kvalue, hF, cube, arraylen, pvalue, metric, Mvalue, vectorsList);
        cout << "\nEnd of query data, type the name of another query file, or 'exit'\n";
        cin >> output;
    }while(output!="exit");


    for (auto i = vectorsList.begin(); i != vectorsList.end(); i++) delete (*i);

    return 0;
}
