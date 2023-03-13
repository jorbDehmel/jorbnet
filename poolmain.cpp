#include "npool.hpp"
using namespace std;

int main()
{
    cout << "Loading network...\n";

    network n({1, 2, 3});

    dataset a;
    a.inputs = {123};
    a.expected = {1, 2, 3};
    dataset b;
    b.inputs = {321};
    b.expected = {3, 2, 1};
    n.trainingData = {a, b};

    cout << "Loaded. Cloning...\n";
    npool p(n, 14, 7, 100);
    cout << "Initialized clone pool. Training...\n";

    p.train(1000);
    cout << "Trained.\n";

    return 0;
}
