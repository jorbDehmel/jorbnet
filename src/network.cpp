/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include "network.hpp"

// Easy-der
double __stderr(const SafeArray<double> &Obs, const SafeArray<double> &Exp)
{
    double out = 0;
    for (int i = 0; i < Obs.getSize(); i++)
    {
        out += pow(Exp[i] - Obs[i], 2);
    }
    return out / Obs.getSize();
}

SafeArray<double> __stderrder(const SafeArray<double> &Obs, const SafeArray<double> &Exp)
{
    SafeArray<double> out(Obs.getSize());
    for (int i = 0; i < out.getSize(); i++)
    {
        out[i] = -2 / out.getSize() * (Exp[i] - Obs[i]);
    }
    return out;
}

string toTime(long long Ns)
{
    if (Ns < 0)
    {
        return "NaN";
    }

    long long nanoseconds, microseconds, milliseconds, seconds, minutes, hours, days;

    nanoseconds = Ns;
    microseconds = nanoseconds / 1'000;
    milliseconds = microseconds / 1'000;
    seconds = milliseconds / 1'000;
    minutes = seconds / 60;
    hours = minutes / 60;
    days = hours / 24;

    nanoseconds %= 1'000;
    microseconds %= 1'000;
    milliseconds %= 1'000;
    seconds %= 60;
    minutes %= 60;
    hours %= 24;

    string out = "";
    if (days != 0)
    {
        out = to_string(days) + ((days != 1) ? " days, " : " day, ");
    }
    if (hours != 0)
    {
        out += to_string(hours) + ((hours != 1) ? " hours, " : " hour, ");
    }
    if (minutes != 0)
    {
        out += to_string(minutes) + ((minutes != 1) ? " minutes, " : " minute, ");
    }
    if (seconds != 0)
    {
        out += to_string(seconds) + ((seconds != 1) ? " seconds, " : " second, ");
    }
    if (milliseconds != 0)
    {
        out += to_string(milliseconds) + ((milliseconds != 1) ? " milliseconds, " : " millisecond, ");
    }
    if (microseconds != 0)
    {
        out += to_string(microseconds) + ((microseconds != 1) ? " microseconds, " : " microsecond, ");
    }
    if (nanoseconds != 0)
    {
        out += to_string(nanoseconds) + ((nanoseconds != 1) ? " nanoseconds  " : " nanosecond  ");
    }

    return out.substr(0, out.size() - 2) + '.';
}

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

double __lin(const double &X)
{
    return X;
}

double __linder(const double &Act)
{
    return 1;
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
    // cout << "Beginning dot product...\n";

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
        throw runtime_error("Dimensional mismatch: Cannot take dot product or transposed dot product.");
    }

    // cout << "Completed dot product.\n";

    return;
}

Network::Network(const vector<int> &Sizes)
{
    // Metadata
    creationTime = time(NULL);
    description = "N/A";
    dataInfo = 0;

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
                weights[layer][node][weight] = drand(-weightVariance, weightVariance);
            }
        }
    }

    return;
}

Network::Network(const Network &Other)
{
    // Steal metadata
    description = Other.description;
    creationTime = Other.creationTime;
    dataInfo = Other.dataInfo;

    // Construct sizes list
    numLayers = Other.sizes.getSize();
    sizes = SafeArray<int>(numLayers);
    for (int i = 0; i < numLayers; i++)
    {
        sizes[i] = Other.sizes[i];
    }

    // Construct the first dimensions of activations and weights
    activations = SafeArray<SafeArray<double>>(numLayers);

    weights = SafeArray<SafeArray<SafeArray<double>>>(numLayers - 1);
    weightDeltas = SafeArray<SafeArray<SafeArray<double>>>(numLayers - 1);

    // Construct the second dimensions of activations and weights
    for (int layer = 0; layer < numLayers; layer++)
    {
        activations[layer] = SafeArray<double>(sizes[layer]);
        for (int node = 0; node < Other.sizes[layer]; node++)
        {
            activations[layer][node] = Other.activations[layer][node];
        }

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
                weights[layer][node][weight] = Other.weights[layer][node][weight];
            }
        }
    }

    // Copy training data
    for (int i = 0; i < Other.trainingData.size(); i++)
    {
        trainingData.push_back(Other.trainingData[i]);
    }

    passes = Other.passes;

    return;
}

