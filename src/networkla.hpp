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
#define MIN_STEP_SIZE 0.001
#define MAX_STEP_SIZE 0.5

// Get a random double between Min and Max
double drand(const double &Min, const double &Max);

// Default activation function and derivative (given activation)
double __sigmoid(const double &X);
double __sigder(const double &Act);

// Rectifier activation function and derivative (given activation)
double __ReLU(const double &X);
double __ReLUder(const double &Act);

double dot(const SafeArray<double> &A, const SafeArray<double> &B);
void dotEquals(SafeArray<double> &What, const SafeArray<SafeArray<double>> &By);

// A training dataset
struct dataset
{
    vector<double> input;
    vector<double> output;
};

// Network using linear algebra
class NetworkLA
{
public:
    // Construct a linear-algebra based network with the given dimensions
    NetworkLA(const vector<int> &Sizes);

    // Basic propogation and backpropogation functions
    vector<double> prop(const vector<double> &Input);
    void backprop(const vector<double> &Expected);

    // Assumes weights is one item longer, with the final entry being bias
    double bdot(SafeArray<double> Inputs, SafeArray<double> Weights, const int &SizeOfInputs) const;

    // Current activation function and its derivative (these can be swapped out)
    double (*act)(const double &X) = __sigmoid;
    double (*actder)(const double &X) = __sigder;

    // Training stuff for convenience
    vector<dataset> trainingData;
    void train(const int &Num);
    double getError();

protected:
    // Internal data structure containing network data
    int numLayers = 0;
    SafeArray<int> sizes;
    SafeArray<SafeArray<double>> activations;

    SafeArray<SafeArray<SafeArray<double>>> weights;
    SafeArray<SafeArray<SafeArray<double>>> weightDeltas;
};

#endif
