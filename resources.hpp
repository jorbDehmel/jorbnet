#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <iostream>
#include <map>
#include <cmath>
#include <vector>
using namespace std;

#define WEIGHT_VARIATION 5

struct node
{
    double act;
    map<node *, double> links;
    double bias;
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

#endif
