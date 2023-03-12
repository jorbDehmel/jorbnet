#include "networkSave.hpp"

void saveNetwork(const string &Filename, network &From)
{
    ofstream file(Filename);
    assert(file.is_open());

    // Save sizes
    file << From.sizes.size() << '\n';
    for (auto s : From.sizes)
    {
        file << s << '\t';
    }
    file << "\n\n";

    // Save nodes
    for (int layer = 1; layer < From.sizes.size(); layer++)
    {
        for (int node = 0; node < From.sizes[layer]; node++)
        {
            // Activation and bias
            file << From.nodes[layer][node].act << '\t' << From.nodes[layer][node].bias << '\n';

            // Weights
            for (int weight = 0; weight < From.sizes[layer - 1]; weight++)
            {
                file << From.nodes[layer][node].weights[weight] << '\t';
            }
            file << '\n';
        }
    }
    file << '\n';

    // Save training data
    file << From.trainingData.size() << '\n';
    for (int i = 0; i < From.trainingData.size(); i++)
    {
        file << From.trainingData[i].inputs.size() << '\n';
        for (auto inp : From.trainingData[i].inputs)
        {
            file << inp << '\t';
        }
        file << '\n';

        file << From.trainingData[i].expected.size() << '\n';
        for (auto exp : From.trainingData[i].expected)
        {
            file << exp << '\t';
        }
        file << '\n';
    }
    file << '\n';

    // Save error history
    for (auto e : From.errors)
    {
        file << e << '\t';
    }

    file.close();
    return;
}

network loadNetwork(const string &Filename)
{
    ifstream file(Filename);
    assert(file.is_open());

    vector<int> sizes;

    // Load sizes
    int size;
    file >> size;
    for (int i = 0; i < size; i++)
    {
        int layerSize;
        file >> layerSize;
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
            file >> out.nodes[layer][node].act >> out.nodes[layer][node].bias;

            // Weights
            for (int weight = 0; weight < out.sizes[layer - 1]; weight++)
            {
                file >> out.nodes[layer][node].weights[weight];
            }
        }
    }

    // Load training data
    file >> size;
    for (int i = 0; i < size; i++)
    {
        dataset toAdd;

        int dataSize;
        file >> dataSize;
        for (int j = 0; j < dataSize; j++)
        {
            double temp;
            file >> temp;
            toAdd.inputs.push_back(temp);
        }

        file >> dataSize;
        for (int j = 0; j < dataSize; j++)
        {
            double temp;
            file >> temp;
            toAdd.expected.push_back(temp);
        }

        out.trainingData.push_back(toAdd);
    }

    // Load error history
    while (!file.eof())
    {
        double error;
        file >> error;
        out.errors.push_back(error);
    }

    file.close();
    return out;
}