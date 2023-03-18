/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include "networkSave.hpp"

void saveNetwork(ostream &Stream, network &From)
{
    // Save passes
    Stream << From.passes << "\n\n";

    // Save sizes
    Stream << From.sizes.size() << '\n';
    for (auto s : From.sizes)
    {
        Stream << s << '\t';
    }
    Stream << "\n\n";

    // Save nodes
    for (int layer = 1; layer < From.sizes.size(); layer++)
    {
        for (int node = 0; node < From.sizes[layer]; node++)
        {
            // Activation and bias
            Stream << From.nodes[layer][node].act << '\t' << From.nodes[layer][node].bias << '\n';

            // Weights
            for (int weight = 0; weight < From.sizes[layer - 1]; weight++)
            {
                Stream << From.nodes[layer][node].weights[weight] << '\t';
            }
            Stream << '\n';
        }
    }
    Stream << '\n';

    // Save training data
    Stream << From.trainingData.size() << '\n';
    for (int i = 0; i < From.trainingData.size(); i++)
    {
        Stream << From.trainingData[i].inputs.size() << '\n';
        for (auto inp : From.trainingData[i].inputs)
        {
            Stream << inp << '\t';
        }
        Stream << '\n';

        Stream << From.trainingData[i].expected.size() << '\n';
        for (auto exp : From.trainingData[i].expected)
        {
            Stream << exp << '\t';
        }
        Stream << '\n';
    }
    Stream << '\n';

    // Save error history
    for (auto e : From.errors)
    {
        Stream << e << '\t';
    }

    return;
}

void saveNetwork(const string &Filename, network &From)
{
    ofstream file(Filename);
    assert(file.is_open());

    saveNetwork(file, From);

    file.close();
    return;
}

network loadNetwork(istream &Stream)
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

    // Create network using constructor
    network out(sizes);

    // Load nodes
    for (int layer = 1; layer < out.sizes.size(); layer++)
    {
        for (int node = 0; node < out.sizes[layer]; node++)
        {
            // Activation and bias
            Stream >> out.nodes[layer][node].act >> out.nodes[layer][node].bias;

            // Weights
            for (int weight = 0; weight < out.sizes[layer - 1]; weight++)
            {
                Stream >> out.nodes[layer][node].weights[weight];
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
            toAdd.inputs.push_back(temp);
        }

        Stream >> dataSize;
        for (int j = 0; j < dataSize; j++)
        {
            double temp;
            Stream >> temp;
            toAdd.expected.push_back(temp);
        }

        out.trainingData.push_back(toAdd);
    }

    // Load error history
    while (!Stream.eof())
    {
        double error;
        Stream >> error;
        out.errors.push_back(error);
    }
    out.passes = passes;

    return out;
}

network loadNetwork(const string &Filename)
{
    ifstream file(Filename);
    assert(file.is_open());

    network out = loadNetwork(file);

    file.close();
    return out;
}