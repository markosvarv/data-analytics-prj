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
#include "hFunction.h"
#include "assignment.h"
#include "init.h"
#include "cube.h"
#include "lsh.h"
#include "clustering_helping.h"

using namespace std;

double euclideanDistance (const vector<double>& v1, const vector<double>& v2) {
    double diff_square_sum = 0.0;
    for (unsigned int i=0; i<v1.size(); i++) {
        diff_square_sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }
    return sqrt(diff_square_sum);
}

double cosineDistance (const vector<double>& v1, const vector<double>& v2) {
    return (1-(inner_product(v1.begin(), v1.end(), v2.begin(), 0.0) / (magnitude(v1) * magnitude(v2))));
}

double magnitude (const vector<double>& v) {
    double sum = 0.0;
    for (auto e : v) sum+=e*e;
    return sqrt(sum);
}

double distance (const vector<double>& v1, const vector<double>& v2, int metric) {
    unsigned long v1_size, v2_size;
    v1_size=v1.size(); v2_size=v2.size();

    if (v1_size!=v2_size) {
        cerr << "Error in distance. Vectors must have the same size" << endl;
        //cout << "v1 size = " << v1_size << " v2 size = " << v2_size << endl;
        return -1.0;
    }
    switch (metric) {
        case EUCLIDEAN:
            return euclideanDistance(v1, v2);
        case COSINE:
            return cosineDistance(v1, v2);
        default:
            return -1.0;
    }
}

//return the cluster id with the nearest center from the object obj
tuple<int, int, double> getNearestCluster (const vector<double>& obj, Cluster clusters[], int metric, unsigned int clusters_num) {
    //auto begin = chrono::high_resolution_clock::now();

    unsigned int cmin = clusters_num;
    unsigned int second_cmin = clusters_num;
    //dVector* b= nullptr;
    double dmin = numeric_limits<double>::max();
    double second_dmin = numeric_limits<double>::max();

    for (unsigned int i=0; i<clusters_num; i++) {
        vector<double> p = clusters[i].getCenter();

        double t = distance(obj, p, metric);
        //cout << "t = " << t << endl;
        if (t<=dmin) {
            second_cmin = cmin;
            second_dmin = dmin;
            cmin = i;
            dmin = t;
            //b=pVector;
        }else if (t<second_dmin) {
            second_cmin = i;
            second_dmin = dmin;
        }
    }
    //cout << "min distance = " << dmin;
    //auto end = chrono::high_resolution_clock::now();
    //auto dur = end - begin;
    //double ms = (chrono::duration_cast<std::chrono::nanoseconds>(dur).count())/1000000.0;
    //if (b) return make_tuple(b->getName(), dmin, ms);
    //else return make_tuple("", 0, 0);

    return make_tuple(cmin, second_cmin, dmin);
}

int readInput(list<dVector*>& dataList, const string& filename) {
    //cout << "filename = " << filename;
    ifstream in(filename);

    if (!in) {
        cout << "Cannot open input file.\n";
        return 0;
    }
    string str;
    int count = 0;

    while (getline(in, str)) {
        dVector *pValue;
        pValue = dVector::readVector(str, count++);
        dataList.push_back(pValue);
        if (pValue->getVector().size()!=VECTOR_SIZE) exit(1);
    }

    in.close();
    return 1;
}

int readConfig(const string& filename, unsigned int& clusters, unsigned int& hash_functions, unsigned int& hash_tables) {
    //cout << "filename = " << filename;

    ifstream in(filename);
    if(!in) {
        cout << "Cannot open input file.\n";
        return 0;
    }

    string word1, word2;

    for (int i=0; i<3; i++) {
        in >> word1 >> word2;
        //cout << word1 << word2 << endl;
        if (word1 == "number_of_clusters:") clusters = (unsigned int)stoi(word2);
        else if (word1 == "number_of_hash_functions:") hash_functions = (unsigned int)stoi(word2);
        else if (word1 == "number_of_hash_tables:") hash_tables = (unsigned int)stoi(word2);
        else cerr << "unknown configuration value: " << word1 << endl;
    }
    in.close();
    return 1;
}


unsigned int readMetric (const string& metric) {
    if (metric=="euclidean") return EUCLIDEAN;
    else if (metric=="cosine") return COSINE;
    else {
        cerr << "Unknown metric " << metric << endl;
        return UNKNOWN_METRIC;
    }
}

