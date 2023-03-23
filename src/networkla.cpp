/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include "networkla.hpp"
using namespace std;

double drand(const double &Min, const double &Max)
{
    return Min + (Max - Min) * rand() / ((double)RAND_MAX + 1);
}

double __sigmoid(const double &X)
{
    return 1 / (1 + exp(-X));
}

double __sigder(const double &Act)
{
    return Act * (1 - Act);
}

double __ReLU(const double &X)
{
    if (X > 0)
    {
        return X;
    }
    else
    {
        return 0;
    }
}

double __ReLUder(const double &Act)
{
    if (Act > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

NetworkLA::NetworkLA(const vector<int> &Sizes)
{
    // Construct sizes list
    numLayers = Sizes.size();
    sizes = SafeArray<int>(numLayers);
    for (int i = 0; i < numLayers; i++)
    {
        sizes[i] = Sizes[i];
    }

    // Construct the first dimensions of activations and weights
    activations = SafeArray<SafeArray<double>>(numLayers);
    weights = SafeArray<SafeArray<SafeArray<double>>>(numLayers - 1);

    // Construct the second dimensions of activations and weights
    for (int layer = 0; layer < numLayers; layer++)
    {
        activations[layer] = SafeArray<double>(sizes[layer]);

        if (layer + 1 != numLayers)
        {
            weights[layer] = SafeArray<SafeArray<double>>(sizes[layer + 1]);
        }
    }

    // Construct the third dimension of weights
    for (int layer = 0; layer + 1 < numLayers; layer++)
    {
        for (int node = 0; node < sizes[layer + 1]; node++)
        {
            weights[layer][node] = SafeArray<double>(sizes[layer] + 1);

            for (int weight = 0; weight < sizes[layer] + 1; weight++)
            {
                weights[layer][node][weight] = drand(-WV, WV);
            }
        }
    }

    return;
}

vector<double> NetworkLA::prop(const vector<double> &Input)
{
    // Ensure input vector matches network size
    if (Input.size() != sizes[0])
    {
        throw runtime_error("Error: Input size does not match network size");
    }

    // Load into inputs
    for (int i = 0; i < Input.size(); i++)
    {
        activations[0][i] = Input[i];
    }

    // Propogate
    for (int layer = 1; layer < numLayers; layer++)
    {
        for (int node = 0; node < sizes[layer]; node++)
        {
            activations[layer][node] = bdot(activations[layer - 1], weights[layer - 1][node], sizes[layer - 1]);
            activations[layer][node] = act(activations[layer][node]);
        }
    }

    // Return output layer
    vector<double> out;
    for (int i = 0; i < sizes[numLayers - 1]; i++)
    {
        out.push_back(activations[numLayers - 1][i]);
    }

    return out;
}

double NetworkLA::backprop(const vector<double> &Expected)
{
    throw runtime_error("Bcakprop is unimplemented.");
    return -1;
}

double NetworkLA::bdot(SafeArray<double> Inputs, SafeArray<double> Weights, const int &SizeOfInputs) const
{
    double out = Weights[SizeOfInputs];
    for (int i = 0; i < SizeOfInputs; i++)
    {
        if (Weights[i] && Inputs[i])
        {
            out += Inputs[i] * Weights[i];
        }
    }

    return out;
}

#include <chrono>
int main()
{
    srand(time(NULL));

    vector<int> sizes = {3, 5, 10, 2, 5};
    NetworkLA n(sizes);
    n.act = __ReLU;
    n.actder = __ReLUder;

    vector<double> inp = {1, 2, 3};

    auto start = chrono::high_resolution_clock::now();

    auto out = n.prop(inp);

    auto end = chrono::high_resolution_clock::now();
    int elapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    cout << "Propogation took " << elapsed << " ns.\n";

    for (auto i : out)
    {
        cout << i << '\n';
    }

    return 0;
}