Network &Network::operator=(const Network &Other)
{
    // Note: SafeArrays should automatically free their data when
    // the = operator is used upon them. Thus, this is identical
    // to the copy constructor (since the old data is freed).

    // Construct sizes list
    numLayers = Other.sizes.getSize();
    sizes = SafeArray<int>(numLayers);
    for (int i = 0; i < numLayers; i++)
    {
        sizes[i] = Other.sizes[i];
    }

    // Construct the first dimensions of activations and weights
    activations = SafeArray<SafeArray<double>>(numLayers);

    weights = SafeArray<SafeArray<SafeArray<double>>>(numLayers - 1);
    weightDeltas = SafeArray<SafeArray<SafeArray<double>>>(numLayers - 1);

    // Construct the second dimensions of activations and weights
    for (int layer = 0; layer < numLayers; layer++)
    {
        activations[layer] = SafeArray<double>(sizes[layer]);
        for (int node = 0; node < Other.sizes[layer]; node++)
        {
            activations[layer][node] = Other.activations[layer][node];
        }

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
                weights[layer][node][weight] = Other.weights[layer][node][weight];
            }
        }
    }

    // Copy training data
    for (int i = 0; i < Other.trainingData.size(); i++)
    {
        trainingData.push_back(Other.trainingData[i]);
    }

    passes = Other.passes;

    return *this;
}

SafeArray<double> Network::prop(const SafeArray<double> &Input)
{
    // Ensure input vector matches network size
    if (Input.getSize() != sizes[0])
    {
        throw runtime_error("Error: Input size does not match network size");
    }

    // Load into inputs
    for (int i = 0; i < Input.getSize(); i++)
    {
        activations[0][i] = Input[i];
    }

    // Propogate
    // Time proportional to the number of weights in the network
    for (int layer = 1; layer < numLayers; layer++)
    {
        for (int node = 0; node < sizes[layer]; node++)
        {
            // O(len(weights[layer - 1][node]))
            activations[layer][node] = bdot(activations[layer - 1], weights[layer - 1][node], sizes[layer - 1]);

            // O(1)
            activations[layer][node] = act(activations[layer][node]);
        }
    }

    // Return output layer
    return activations[-1];
}

// Requires previous propogation
// Time proportional to the number of weights in the network
void Network::backprop(const SafeArray<double> &Expected)
{
    // see wikipedia on backpropagation
    // http://neuralnetworksanddeeplearning.com/chap2.html
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
    // C * n := (e_0 - a_0)^2 + (e_1 - a_1)^2 + (e_2 - a_2)^2
    // Ergo the error partial derivative with respect to each act is:
    // wrt a_0:
    //      -2 / n (e_0 - a_0) + (C - (e_0 - a_0)^2 / n)
    // wrt a_1:
    //      -2 / n (e_1 - a_1) + (C - (e_1 - a_1)^2 / n)
    // generally:
    //      -2 / n (e_i - a_i) + (C - (e_i - a_i)^2 / n)

    // Amount to modify the weights by is
    // = dot(gradientAggregate, activationsOfLayerBelow)

    // Not sure what I was smoking when I wrote this, but it doesn't make any sense:
    /*double C = 0;
    for (int i = 0; i < sizes[numLayers - 1]; i++)
    {
        C += pow(Expected[i] - activations[-1][i], 2);
        // cout << "Exp: " << Expected[i] << " act: " << activations[-1][i] << '\n';
    }
    C /= sizes[numLayers - 1];
    // This value is NOT OF ANY USE in calculating the partial derivative of the error function!*/

    // Set up gradient aggregate as gradient of errors wrt final acts
    // Then hadamard-ed by vector of sigder of previous final act

    // SafeArray<double> gradientAggregate(Expected.size());
    SafeArray<double> gradientAggregate = errder(activations[-1], Expected);

    for (int i = 0; i < gradientAggregate.getSize(); i++)
    {
        gradientAggregate[i] *= actder(activations[-1][i]);
    }

    // Modify weights and biases for the last layer
    for (int node = 0; node < sizes[numLayers - 1]; node++)
    {
        // Iterate through weights
        for (int weight = 0; weight < sizes[numLayers - 2]; weight++)
        {
            weightDeltas[numLayers - 2][node][weight] = activations[numLayers - 2][weight] * gradientAggregate[node];
        }

        // Adjust bias
        weightDeltas[numLayers - 2][node][-1] = gradientAggregate[node]; // bias
    }

    // Iterate back through network
    for (int layer = numLayers - 2; layer >= 1; layer--)
    {
        // Dot product equals by weights of above layer
        dotEquals(gradientAggregate, weights[layer]);

        // Hadamard equals by sigder of current layer act
        for (int i = 0; i < gradientAggregate.getSize() - 1; i++)
        {
            gradientAggregate[i] *= actder(activations[layer][i]);
        }

        // Compute weight deltas for this layer

        // CONVERT GRADIENT AGGREGATE TO GRADIENT WRT WEIGHTS
        // partial C wrt w^l_{jk} = a_k^{l-1} (ga)^l_j
        // For layer l, for source node j, towards node k
        // = (destination node's old act) (the jth item in ga)
        // For bias: = (jth item in ga) or (node-th item in ga)

        for (int node = 0; node < sizes[layer]; node++)
        {
            // Iterate through weights
            for (int weight = 0; weight < sizes[layer - 1]; weight++)
            {
                weightDeltas[layer - 1][node][weight] = activations[layer - 1][weight] * gradientAggregate[node];
            }

            // Adjust bias
            weightDeltas[layer - 1][node][-1] = gradientAggregate[node]; // bias
        }

        // Remove bias from gradient aggregate for next layer
        gradientAggregate.setSize(-1);
    }

    // Find magnitude for unit-vector-ification
    double magnitude = 0;
    for (int layer = 1; layer < numLayers; layer++)
    {
        for (int node = 0; node < sizes[layer]; node++)
        {
            for (int weight = 0; weight < weights[layer - 1][node].getSize(); weight++)
            {
                magnitude += pow(weightDeltas[layer - 1][node][weight], 2);
            }
        }
    }
    magnitude = sqrt(magnitude);

    // Prevent error from cascading into nan's forever
    if (magnitude == 0)
    {
        magnitude = 1;
    }

    // Modify weights by computed gradient values
    double coefficient = drand(minStepSize, maxStepSize) / magnitude;

    for (int layer = 1; layer < numLayers; layer++)
    {
        for (int node = 0; node < sizes[layer]; node++)
        {
            for (int weight = 0; weight < weights[layer - 1][node].getSize(); weight++)
            {
                weights[layer - 1][node][weight] -= coefficient * weightDeltas[layer - 1][node][weight];
            }
        }
    }

    return;
}

