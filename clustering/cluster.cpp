#include <iostream>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <list>
#include <fstream>

#include "dVector.h"
#include "parameter_values.h"

using namespace std;

int readInput(list<dVector*>& dataList, const string& filename) {
    //cout << "filename = " << filename;
    ifstream in(filename);

    if(!in) {
        cout << "Cannot open input file.\n";
        return -1;
    }
    string str;
    unsigned long count=0;

    do {
        dVector *pValue;
        pValue = dVector::readVector(str, count++);
        dataList.push_back(pValue);
    }while(getline(in, str));

    in.close();
    return 1;
}

int readConfig(const string& filename, int& clusters, int& hash_functions, int& hash_tables) {
    //cout << "filename = " << filename;

    ifstream in(filename);
    if(!in) {
        cout << "Cannot open input file.\n";
        return -1;
    }

    string word1, word2;

    for (int i=0; i<3; i++) {
        in >> word1 >> word2;
        cout << word1 << word2 << endl;
        if (word1 == "number_of_clusters:") clusters = stoi(word2);
        else if (word1 == "number_of_hash_functions:") hash_functions = stoi(word2);
        else if (word1 == "number_of_hash_tables:") hash_tables = stoi(word2);
        else cerr << "unknown configuration value: " << word1 << endl;
    }
    in.close();
    return 1;
}


int main(int argc, char *argv[]) {
    int opt;
    char *ivalue = nullptr, *cvalue = nullptr, *ovalue = nullptr, *dvalue=nullptr;

    if (argc!=9) {
        cout << "Must give 4 arguments\n";
        return -1;
    }

    while ((opt = getopt(argc, argv, "i:c:o:d:")) != -1) {
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
    printf("ivalue = %s; cvalue=%s; ovalue=%s, dvalue=%s\n", ivalue, cvalue, ovalue, dvalue);

    if (!ivalue || !cvalue || !ovalue || !dvalue) {
        cout << "Arguments cannot be empty\n";
        return -1;
    }

    int clusters=0, hash_functions=H_FUNCTIONS_DEFAULT, hash_tables=H_TABLES_DEFAULT;

    readConfig(cvalue, clusters, hash_functions, hash_tables);

    if (clusters<=1) {
        cerr << "Number of clusters must be higher than 1\n";
        return -1;
    }

    list<dVector*> vectorsList;
    readInput(vectorsList, ivalue);

    for (auto i = vectorsList.begin(); i != vectorsList.end(); i++) delete (*i);
}