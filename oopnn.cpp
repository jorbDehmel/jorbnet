#include <iostream>
#include "resources.hpp"
#include "jgraph/jgraph.hpp"
using namespace std;

vector<pair<double, double>> dataToGraph;
int item = 0;
bool graph(double &X, double &Y)
{
    if (item >= dataToGraph.size())
    {
        item = 0;
        return false;
    }

    X = dataToGraph[item].first;
    Y = dataToGraph[item].second;

    item++;
    return true;
}

int main()
{
    // Trained to be a XOR and an AND gate
    dataset a;
    a.inputs = {0, 0};
    a.expected = {1, 0};

    dataset b;
    b.inputs = {1, 1};
    b.expected = {0, 1};

    srand(time(NULL));
    vector<int> sizes = {2, 2, 5, 2, 2};
    network n(sizes);
    n.trainingData = {a, b};

    n.train(10000);

    for (auto e : n.errors)
    {
        dataToGraph.push_back(pair<double, double>(dataToGraph.size(), e));
    }

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
    jgraph::XMIN = jgraph::YMIN = -10;
    jgraph::XMAX = dataToGraph.size() - 1;
    jgraph::YMAX = 5;
    jgraph::TICK_SPACING_X = dataToGraph.size() / 10;

    g.equations.push_back(graph);
    g.refresh();
    mainLoop(&g);

    return 0;
}