double Network::bdot(SafeArray<double> Inputs, SafeArray<double> Weights, const int &SizeOfInputs) const
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

void Network::train(const int &Num)
{
    for (int i = 0; i < Num; i++)
    {
        // Choose random dataset
        int choice = random() % trainingData.size();
        SafeArray<double> inp = trainingData[choice].input;
        SafeArray<double> exp = trainingData[choice].output;

        // Train on the selected dataset
        prop(inp);
        backprop(exp);
    }
    passes += Num;
    return;
}

vector<double *> Network::getWeights() const
{
    vector<double *> out;
    for (int layer = 1; layer < sizes.getSize(); layer++)
    {
        for (int node = 0; node < sizes[layer]; node++)
        {
            for (int weight = 0; weight < weights[layer - 1][node].getSize(); weight++)
            {
                out.push_back(&weights[layer - 1][node][weight]);
            }
        }
    }
    return out;
}

double Network::getError()
{
    double sum = 0;

    for (auto set : trainingData)
    {
        SafeArray<double> obs = prop(set.input);
        for (int i = 0; i < set.output.getSize(); i++)
        {
            sum += pow(set.output[i] - obs[i], 2);
        }
    }
    sum /= trainingData[0].output.getSize();

    return sum;
}

// For 1000 passes on an XOR/XAND 2-5-2 network:
// This method:
//  36'395'427
// OOP method:
// 471'944'217
// LA method is 10 times faster!
// (Thank god, or I would have wasted a lot of work)

// A little test main if you want:
/*
#include <chrono>
#include <iostream>
using namespace std;
int main()
{
    srand(time(NULL));

    vector<int> sizes = {2, 5, 2};
    NetworkLA n(sizes);
    n.act = __sigmoid;
    n.actder = __sigder;

    n.trainingData.push_back(dataset{{0, 0}, {0, 1}});
    n.trainingData.push_back(dataset{{0, 1}, {1, 0}});
    n.trainingData.push_back(dataset{{1, 0}, {1, 0}});
    n.trainingData.push_back(dataset{{1, 1}, {0, 1}});

    cout << "Total error pre-training: " << n.getError() << '\n';

    const int numPasses = 1000;

    auto start = chrono::high_resolution_clock::now();
    n.train(numPasses);
    auto end = chrono::high_resolution_clock::now();
    int elapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    cout << "Elapsed NS: " << elapsed << '\n'
         << "NS per pass: " << (double)elapsed / numPasses
         << "\n\n";

    for (auto set : n.trainingData)
    {
        // Display input
        for (auto i : set.input)
        {
            cout << i << ' ';
        }
        cout << ":\t";

        // Display output (rounded)
        auto obs = n.prop(set.input);
        for (auto o : obs)
        {
            cout << (int)(o + 0.5) << ' ';
        }
        cout << '\n';

        // Display expected
        cout << "Exp: ";
        for (auto e : set.output)
        {
            cout << e << " ";
        }
        cout << "\n\n";
    }

    cout << "Total error post-training: " << n.getError() << "\n\n";

    return 0;
}
*/
