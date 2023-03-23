/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#ifndef NETWORKLA_HPP
#define NETWORKLA_HPP

#include <vector>
#include <cmath>
#include <iostream>
#include <cassert>

#include "misc/safeArray.hpp"

using namespace std;

#define WV 5

double drand(const double &Min, const double &Max);

double __sigmoid(const double &X);
double __sigder(const double &X);

// Network using linear algebra
class NetworkLA
{
public:
    NetworkLA(const vector<int> &Sizes);
    ~NetworkLA();

    vector<double> prop(const vector<double> &Input);
    double backprop(const vector<double> &Expected);

    double dot(const double *A, const double *B, const int &Size) const;

    // Assumes weights is one item longer, with the final entry being bias
    double bdot(SafeArray<double> Inputs, SafeArray<double> Weights, const int &SizeOfInputs) const;

    double (*act)(const double &X) = __sigmoid;
    double (*actder)(const double &X) = __sigder;

protected:
    int numLayers = 0;

    SafeArray<int> sizes;
    SafeArray<SafeArray<double>> activations;
    SafeArray<SafeArray<SafeArray<double>>> weights;

    // int *sizes = nullptr;
    // double **activations = nullptr;
    // double ***weights = nullptr;
};

#endif
