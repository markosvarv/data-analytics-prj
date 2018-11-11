#include <iostream>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[]) {
    int opt;
    char *ivalue = nullptr, *cvalue = nullptr, *ovalue = nullptr, *dvalue=nullptr;

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
}