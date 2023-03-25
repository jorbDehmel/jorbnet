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
#include <chrono>

#include "safeArray.hpp"
using namespace std;

// Convenience macros build on chrono
#define chrono_now() chrono::high_resolution_clock::now()
#define chrono_ns(A) chrono::duration_cast<chrono::nanoseconds>(A).count()

// Get a string representation of an amount of nanoseconds
string toTime(long long Ns);

// Get a random double between Min and Max
double drand(const double &Min, const double &Max);

// Default activation function and derivative (given activation)
double __sigmoid(const double &X);
double __sigder(const double &Act);

// Rectifier activation function and derivative (given activation)
double __ReLU(const double &X);
double __ReLUder(const double &Act);

// Linear activation function and derivative (not that useful)
double __lin(const double &X);
double __linder(const double &Act);

// Helper functions for linear algebra
double dot(const SafeArray<double> &A, const SafeArray<double> &B);
void dotEquals(SafeArray<double> &What, const SafeArray<SafeArray<double>> &By);

// A training dataset
struct dataset
{
    vector<double> input;
    vector<double> output;
};

// Network using linear algebra
class Network
{
public:
    // Construct a linear-algebra based network with the given dimensions
    Network(const vector<int> &Sizes);

    // For cloning
    Network(const Network &Other);
    Network &operator=(const Network &Other);

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
    vector<double *> getWeights() const;

    // Training metavariables
    double minStepSize = 0.01, maxStepSize = 5, weightVariance = 5;

    // Data
    int passes = 0;

    // Friend functions
    friend void saveNetwork(ostream &Stream, Network &From);
    friend Network loadNetwork(istream &Stream);
    friend class NPool;

protected:
    // Internal data structure containing network data
    int numLayers = 0;
    SafeArray<int> sizes;
    SafeArray<SafeArray<double>> activations;

    SafeArray<SafeArray<SafeArray<double>>> weights;
    SafeArray<SafeArray<SafeArray<double>>> weightDeltas;
};

#endif
