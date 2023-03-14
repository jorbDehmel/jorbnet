#include <iostream>
#include "resources.hpp"
#include "jgraph/jgraph.hpp"
#include "networkSave.hpp"
using namespace std;

vector<double> dataToGraph;
int item = 0;
bool graph(double &X, double &Y)
{
    // Failsafe
    if (dataToGraph.size() <= 1)
    {
        X = Y = 0;
        return false;
    }

    if (item >= dataToGraph.size())
    {
        item = 0;
        return false;
    }

    X = item;
    Y = dataToGraph[item];

    item++;
    return true;
}

int main()
{
    srand(time(NULL));

    // Trained to be a XOR and an AND gate
    dataset a;
    a.inputs = {0, 0};
    a.expected = {1, 0};

    dataset b;
    b.inputs = {0, 1};
    b.expected = {0, 0};

    dataset c;
    c.inputs = {1, 0};
    c.expected = {0, 0};

    dataset d;
    d.inputs = {1, 1};
    d.expected = {1, 1};

    vector<int> sizes = {2, 10, 2};

    network n(sizes);

    n.trainingData = {a, b, c, d};

    auto start = chrono::high_resolution_clock::now();
    n.train(1000);
    auto end = chrono::high_resolution_clock::now();

    int ellapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    double lastError = n.errors.back();
    double errorNs = ellapsed * lastError;

    cout << "Ellapsed ns: " << ellapsed << '\n'
         << "Final error: " << lastError << '\n'
         << "Network error-NS evaluation: " << errorNs << '\n';

    // graphNetworkError(n);
    // saveNetwork("hi.nn", n);

    return 0;
}