void print_results (const string& out, Cluster clusters[], vector<dVector*>& dataVector, int clusters_num, long sec, int init, int assignment, int update, int metric, bool complete) {
    cout << "ovalue = " << out << endl;
    ofstream output;
    output.open (out);
    double silhouettes_sum[clusters_num];
    double total_sil_sum=0;
    if(!output) {
        cerr  << "Cannot open file.\n";
        return;
    }

    output << "Algorithm: I" << init << "A" << assignment << "U" << update << "\nMetric: ";
    if (metric==EUCLIDEAN) output << "Euclidean\n";
    else if (metric==COSINE) output << "Cosine\n";
    else return;


    for (int i=0; i<clusters_num; i++) {
        output << "CLUSTER-" << i+1 << "{\nsize: " << clusters[i].getItemsNum() << "\ncentroid: [ ";
        vector<double> center = clusters[i].getCenter();
        for (double v : center) output << v << ' ';
        output << "]}\n\n";
        silhouettes_sum[i] = clusters[i].silhouette_sum(dataVector, clusters, i, metric);
        cout << "sil_sum " << i << " = " << silhouettes_sum[i]/clusters[i].getItemsNum() << endl;
        total_sil_sum+=silhouettes_sum[i];
    }
    long minutes = sec / 60;
    cout << "Clustering time: " << minutes << " minutes and " << int(sec%60) << " seconds.\n";
    output << "Clustering time: " << minutes << " minutes and " << int(sec%60) << " seconds.\n";


    output << "Silhouette: [";
    output << silhouettes_sum[0]/clusters[0].getItemsNum();
    for (int i=0; i<clusters_num; i++) output << ", " << silhouettes_sum[i]/clusters[i].getItemsNum();
    output << "]\nTotal silhouette: " << total_sil_sum/dataVector.size();

    if (complete) {
        for (int i=0; i<clusters_num; i++) {
            output << "CLUSTER-" << i+1 << "{\n";
            set<int> vectors = clusters[i].getVectors();
            for (int v : vectors) {
                output << dataVector[v]->getName() << ", ";
            }
            output << "}\n\n";
        }
    }

    cout << "ev = " << total_sil_sum/dataVector.size() << endl;

    output.close();

}

double objective_function (vector<dVector*>& dataVector, Cluster clusters[], int metric) {
    double obj_value=0;
    for (auto vec : dataVector) {
        vector<double> center = clusters[vec->getCluster_num()].getCenter();
        vector<double> current_vec = vec->getVector();
        double dist=distance(center, current_vec, metric);
        obj_value+=dist*dist;
    }
    return obj_value;
}


void clustering_algorithms (int init, int assignment, int update, int k, int metric, vector<dVector*>& dataVector, const string& out, bool complete) {
    auto begin = chrono::high_resolution_clock::now();


    Cluster clusters[k];
    set<int> centers;

    //initializing
    switch (init) {
        case 0:
            random_init(centers, k, dataVector);
            break;
        case 1:
            kmeanspp_init(centers, dataVector, metric, k);
            break;
        default:
            cerr << "Unknown init command\n";
            return;
    }
    int cluster_num = 0;
    for (auto c : centers) {
        //cout << "center = " << c << endl;
        clusters[cluster_num].setCenter(dataVector[c]->getVector());
        cluster_num++;
    }

    //init lsh
    unordered_map<string, list<dVector *>> umap[L_DEFAULT];
    vector<hFunction> hF[L_DEFAULT];
    default_random_engine generator;
    for (int i = 0; i < L_DEFAULT; i++) hF[i] = hFunction::init_hFunctions(VECTOR_SIZE, K_DEFAULT, generator);

    if (!add_toHashTables(dataVector, K_DEFAULT, hF, umap, L_DEFAULT, metric)) {
        cerr << "ERROR in add to hashtables\n";
        return;
    }

    //init cube
    int kvalue = K_DEFAULT;
    //default_random_engine generator;
    if (kvalue <= 0) kvalue = (int) round(log10(dataVector.size()));
    int arraylen = int(pow(2, kvalue));

    list<dVector *> cube[arraylen];
    vector<hFunction> hF_cube;
    hF_cube = hFunction::init_hFunctions(VECTOR_SIZE, kvalue, generator);

    add_toCube(dataVector, kvalue, hF_cube, cube, metric);

    bool change;

    double current_obj = 0, previous_obj;

    do {
        if (assignment == 0) kmeans_assignment(dataVector, clusters, metric, k);
        else if (assignment == 1) lsh_range_assignment(umap, hF, generator, dataVector, clusters, metric, k);
        else if (assignment == 2) cube_range_assignment(cube, arraylen, hF_cube, generator, dataVector, clusters, metric, k);


        previous_obj = current_obj;
        current_obj = objective_function(dataVector, clusters, metric);

        double obj_fraction = previous_obj / current_obj;

        cout << obj_fraction << endl;

        if (previous_obj != 0) change = (obj_fraction > 1.001 || obj_fraction <1);
        else change = true;

        //update
        //update only if has been a change in clusters
        if (change) {
            for (int i = 0; i < k; i++) {
                switch (update) {
                    case 0:
                        clusters[i].updateCenter(dataVector);
                        break;
                    case 1:
                        clusters[i].updatePAM_Lloyds(dataVector, metric);
                        break;
                    default:
                        cerr << "Unknown init command\n";
                        return;
                }
            }
        }
    } while (change);

    auto end = chrono::high_resolution_clock::now();
    auto dur = end - begin;
    long sec = (chrono::duration_cast<std::chrono::seconds>(dur).count());

    //evaluation
    cout << "eimai prin tin silouet\n";
    print_results(out, clusters, dataVector, k, sec, init, assignment, update, metric, complete);

    //double ev = silhouette(dataVector, clusters, metric);
    //cout << "ev swsto = " << ev << endl;
}
