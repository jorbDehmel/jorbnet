/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include "../src/jorbNet.hpp"
#include <chrono>
#include <iostream>
using namespace std;

int main()
{
    srand(time(NULL));

    // Trained to be a XOR and an AND gate
    dataset a;
    a.input = {0, 0};
    a.output = {1, 0};

    dataset b;
    b.input = {0, 1};
    b.output = {0, 0};

    dataset c;
    c.input = {1, 0};
    c.output = {0, 0};

    dataset d;
    d.input = {1, 1};
    d.output = {1, 1};

    vector<int> sizes = {2, 10, 2};
    Network n(sizes);
    n.trainingData = {a, b, c, d};

    NPool p(n, 10, 7, 10);

    auto start = chrono::high_resolution_clock::now();
    p.train(1000);
    auto end = chrono::high_resolution_clock::now();

    long long ellapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    double lastError = p.best().getError();
    double errorNs = ellapsed * lastError;

    cout << "Ellapsed ns: " << ellapsed << '\n'
         << "Final error: " << lastError << '\n'
         << "Network error-NS evaluation: " << errorNs << '\n';

    // graphNetworkError(p.best());

    return 0;
}
