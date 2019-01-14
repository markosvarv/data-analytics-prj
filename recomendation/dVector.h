#include <iostream>
#include <vector>
#ifndef RECOMENDATION_DVECTOR_H
#define RECOMENDATION_DVECTOR_H


class dVector {
private:
    std::vector<double> p;
    std::vector<bool> known_elements;
    int cluster_num;
    int second_best_cluster;
    int id;

public:
    dVector(const std::vector<double> given_p, int given_id);

    bool hasInfo () const;

    void setUnknownElements ();

    double getAverage() const;


    void setCluster_num (int cluster) {
        cluster_num=cluster;
    }
    int getCluster_num (void) {
        return cluster_num;
    }

    int getID (void) {
        return id;
    }

    std::vector<double> getVector () const;

    void setSecond_best_cluster (int sec_best) {
        second_best_cluster = sec_best;
    }

    int getSecond_best_cluster () const {
        return second_best_cluster;
    }

    std::vector<bool> getKnownElements () const {
        return known_elements;
    }

    static dVector* readVector (std::string str);
};

double euclideanDistance (const std::vector<double>& v1, const std::vector<double> &v2);
double cosineDistance (const std::vector<double>& v1, const std::vector<double>& v2);
double magnitude (const std::vector<double>& v);
double distance (const std::vector<double>& v1, const std::vector<double>& v2, int metric);

#endif //RECOMENDATION_DVECTOR_H
