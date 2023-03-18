/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <iostream>
#include <map>
#include <cmath>
#include <vector>
#include <chrono>
#include <cassert>
#include "jgraph/jgraph.hpp"
using namespace std;

#define WEIGHT_VARIATION 5
#define MIN_STEP_SIZE 0.1
#define MAX_STEP_SIZE 1
#define LOG_INTERVAL 100

#define ERROR_BREAK 0.01

#define chrono_now() chrono::high_resolution_clock::now()
#define chrono_ns(A) chrono::duration_cast<chrono::nanoseconds>(A).count()

string toTime(long long Ns);

struct node
{
    double act;

    vector<node *> links;
    double *weights;

    double bias;
    int wIndex;
};

struct dataset
{
    vector<double> inputs;
    vector<double> expected;
};

double drand(const double &Min, const double &Max);

void initNode(node &out, node *Links, const int &NumLinks);
void freeNode(node *What);
void cloneNode(node &To, const node &From, node *Links, const int &NumLinks);

double wsum(const node &What);
double sig(const double &X);
double prop(node &What);

// Sum of squares, not norm
double err(node *Layer, double *Expected, const int &Size);
double err(vector<double> &Obs, vector<double> &Exp);
double mag(vector<double> &What);

// Get the derivative of a single node with respect to some weight 'For'
double bprop(node &What, double *For, map<node *, double> &FoundWeights, map<node *, double> &FoundGenerals);

#endif
