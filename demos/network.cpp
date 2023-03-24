/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include <iostream>
#include "../src/jorbNet.hpp"
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
    b.output = {0, 1};

    dataset c;
    c.input = {1, 0};
    c.output = {0, 1};

    dataset d;
    d.input = {1, 1};
    d.output = {1, 0};

    vector<int> sizes = {2, 5, 2};

    Network n(sizes);

    n.trainingData = {a, b, c, d};

    auto start = chrono::high_resolution_clock::now();
    n.train(1000);
    auto end = chrono::high_resolution_clock::now();

    int ellapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    double lastError = n.getError();
    double errorNs = ellapsed * lastError;

    cout << "Ellapsed ns: " << ellapsed << '\n'
         << "Final error: " << lastError << " (" << (int)(lastError + 0.5) << ")\n"
         << "Network error-NS evaluation: " << errorNs << '\n';

    return 0;
}
