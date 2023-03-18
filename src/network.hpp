/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "resources.hpp"

class network
{
public:
    network(const vector<int> &Sizes);
    network(const network &Other);
    ~network();

    vector<double> propogate(vector<double> &Input);
    void backprop(const int Index = -1);

    void train(const int NumTimes);

    vector<int> sizes;
    node **nodes;
    vector<dataset> trainingData;
    vector<double *> weights;

    vector<double> errors;
    int passes = 0;
};

// This will halt execution until the graph is closed
void graphNetworkError(const network &What);

extern vector<double> __graphData;
extern int __graphItem;
bool __graphErrorHelper(double &X, double &Y);

#endif
