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

    sizes = SafeArray<int>(numLayers);
    for (int i = 0; i < numLayers; i++)
    {
        sizes[i] = Sizes[i];
    }

    activations = SafeArray<SafeArray<double>>(numLayers);
    weights = SafeArray<SafeArray<SafeArray<double>>>(numLayers - 1);

    for (int layer = 0; layer < numLayers; layer++)
    {
        activations[layer] = SafeArray<double>(sizes[layer]);

        if (layer + 1 != numLayers)
        {
            weights[layer] = SafeArray<SafeArray<double>>(sizes[layer]);
        }
    }

    cout << "Populating and constructing weights...\n";

    for (int layer = 0; layer + 1 < numLayers; layer++)
    {
        for (int node = 0; node < sizes[layer + 1]; node++)
        {
            cout << "Creating " << layer << " " << node << "...\n";

            weights[layer][node] = SafeArray<double>(sizes[layer] + 1);

            for (int weight = 0; weight < sizes[layer] + 1; weight++)
            {
                cout << "Populating " << layer << " " << node << " " << weight << '\n';

                weights[layer][node][weight] = drand(-WV, WV);
            }
        }
    }

    cout << "Finished construction.\n";

    return;
}

NetworkLA::~NetworkLA()
{
    /*
    cout << "Beginning deallocation.\n";

    for (int layer = 0; layer + 1 < numLayers; layer++)
    {
        for (int node = 0; node < sizes[layer + 1]; node++)
        {
            delete[] weights[layer][node];
        }
        delete[] weights[layer];
    }
    delete[] weights;

    cout << "Deallocating activations...\n";
    for (int layer = 0; layer < numLayers; layer++)
    {
        cout << "Deallocating for layer " << layer << " w/ address " << activations[layer] << '\n';
        delete[] activations[layer];
        cout << "Dealloc'd.\n";
    }
    cout << "Finished deallocating activations.\n";
    delete[] activations;
    delete[] sizes;
    */

    return;
}

vector<double> NetworkLA::prop(const vector<double> &Input)
{
    if (Input.size() != sizes[0])
    {
        throw runtime_error("Error: Input size does not match network size");
    }

    cout << "Loading inputs...\n";

    // Load into inputs
    for (int i = 0; i < Input.size(); i++)
    {
        activations[0][i] = Input[i];
    }

    cout << "Propogating...\n";

    // Propogate
    for (int layer = 1; layer < numLayers; layer++)
    {
        for (int node = 0; node < sizes[layer]; node++)
        {
            cout << "on node at " << layer << " " << node << '\n';
            cout << "size of below: " << sizes[layer - 1] << '\n';
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

double NetworkLA::bdot(SafeArray<double> Inputs, SafeArray<double> Weights, const int &SizeOfInputs) const
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

    vector<int> sizes = {3, 5, 2};
    NetworkLA n(sizes);

    vector<double> inp = {1, 2, 3};
    auto out = n.prop(inp);

    for (auto i : out)
    {
        cout << i << '\n';
    }

    return 0;
}
