#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <iostream>
#include <map>
#include <cmath>
#include <vector>
#include <cassert>
using namespace std;

#define WEIGHT_VARIATION 5

struct node
{
    double act;
    map<node *, double> links;
    double bias;
};

struct dataset
{
    vector<double> inputs;
    vector<double> expected;
};

double drand(const double &Min, const double &Max)
{
    return Min + (Max - Min) * (double)rand() / RAND_MAX;
}

void initNode(node &out, node *Links, const int &NumLinks)
{
    out.act = 0;
    out.bias = drand(-WEIGHT_VARIATION, WEIGHT_VARIATION);
    for (int i = 0; i < NumLinks; i++)
    {
        out.links[&Links[i]] = drand(-WEIGHT_VARIATION, WEIGHT_VARIATION);
    }
    return;
}

double wsum(const node &What)
{
    double out = 0;
    for (auto i : What.links)
    {
        out += i.first->act * i.second;
    }
    out += What.bias;
    return out;
}

double sig(const double &X)
{
    return 1 / (1 + pow(M_E, -X));
}

double prop(node &What)
{
    What.act = sig(wsum(What));
    return What.act;
}

// Sum of squares, not norm
double err(node *Layer, double *Expected, const int &Size)
{
    double out = 0;
    for (int i = 0; i < Size; i++)
    {
        out += pow(Expected[i] - Layer[i].act, 2);
    }
    return out;
}

double err(vector<double> &Obs, vector<double> &Exp)
{
    double out = 0;
    for (int i = 0; i < Obs.size(); i++)
    {
        out += pow(Exp[i] - Obs[i], 2);
    }
    return out;
}

class network
{
public:
    network(const vector<int> &Sizes);
    ~network();

    vector<double> propogate(vector<double> &Input);
    void backProp();

    vector<int> sizes;
    node **nodes;
    vector<dataset> trainingData;
};

network::network(const vector<int> &Sizes)
{
    for (auto i : Sizes)
    {
        sizes.push_back(i);
    }

    // Create network
    nodes = new node *[sizes.size()];
    assert(nodes != nullptr);
    for (int i = 0; i < sizes.size(); i++)
    {
        nodes[i] = new node[sizes[i]];
        assert(nodes[i] != nullptr);
    }

    // Link nodes
    for (int i = 1; i < sizes.size(); i++)
    {
        for (int j = 0; j < sizes[i]; j++)
        {
            initNode(nodes[i][j], nodes[i - 1], sizes[i - 1]);
        }
    }

    return;
}

network::~network()
{
    // Delete network
    for (int i = 0; i < sizes.size(); i++)
    {
        delete[] nodes[i];
    }
    delete[] nodes;
    return;
}

vector<double> network::propogate(vector<double> &Input)
{
    // Assert sizes match
    if (Input.size() != sizes[0])
    {
        throw runtime_error("Dimensional mismatch: Cannot propogate");
    }

    // Write input to input layer activations
    for (int i = 0; i < Input.size(); i++)
    {
        cout << i << '\t' << Input[i] << '\n';
        nodes[0][i].act = Input[i];
    }

    // Propogate all layers but the first
    for (int layer = 1; layer < sizes.size(); layer++)
    {
        for (int i = 0; i < sizes[i]; i++)
        {
            prop(nodes[layer][i]);
        }
    }

    // Copy output activations to output vector
    vector<double> out;
    for (int i = 0; i < sizes.back(); i++)
    {
        out.push_back(nodes[sizes.size() - 1][i].act);
    }
    return out;
}

#endif
