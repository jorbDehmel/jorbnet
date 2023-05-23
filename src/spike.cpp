#include "spike.hpp"

SafeArray<double> SpikeNetwork::spikeProp(const SafeArray<double> &Input)
{
    // Ensure input vector matches network size
    if (Input.getSize() != sizes[0])
    {
        cout << "Requires " << sizes[0] << " but was given " << Input.size() << '\n';
        throw runtime_error("Error: Input size does not match network size");
    }

    // Zero out activations
    for (int layer = 1; layer < numLayers; layer++)
    {
        for (int node = 0; node < sizes[layer]; node++)
        {
            activations[layer][node] = 0;
        }
    }

    // Load into inputs and get sum
    double sum = 0;
    for (int i = 0; i < Input.getSize(); i++)
    {
        activations[0][i] = Input[i];
        sum += Input[i];
    }

    // First hidden layer:
    // If the sum of the input is greater than the bias of a node, its activation becomes one
    for (int i = 0; i < sizes[1]; i++)
    {
        if (sum + weights[0][i][-1] < 0)
        {
            activations[1][i] = 0;
        }
        else
        {
            activations[1][i] = 1;
        }
    }

    // Remaining layers:
    // If the sum of the weights attached to non-zero activations is greater than the bias of
    // a node, its activation becomes one.
    for (int layer = 2; layer < numLayers; layer++)
    {
        for (int node = 0; node < sizes[layer]; node++)
        {
            // Get sum of activated neurons
            double sum = 0;
            for (int i = 0; i < sizes[layer - 1]; i++)
            {
                if (activations[layer - 1][i] > 0)
                {
                    sum += weights[layer - 1][node][i];
                }
            }

            if (sum + weights[layer - 1][node][-1] < 0)
            {
                activations[layer][node] = 0;
            }
            else
            {
                activations[layer][node] = 1;
            }
        }
    }

    // Return output layer
    return activations[-1];
}

SpikeNetwork &SpikeNetwork::operator=(Network &Other)
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
    for (int i = 0; i < Other.trainingData.size(); i++)
    {
        trainingData.push_back(Other.trainingData[i]);
    }

    passes = Other.passes;

    return *this;
}

SpikeNetwork &SpikeNetwork::operator=(SpikeNetwork &Other)
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
    for (int i = 0; i < Other.trainingData.size(); i++)
    {
        trainingData.push_back(Other.trainingData[i]);
    }

    passes = Other.passes;

    return *this;
}
