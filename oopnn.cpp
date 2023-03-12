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

    n.train(10000);

    double min = 1000000;
    double max = -1;
    for (auto e : n.errors)
    {
        if (e < min)
        {
            min = e;
        }
        if (e > max)
        {
            max = e;
        }

        dataToGraph.push_back(e);
    }
    cout << "Minimal error: " << min << '\n'
         << "Maximal error: " << max << '\n';

    for (auto data : n.trainingData)
    {
        auto observed = n.propogate(data.inputs);
        for (int i = 0; i < observed.size(); i++)
        {
            cout << observed[i] << " (" << (int)(observed[i] + 0.5) << ") (r " << data.expected[i] << ")\t";
        }
        cout << '\n';
    }

    LineGraph g;

    jgraph::UPSCALING_X = jgraph::UPSCALING_Y = 2;
    jgraph::XMIN = jgraph::YMIN = -1;
    jgraph::XMAX = dataToGraph.size() - 1;
    jgraph::YMAX = 5;
    jgraph::TICK_SPACING_X = dataToGraph.size() / 10;

    g.equations.push_back(graph);
    g.refresh();
    mainLoop(&g);

    saveNetwork("hi.nn", n);

    return 0;
}
