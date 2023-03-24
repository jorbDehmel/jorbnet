/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include "network.hpp"

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
    for (auto d : Other.trainingData)
    {
        trainingData.push_back(d);
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
    for (auto d : Other.trainingData)
    {
        trainingData.push_back(d);
    }

    passes = Other.passes;

    return *this;
}

vector<double> Network::prop(const vector<double> &Input)
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
    for (int i = 0; i < sizes[-1]; i++)
    {
        out.push_back(activations[-1][i]);
    }

    return out;
}

// Requires previous propogation
void Network::backprop(const vector<double> &Expected)
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
    // cout << "Getting error...\n";
    double C = 0;
    for (int i = 0; i < sizes[numLayers - 1]; i++)
    {
        C += pow(Expected[i] - activations[-1][i], 2);
        // cout << "Exp: " << Expected[i] << " act: " << activations[-1][i] << '\n';
    }
    // cout << "Error: " << C << '\n';

    // Set up gradient aggregate as gradient of errors wrt final acts
    // Then hadamard-ed by vector of sigder of previous final act

    // cout << "Establishing GA...\n";
    SafeArray<double> gradientAggregate(Expected.size());
    for (int i = 0; i < gradientAggregate.getSize(); i++)
    {
        gradientAggregate[i] = -2 * (Expected[i] - activations[-1][i]) + C - pow(Expected[i] - activations[-1][i], 2);
        gradientAggregate[i] *= actder(activations[-1][i]);
    }

    // Modify weights and biases for the last layer
    for (int node = 0; node < sizes[numLayers - 1]; node++)
    {
        // Iterate through weights
        for (int weight = 0; weight < sizes[numLayers - 2]; weight++)
        {
            // cout << "Adjusting weight " << layer << " " << node << " " << weight << '\n';

            weightDeltas[numLayers - 2][node][weight] = activations[numLayers - 2][weight] * gradientAggregate[node];
        }

        // cout << "Adjusting bias.\n";

        // Adjust bias
        weightDeltas[numLayers - 2][node][-1] = gradientAggregate[node]; // bias
    }

    // Iterate back through network
    // cout << "Backpropogating...\n";
    for (int layer = numLayers - 2; layer >= 1; layer--)
    {
        // cout << "Starting layer " << layer << '\n';

        // cout << "GA size: " << gradientAggregate.getSize() << '\n'
        //      << "Weights[layer] size: h = " << weights[layer].getSize() << " w = " << weights[layer][0].getSize() << '\n';

        // Dot product equals by weights of above layer
        dotEquals(gradientAggregate, weights[layer]);

        // cout << "Applying Hadamard product...\n";

        // Hadamard equals by sigder of current layer act
        for (int i = 0; i < gradientAggregate.getSize() - 1; i++)
        {
            gradientAggregate[i] *= actder(activations[layer][i]);
        }

        // Compute weight deltas for this layer

        // cout << "Converting GA to grad wrt weights...\n";

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
                // cout << "Adjusting weight " << layer << " " << node << " " << weight << '\n';

                weightDeltas[layer - 1][node][weight] = activations[layer - 1][weight] * gradientAggregate[node];
            }

            // cout << "Adjusting bias.\n";

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

    // cout << "Done. Performing stochastic gradient descent...\n";

    // Modify weights by computed gradient values
    double coefficient = drand(minStepSize, maxStepSize) / magnitude;
    // cout << "Coeff: " << coefficient << '\n';

    for (int layer = 1; layer < numLayers; layer++)
    {
        for (int node = 0; node < sizes[layer]; node++)
        {
            for (int weight = 0; weight < weights[layer - 1][node].getSize(); weight++)
            {
                // cout << "l " << layer << " n " << node << " w " << weight << " by " << coefficient * weightDeltas[layer - 1][node][weight] << '\n';
                weights[layer - 1][node][weight] -= coefficient * weightDeltas[layer - 1][node][weight];
            }
        }
    }

    // cout << "Finished.\n";

    // Die because this algorithm sucks hardcore

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
        vector<double> inp = trainingData[choice].input;
        vector<double> exp = trainingData[choice].output;

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
        auto obs = prop(set.input);
        for (int i = 0; i < set.output.size(); i++)
        {
            sum += pow(set.output[i] - obs[i], 2);
        }
    }

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
