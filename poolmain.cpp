#include "jgraph/jgraph.hpp"
#include "npool.hpp"
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
    cout << "Loading network...\n";

    network n = loadNetwork("hi.nn");

    cout << "Loaded. Cloning...\n";
    npool p(n, 14, 7, 100);
    cout << "Initialized clone pool. Training...\n";

    p.train(500);
    cout << "Trained.\n";

    network best = p.best();

    for (auto e : best.errors)
    {
        dataToGraph.push_back(e);
    }
    cout << '\n';

    LineGraph g;

    jgraph::UPSCALING_X = jgraph::UPSCALING_Y = 2;
    jgraph::XMIN = jgraph::YMIN = -1;
    jgraph::XMAX = dataToGraph.size() - 1;
    jgraph::YMAX = 5;
    jgraph::TICK_SPACING_X = dataToGraph.size() / 10;

    g.equations.push_back(graph);
    g.refresh();
    mainLoop(&g);

    return 0;
}
