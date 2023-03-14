/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include "jgraph/jgraph.hpp"
#include "npool.hpp"
#include <chrono>
#include <iostream>
using namespace std;

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

    npool p(n, 10, 7, 10);

    auto start = chrono::high_resolution_clock::now();
    p.train(1000);
    auto end = chrono::high_resolution_clock::now();

    long long ellapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    double lastError = p.best().errors.back();
    double errorNs = ellapsed * lastError;

    cout << "Ellapsed ns: " << ellapsed << '\n'
         << "Final error: " << lastError << '\n'
         << "Network error-NS evaluation: " << errorNs << '\n';

    // graphNetworkError(p.best());

    return 0;
}
