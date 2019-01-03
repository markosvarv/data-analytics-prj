#include <iostream>
#include <vector>
#ifndef RECOMENDATION_DVECTOR_H
#define RECOMENDATION_DVECTOR_H


class dVector {
private:
    std::string name;
    std::vector<double> p;
    int cluster_num;
    int second_best_cluster;

    int id;
    bool assigned;

    double cluster_dist;
    double second_cluster_dist;

    //std::vector<double> g;

public:
    dVector(const std::string given_name, const std::vector<double> given_p, int given_id);
    //void setG (const std::vector<double> given_g);
    //std::vector<double> getG ();
    std::string getName() const;
    void setCluster_num (int cluster) {
        cluster_num=cluster;
    }
    int getCluster_num (void) {
        return cluster_num;
    }
    void setSecond_best_cluster (int cluster) {
        second_best_cluster = cluster;
    }
    int getSecond_best_cluster (void) {
        return second_best_cluster;
    }
    int getID (void) {
        return id;
    }

    void set_assigned(bool given_bool) {
        assigned = given_bool;
    }

    void set_cluster_dist (double given_dist) {
        cluster_dist = given_dist;
    }

    void set_second_cluster_dist (double given_dist) {
        second_cluster_dist = given_dist;
    }

    double get_cluster_dist (void) {
        return cluster_dist;
    }

    double get_second_cluster_dist (void) {
        return second_cluster_dist;
    }

    bool is_assigned(void) {
        return assigned;
    }

    std::vector<double> getVector () const;
    static dVector* readVector (std::string str);
};

#endif //RECOMENDATION_DVECTOR_H
