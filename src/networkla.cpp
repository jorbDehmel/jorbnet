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

double dot(const SafeArray<double> &A, const SafeArray<double> &B)
{
    assert(A.getSize() == B.getSize());
    double out = 0;
    for (int i = 0; i < A.getSize(); i++)
    {
        if (A[i] && B[i])
        {
            out += A[i] * B[i];
        }
    }
    return out;
}

// what := By * What
// What may change dimensions!
void dotEquals(SafeArray<double> &What, const SafeArray<SafeArray<double>> &By)
{
    cout << "Beginning dot product...\n";

    if (By[0].getSize() == What.getSize())
    {
        // Each entry in the vector becomes the dot product with the row of the matrix
        SafeArray<double> out(By.getSize());
        for (int row = 0; row < By.getSize(); row++)
        {
            out[row] = dot(By[row], What);
        }
        What = out;
    }

    // Automatically transpose if necessary (faster than taking an actual transpose)
    else if (By.getSize() == What.getSize())
    {
        SafeArray<double> out(By[0].getSize());
        for (int col = 0; col < By[0].getSize(); col++)
        {
            double sum = 0;
            for (int row = 0; row < By.getSize(); row++)
            {
                sum += What[row] * By[row][col];
            }
            out[col] = sum;
        }
        What = out;
    }
    else
    {
        throw runtime_error("Dimensional mismatch: Cannot take dot product \
                            or transposed dot product.");
    }

    cout << "Completed dot product.\n";

    return;
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
    weightDeltas = SafeArray<SafeArray<SafeArray<double>>>(numLayers - 1);

    // Construct the second dimensions of activations and weights
    for (int layer = 0; layer < numLayers; layer++)
    {
        activations[layer] = SafeArray<double>(sizes[layer]);

        if (layer + 1 != numLayers)
        {
            weights[layer] = SafeArray<SafeArray<double>>(sizes[layer + 1]);
            weightDeltas[layer] = SafeArray<SafeArray<double>>(sizes[layer + 1]);
        }
    }

    // Construct the third dimension of weights
    for (int layer = 0; layer + 1 < numLayers; layer++)
    {
        for (int node = 0; node < sizes[layer + 1]; node++)
        {
            weights[layer][node] = SafeArray<double>(sizes[layer] + 1);
            weightDeltas[layer][node] = SafeArray<double>(sizes[layer] + 1);

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

// Requires previous propogation
void NetworkLA::backprop(const vector<double> &Expected)
{
    // see wikipedia on backpropagation
    // ga^L := (f^L)' x \grad_{a^L}C
    // To start
    // Where x is hadamard product (element-wise)
    // C is error, a^L is the activation of the L'th layer
    // f^L is the activation function of the L'th layer
    // ga^{l-1} := (f^{l-1})' x (W^l)^T * ga^l
    // until l == 1

    // Base case
    // ga starts as the gradient of the error wrt final activations
    // we use sum((e - a)^2)

    // For expected e and observed a (for activation), we compute
    // cumulative error C as:
    // C := (e_0 - a_0)^2 + (e_1 - a_1)^2 + (e_2 - a_2)^2
    // Ergo the error partial derivative with respect to each act is:
    // wrt a_0:
    //      -2(e_0 - a_0) + (C - (e_0 - a_0)^2)
    // wrt a_1:
    //      -2(e_1 - a_1) + (C - (e_1 - a_1)^2)
    // generally:
    //      -2(e_i - a_i) + (C - (e_i - a_i)^2)

    // Amount to modify the weights by is
    // = dot(gradientAggregate, activationsOfLayerBelow)

    // Calculate cumulative error
    cout << "Getting cum error...\n"; // hehe
    double C = 0;
    for (int i = 0; i < sizes[numLayers - 1]; i++)
    {
        C += pow(Expected[i] - activations[numLayers - 1][i], 2);
    }

    // Set up gradient aggregate as gradient of errors wrt final acts
    // Then hadamard-ed by vector of sigder of previous final act

    cout << "Establishing GA...\n";
    SafeArray<double> gradientAggregate(Expected.size());
    for (int i = 0; i < gradientAggregate.getSize(); i++)
    {
        gradientAggregate[i] = -2 * (Expected[i] - activations[-1][i]) + C - pow(Expected[i] - activations[-1][i], 2);
        gradientAggregate[i] *= actder(activations[-1][i]);
    }

    // Iterate back through network
    cout << "Backpropogating...\n";
    for (int layer = numLayers - 2; layer >= 1; layer--)
    {
        cout << "Starting layer " << layer << '\n';

        cout << "GA size: " << gradientAggregate.getSize() << '\n'
             << "Weights[layer] size: h = " << weights[layer].getSize() << " w = " << weights[layer][0].getSize() << '\n';

        // Dot product equals by weights of above layer
        dotEquals(gradientAggregate, weights[layer]);

        cout << "Applying Hadamard product...\n";

        // Hadamard equals by sigder of current layer act
        for (int i = 0; i < gradientAggregate.getSize(); i++)
        {
            gradientAggregate[i] *= actder(activations[layer][i]);
        }

        // Compute weight deltas for this layer
        // := dot(gradientAggregate, activationsOfBelowLevel);

        cout << "Dotting...\n";

        // WTH?
        weightDeltas[layer] = dot(gradientAggregate, activations[layer - 1]);
        throw runtime_error("HOW");

        // FOR TESTING PURPOSES
        cout << "Layer " << layer << " has gradient aggregate:\n";
        for (int i = 0; i < gradientAggregate.getSize(); i++)
        {
            cout << '\t' << gradientAggregate[i] << '\n';
        }
    }

    cout << "Done. Performing stochastic gradient descent...\n";

    // Modify weights by computed gradient values
    double coefficient = drand(MIN_STEP_SIZE, MAX_STEP_SIZE);
    for (int layer = 1; layer < numLayers; layer++)
    {
        for (int node = 0; node < sizes[layer]; node++)
        {
            for (int weight = 0; weight < weights[layer][node].getSize(); weight++)
            {
                weights[layer][node][weight] -= coefficient * weightDeltas[layer][node][weight];
            }
        }
    }

    // Die because this algorithm sucks hardcore

    return;
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

    start = chrono::high_resolution_clock::now();

    n.backprop({1, 2, 3, 4, 5});

    end = chrono::high_resolution_clock::now();
    elapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    cout << "Backpropogation took " << elapsed << " ns.\n";

    return 0;
}
