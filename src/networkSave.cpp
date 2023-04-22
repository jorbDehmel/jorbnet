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
    Stream << From.passes << " ";

    // Save sizes
    Stream << From.sizes.getSize() << ' ';
    for (int i = 0; i < From.sizes.getSize(); i++)
    {
        Stream << From.sizes[i] << ' ';
    }

    // Save nodes
    for (int layer = 1; layer < From.sizes.getSize(); layer++)
    {
        for (int node = 0; node < From.sizes[layer]; node++)
        {
            // Weights
            for (int weight = 0; weight < From.sizes[layer - 1]; weight++)
            {
                Stream << From.weights[layer - 1][node][weight] << ' ';
            }

            // Bias
            Stream << From.weights[layer - 1][node][-1] << ' ';
        }
    }

    // Save training data
    Stream << From.trainingData.size() << ' ';
    for (int i = 0; i < From.trainingData.size(); i++)
    {
        Stream << From.trainingData[i].input.getSize() << ' ';

        for (int j = 0; j < From.trainingData[i].input.getSize(); j++)
        {
            Stream << From.trainingData[i].input[j] << ' ';
        }

        Stream << From.trainingData[i].output.getSize() << ' ';

        for (int j = 0; j < From.trainingData[i].output.getSize(); j++)
        {
            Stream << From.trainingData[i].output[j] << ' ';
        }
    }

    Stream << From.creationTime << '\n'
           << From.dataInfo << '\n'
           << From.description << '\n';

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
            // Weights
            for (int weight = 0; weight < out.sizes[layer - 1]; weight++)
            {
                Stream >> out.weights[layer - 1][node][weight];
            }

            // Bias
            Stream >> out.weights[layer - 1][node][-1];
        }
    }

    // Load training data
    Stream >> size;
    for (int i = 0; i < size; i++)
    {
        dataset toAdd;

        int dataSize;
        Stream >> dataSize;
        toAdd.input = SafeArray<double>(dataSize);
        for (int j = 0; j < dataSize; j++)
        {
            double temp;
            Stream >> temp;

            toAdd.input[j] = temp;
        }

        Stream >> dataSize;
        toAdd.output = SafeArray<double>(dataSize);
        for (int j = 0; j < dataSize; j++)
        {
            double temp;
            Stream >> temp;
            toAdd.output[j] = temp;
        }

        out.trainingData.push_back(toAdd);
    }

    out.passes = passes;

    Stream >> out.creationTime >> out.dataInfo;

    string line;
    out.description = "";
    while (!Stream.eof())
    {
        getline(Stream, line);
        out.description += line;
    }

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