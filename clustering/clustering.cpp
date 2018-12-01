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

using namespace std;

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

double silhouette (vector<dVector*> dataVector, Cluster clusters[], int metric) {
    unsigned int obj_num=0;
    double sil_sum = 0;

    //for every vector compute the average distance from its cluster
    //and the average distance from the second best cluster
    for (auto vector : dataVector) {
        int cluster = vector->getCluster_num();
        int second_best_cluster = vector->getSecond_best_cluster();

        //cout << "cluster = " << cluster << "\tsecond_best = " << second_best_cluster << endl;

        double ai=0;
        //cout << "eimai prin to if\n";
        if (clusters[cluster].getItemsNum()>1)
            ai = clusters[cluster].vectorDistanceSum(dataVector, obj_num, metric)/(clusters[cluster].getItemsNum()-1); //-1 because cluster contains the object
        double bi = clusters[second_best_cluster].vectorDistanceSum(dataVector, obj_num, metric)/clusters[second_best_cluster].getItemsNum();
        //cout << "eimai meta to if\n";

        double max;
        ai > bi ? max=ai : max=bi;
        sil_sum += (bi-ai)/max;
        obj_num++;
    }
    return sil_sum/dataVector.size();
}

//returns the the min distance of dvec_num point to some centroid
//returns 0 if dvec_num point is a center
double di (int i, set<int>& centers, vector<dVector*>& dataVector, int metric) {
    vector<double> current_vec = dataVector[i]->getVector();
    double di = numeric_limits<double>::max();

    for (auto center_num : centers) {
        vector<double> center = dataVector[center_num]->getVector();

        double t = distance(current_vec, center, metric);
        if (t<=di) {
            //cmin = i;
            di = t;
        }
    }
    return di;

}

double pr (int r, set<int>& centers, vector<dVector*>& dataVector, int metric) {
    double sum=0;
    for (int i=0; i<=r; i++) {
        if (centers.find(i) != centers.end()) continue; //if dvec is a centroid go to next dvec
        double current_di=di(i, centers, dataVector, metric);
        sum+=current_di*current_di;
    }
    return sum;
}

int kmeanspp_init (set<int>& centers, vector<dVector*>& dataVector, int metric, unsigned int k) {
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    int data_size = (int)dataVector.size();
    uniform_int_distribution<> center_dis(0, data_size-1);
    centers.insert(center_dis(gen)); //choose first centroid uniformly at random

    while (centers.size()<k) {
        double max_pr = pr(data_size-1, centers, dataVector, metric);

        uniform_real_distribution<> x_dis(0, max_pr);
        double x = x_dis(gen); //pick a uniformly distributed float at [0,max_pr]

        unsigned int dvec_num;
        double current_pr = 0; //P(0)=0
        //end the for loop when x<=current_pr and keep this dvec_num
        for (dvec_num = 0; current_pr < x && dvec_num < dataVector.size(); dvec_num++) {
            if (centers.find(dvec_num) != centers.end()) continue; //if dvec is a centroid go to next dvec
            double current_di = di(dvec_num, centers, dataVector, metric);
            current_pr += current_di * current_di;
        }
        centers.insert(dvec_num);
    }
    return 1;
}


bool kmeans_assignment (vector<dVector*>& dataVector, Cluster clusters[], int metric, int k) {
    bool change = false;
    int obj_num=0;

    for (auto vec : dataVector) {

        int cluster_num=vec->getCluster_num();

        double distance;
        int new_cluster_num, second_best;
        tie(new_cluster_num, second_best, distance) = getNearestCluster(vec->getVector(), clusters, metric, k);

        //cout << distance << endl;

        if (cluster_num!=new_cluster_num) {
            //if cluster has changed erase vector from previous cluster and add it to the new cluster
            change=true;
            if (cluster_num!=-1) clusters[cluster_num].eraseVector(obj_num);
            clusters[new_cluster_num].addObjToCluster(obj_num);
            vec->setCluster_num(new_cluster_num);
        }
        vec->setSecond_best_cluster(second_best);
        obj_num++;
    }
    return change;
}

