/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include "networkSave.hpp"

void saveNetwork(ostream &Stream, Network &From)
{
    // Save passes
    Stream << From.passes << "\n\n";

    // Save sizes
    Stream << From.sizes.getSize() << '\n';
    for (int i = 0; i < From.sizes.getSize(); i++)
    {
        Stream << From.sizes[i] << '\t';
    }
    Stream << "\n\n";

    // Save nodes
    for (int layer = 1; layer < From.sizes.getSize(); layer++)
    {
        for (int node = 0; node < From.sizes[layer]; node++)
        {
            // Activation and bias
            Stream << From.activations[layer][node] << '\t' << From.weights[layer - 1][node][-1] << '\n';
            // Stream << From.nodes[layer][node].act << '\t' << From.nodes[layer][node].bias << '\n';

            // Weights
            for (int weight = 0; weight < From.sizes[layer - 1]; weight++)
            {
                Stream << From.weights[layer - 1][node][weight] << '\t';
                // Stream << From.nodes[layer][node].weights[weight] << '\t';
            }
            Stream << '\n';
        }
    }
    Stream << '\n';

    // Save training data
    Stream << From.trainingData.size() << '\n';
    for (int i = 0; i < From.trainingData.size(); i++)
    {
        Stream << From.trainingData[i].input.size() << '\n';
        for (auto inp : From.trainingData[i].input)
        {
            Stream << inp << '\t';
        }
        Stream << '\n';

        Stream << From.trainingData[i].output.size() << '\n';
        for (auto exp : From.trainingData[i].output)
        {
            Stream << exp << '\t';
        }
        Stream << '\n';
    }
    Stream << '\n';

    // Save error
    Stream << From.getError() << '\t';

    return;
}

void saveNetwork(const string &Filename, Network &From)
{
    ofstream file(Filename);
    assert(file.is_open());

    saveNetwork(file, From);

    file.close();
    return;
}

Network loadNetwork(istream &Stream)
{
    // Load passes
    int passes;
    Stream >> passes;

    vector<int> sizes;

    // Load sizes
    int size;
    Stream >> size;
    for (int i = 0; i < size; i++)
    {
        int layerSize;
        Stream >> layerSize;
        sizes.push_back(layerSize);
    }

    // Create Network using constructor
    Network out(sizes);

    // Load nodes
    for (int layer = 1; layer < out.sizes.getSize(); layer++)
    {
        for (int node = 0; node < out.sizes[layer]; node++)
        {
            // Activation and bias
            Stream >> out.activations[layer][node] >> out.weights[layer - 1][node][-1];
            // Stream >> out.nodes[layer][node].act >> out.nodes[layer][node].bias;

            // Weights
            for (int weight = 0; weight < out.sizes[layer - 1]; weight++)
            {
                Stream >> out.weights[layer - 1][node][weight];
                // Stream >> out.nodes[layer][node].weights[weight];
            }
        }
    }

    // Load training data
    Stream >> size;
    for (int i = 0; i < size; i++)
    {
        dataset toAdd;

        int dataSize;
        Stream >> dataSize;
        for (int j = 0; j < dataSize; j++)
        {
            double temp;
            Stream >> temp;
            toAdd.input.push_back(temp);
        }

        Stream >> dataSize;
        for (int j = 0; j < dataSize; j++)
        {
            double temp;
            Stream >> temp;
            toAdd.output.push_back(temp);
        }

        out.trainingData.push_back(toAdd);
    }

    out.passes = passes;

    return out;
}

Network loadNetwork(const string &Filename)
{
    ifstream file(Filename);
    assert(file.is_open());

    Network out = loadNetwork(file);

    file.close();
    return out;
}