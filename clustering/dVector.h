//
// Created by mark on 19/11/2018.
//

#include <iostream>
#include <vector>
#ifndef INC_1_DVECTOR_H
#define INC_1_DVECTOR_H


class dVector {
private:
    std::string name;
    std::vector<double> p;
    int cluster_num;
    int second_best_cluster;
    //std::vector<double> g;

public:
    dVector(const std::string given_name, const std::vector<double> given_p);
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
    std::vector<double> getVector () const;
    static dVector* readVector (std::string str, unsigned long i);
};

#endif //INC_1_DVECTOR_H