bool range_assignment (unordered_map<string, list<dVector *>> umap[], vector<hFunction> hF[], default_random_engine& generator,
        vector<dVector*>& dataVector, Cluster clusters[], int metric, int k) {

    int obj_num=0;
    int oldClusters[dataVector.size()];
    for (auto vec : dataVector) {
        oldClusters[obj_num] = vec->getCluster_num();
        obj_num++;
        vec->set_assigned(false);
    }

    //compute min distance between centers
    double dmin = numeric_limits<double>::max();
    for (int i=0; i<k; i++) {
        vector<double> vec_i = clusters[i].getCenter();
        for (int j=0; j<k; j++) {
            if (i>=j) continue; //the centers matrix is symmetric
            vector<double> vec_j = clusters[j].getCenter();
            double t = distance (vec_i, vec_j, metric);
            if (t<dmin) dmin=t;
        }
    }

    double range_dist = dmin/2;

    cout << "range_dist = " << range_dist << endl;

    //for every cluster

    //set<int> idset[k];

    long total_assignments_num;
    do {
        total_assignments_num = 0;
        for (int i = 0; i < k; i++) {
            //for every center make range queries
            vector<double> qValue = clusters[i].getCenter();

            int assignments_num = rangeSearch(qValue, i, umap, hF, K_DEFAULT, L_DEFAULT, range_dist, metric);
            cout << assignments_num << endl;
            if (assignments_num == -1) {
                cout << "ERROR in assignment\n";
                return false;
            }
            total_assignments_num += assignments_num;
        }
        range_dist *= 2;
        cout << "TOTAL:: " << total_assignments_num << endl;
    }while (total_assignments_num!=0 || range_dist<=2*dmin);



    bool change = false;
    obj_num=0;
    int count = 0;
    for (auto vec : dataVector) {
        //assign all unassigned vectors
        if (!vec->is_assigned()) {
            cout << "mphka sto if\n";
            double dist;
            int new_cluster_num, second_best;
            tie(new_cluster_num, second_best, dist) = getNearestCluster(vec->getVector(), clusters, metric, k);
            //cout << dist << endl;

            vec->setCluster_num(new_cluster_num);
            vec->setSecond_best_cluster(second_best);
            vec->set_cluster_dist(dist);
            count++;
        }
        if (vec->getSecond_best_cluster()==-1) {
            cout << "mphka sto second best\n";
            vec->setSecond_best_cluster(vec->getCluster_num());
        }

        int new_cluster_num=vec->getCluster_num();
        //cout << "second_cluster_num = " << vec->getSecond_best_cluster() << endl;

        if (oldClusters[obj_num]!=new_cluster_num) {
            //if cluster has changed erase vector from previous cluster and add it to the new cluster
            cout << "CHANGE!\n";
            change=true;
            if (oldClusters[obj_num]!=-1) clusters[new_cluster_num].eraseVector(obj_num);
            clusters[new_cluster_num].addObjToCluster(obj_num);
        }
        obj_num++;
    }
    cout << "count = " << count << endl;


    return change;
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
    //printf("ivalue = %s; cvalue=%s; ovalue=%s, dvalue=%s\n", ivalue, cvalue, ovalue, dvalue);
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

    //initializing
    Cluster clusters[k];
    set<int> centers;


    while (centers.size() < k) {
        long num = rand() % dataVector.size();
        centers.insert(num);
    }

    //kmeanspp_init(centers, dataVector, metric, k);


    int cluster_num=0;
    for(auto c : centers) {
        cout << "center = " << c << endl;
        clusters[cluster_num].setCenter(dataVector[c]->getVector());
        cluster_num++;
    }


    unordered_map<string, list<dVector *>> umap[L_DEFAULT];
    vector<hFunction> hF[L_DEFAULT];
    default_random_engine generator;

    for (int i = 0; i < L_DEFAULT; i++) {
        hF[i] = hFunction::init_hFunctions(VECTOR_SIZE, K_DEFAULT, generator);
    }

    if (!add_toHashTables (dataVector, K_DEFAULT, hF, umap, L_DEFAULT, metric)){
        cerr << "ERROR in add to hashtables\n";
        return 1;
    }

    bool change;
    do {
        //assignment
        change = range_assignment(umap, hF, generator, dataVector, clusters, metric, k);


        //change = kmeans_assignment(dataVector, clusters, metric, k);
        cout << change << endl;

        //update
        //update only if has been a change in clusters
        if (change) {
            for (unsigned int i = 0; i < k; i++) {
                clusters[i].updateCenter(dataVector);
                //clusters[i].updatePAM_Lloyds(dataVector, metric);
            }
        }
    }while(change);

    //evaluation
    cout << "eimai prin tin silouet\n";
    double ev = silhouette(dataVector, clusters, metric);
    cout << "ev = " << ev << endl;


    for (auto i = dataVector.begin(); i != dataVector.end(); i++) delete (*i);
}