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

NetworkLA::NetworkLA(const vector<int> &Sizes)
{
    assert(!Sizes.empty());

    numLayers = Sizes.size();

    sizes = new int[numLayers];
    for (int i = 0; i < numLayers; i++)
    {
        sizes[i] = Sizes[i];
    }

    activations = new double *[numLayers];
    weights = new double **[numLayers - 1];

    assert(sizes != nullptr);
    assert(activations != nullptr);
    assert(weights != nullptr);

    for (int layer = 0; layer < numLayers; layer++)
    {
        activations[layer] = new double[sizes[layer]];
        assert(activations[layer] != nullptr);

        if (layer + 1 != numLayers)
        {
            weights[layer] = new double *[sizes[layer]];
            assert(weights[layer] != nullptr);
        }
    }

    for (int layer = 0; layer + 1 < numLayers; layer++)
    {
        for (int node = 0; node < sizes[layer + 1]; node++)
        {
            weights[layer][node] = new double[sizes[layer] + 1];
            assert(weights[layer][node] != nullptr);

            // Initialize weights and bias to random values
            for (int weight = 0; weight < sizes[layer] + 1; weight++)
            {
                weights[layer][node][weight] = drand(-WV, WV);
            }
        }
    }

    return;
}

NetworkLA::~NetworkLA()
{
    for (int layer = 0; layer + 1 < numLayers; layer++)
    {
        for (int node = 0; node < sizes[layer + 1]; node++)
        {
            delete[] weights[layer][node];
        }
        delete[] weights[layer];
    }
    delete[] weights;
    delete[] sizes;

    for (int layer = 0; layer < numLayers; layer++)
    {
        delete[] activations[layer];
    }
    delete[] activations;

    return;
}

vector<double> NetworkLA::prop(const vector<double> &Input)
{
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
    return 0;
}

double NetworkLA::dot(const double *A, const double *B, const int &Size) const
{
    double out = 0;
    for (int i = 0; i < Size; i++)
    {
        out += A[i] * B[i];
    }
    return out;
}

double NetworkLA::bdot(const double *Inputs, const double *Weights, const int &SizeOfInputs) const
{
    double out = Weights[SizeOfInputs];
    for (int i = 0; i < SizeOfInputs; i++)
    {
        out += Inputs[i] * Weights[i];
    }

    return out;
}

int main()
{
    srand(time(NULL));

    vector<int> sizes = {3, 4, 1};
    NetworkLA n(sizes);

    vector<double> inp = {1, 2, 3};
    auto out = n.prop(inp);

    for (auto i : out)
    {
        cout << i << '\n';
    }

    return 0;
}
