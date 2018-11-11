#include <iostream>
#include <vector>
#ifndef INC_1_DVECTOR_H
#define INC_1_DVECTOR_H


class dVector {
private:
    std::string name;
    std::vector<double> p;
    std::vector<double> g;

public:
    dVector(const std::string given_name, const std::vector<double> given_p);
    void setG (const std::vector<double> given_g);
    std::vector<double> getG ();
    std::string getName() const;
    std::vector<double> getVector () const;
    static dVector* readVector (std::string str, unsigned long i);
};

#endif //INC_1_DVECTOR_H
