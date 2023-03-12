#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <iostream>
#include <map>
#include <cmath>
#include <vector>
#include <chrono>
#include <cassert>
using namespace std;

#define WEIGHT_VARIATION 5
#define MIN_STEP_SIZE 0.01
#define MAX_STEP_SIZE 0.5
#define LOG_INTERVAL 100

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

double wsum(const node &What);
double sig(const double &X);
double prop(node &What);

// Sum of squares, not norm
double err(node *Layer, double *Expected, const int &Size);
double err(vector<double> &Obs, vector<double> &Exp);
double mag(vector<double> &What);

// Get the derivative of a single node with respect to some weight 'For'
double bprop(node &What, double *For, map<node *, double> &Found);

class network
{
public:
    network(const vector<int> &Sizes);
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

#endif
