/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include "network.hpp"

network::network(const vector<int> &Sizes)
{
    for (auto i : Sizes)
    {
        sizes.push_back(i);
    }

    // Create network
    nodes = new node *[sizes.size()];

    assert(nodes != nullptr);
    for (int i = 0; i < sizes.size(); i++)
    {
        nodes[i] = new node[sizes[i]];
        assert(nodes[i] != nullptr);
    }

    // Link nodes
    for (int i = 1; i < sizes.size(); i++)
    {
        for (int j = 0; j < sizes[i]; j++)
        {
            initNode(nodes[i][j], nodes[i - 1], sizes[i - 1]);
            nodes[i][j].wIndex = weights.size();
            for (int k = 0; k < nodes[i][j].links.size(); k++)
            {
                weights.push_back(&nodes[i][j].weights[k]);
            }
            weights.push_back(&nodes[i][j].bias);
        }
    }

    return;
}

network::network(const network &Other)
{
    for (auto i : Other.sizes)
    {
        sizes.push_back(i);
    }

    // Create network
    nodes = new node *[sizes.size()];
    assert(nodes != nullptr);

    for (int i = 0; i < sizes.size(); i++)
    {
        nodes[i] = new node[sizes[i]];
        assert(nodes[i] != nullptr);
    }

    // Link nodes
    for (int i = 1; i < sizes.size(); i++)
    {
        for (int j = 0; j < sizes[i]; j++)
        {
            cloneNode(nodes[i][j], Other.nodes[i][j], nodes[i - 1], sizes[i - 1]);
            for (int k = 0; k < Other.nodes[i][j].links.size(); k++)
            {
                assert(nodes[i][j].weights[k] == Other.nodes[i][j].weights[k]);
                assert(&nodes[i][j].weights[k] != &Other.nodes[i][j].weights[k]);

                weights.push_back(&nodes[i][j].weights[k]);
            }
            weights.push_back(&nodes[i][j].bias);
        }
    }

    // Copy training data
    for (auto d : Other.trainingData)
    {
        trainingData.push_back(d);
    }

    // Copy error
    for (int i = Other.errors.size() * 0.5; i < Other.errors.size(); i++)
    {
        errors.push_back(Other.errors[i]);
    }
    if (errors.size() == 0)
    {
        errors.push_back(99999);
    }

    return;
}

network::~network()
{
    // Delete network
    for (int i = 1; i < sizes.size(); i++)
    {
        for (int j = 0; j < sizes[i]; j++)
        {
            freeNode(&nodes[i][j]);
        }
        delete[] nodes[i];
    }
    delete[] nodes;
    return;
}

vector<double> network::propogate(vector<double> &Input)
{
    // Assert sizes match
    if (Input.size() != sizes[0])
    {
        throw runtime_error("Dimensional mismatch: Cannot propogate");
    }

    // Write input to input layer activations
    for (int i = 0; i < Input.size(); i++)
    {
        nodes[0][i].act = Input[i];
    }

    // Propogate all layers but the first
    for (int layer = 1; layer < sizes.size(); layer++)
    {
        for (int i = 0; i < sizes[layer]; i++)
        {
            prop(nodes[layer][i]);
        }
    }

    // Copy output activations to output vector
    vector<double> out;
    for (int i = 0; i < sizes.back(); i++)
    {
        out.push_back(nodes[sizes.size() - 1][i].act);
    }
    return out;
}

// -1 for random
void network::backprop(const int Index)
{
    assert(trainingData.size() > 0);

    // Select training data
    dataset *data;
    if (Index < 0 || Index >= trainingData.size())
    {
        data = &trainingData[rand() % trainingData.size()];
    }
    else
    {
        data = &trainingData[Index];
    }

    // Find observed and error
    vector<double> observed = propogate(data->inputs);

    vector<double> errorVec;
    for (int i = 0; i < observed.size(); i++)
    {
        errorVec.push_back(data->expected[i] - observed[i]);
    }

    // Construct gradient error vector w/ respect to each weight
    /*auto start = chrono::high_resolution_clock::now();
    bpropCount = 0;*/

    double *gradient = new double[weights.size()];

    map<node *, double> foundGenerals;
    for (int weightIndex = 0; weightIndex < weights.size(); weightIndex++)
    {
        double *weight = weights[weightIndex];

        // Get the derivative of the error function
        // with respect to the current weight

        gradient[weightIndex] = 0;
        map<node *, double> foundWeights;
        for (int i = 0; i < observed.size(); i++)
        {
            // Sum of err times derivative of each output node
            gradient[weightIndex] -= errorVec[i] * bprop(nodes[sizes.size() - 1][i], weight, foundWeights, foundGenerals);
        }
        gradient[weightIndex] *= 2;
    }

    delete[] gradient;

    /*auto end = chrono::high_resolution_clock::now();
    cout << "Gradient finding took "
         << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
         << " ns.\n"
         << "There were " << weights.size() << " weights.\n"
         << "BProp was called " << bpropCount << " times.\n";*/

    // Move away from direction of gradient vector (decr weights)
    for (int i = 0; i < weights.size(); i++)
    {
        *weights[i] -= gradient[i] * drand(MIN_STEP_SIZE, MAX_STEP_SIZE);
    }

    passes++;
    return;
}

void network::train(const int NumTimes)
{
    for (int i = 0; i < NumTimes; i++)
    {
        backprop(-1);

        // Note: Passes is incremented by backprop
        if (passes % LOG_INTERVAL == 0)
        {
            double error = 0;
            for (auto set : trainingData)
            {
                auto observed = propogate(set.inputs);
                error += err(observed, set.expected);
            }

            errors.push_back(error);

            if (error < ERROR_BREAK)
            {
                break;
            }
        }
    }

    double error = 0;
    for (auto set : trainingData)
    {
        auto observed = propogate(set.inputs);
        error += err(observed, set.expected);
    }
    errors.push_back(error);

    return;
}

vector<double> __graphData;
int __graphItem;

// This will halt execution until the graph is closed
void graphNetworkError(const network &What)
{
    if (What.errors.size() < 10)
    {
        cout << "Cannot graph network with less than 10 error values.\n";
        return;
    }

    __graphItem = 0;
    __graphData.clear();

    double max = -1;
    for (auto e : What.errors)
    {
        __graphData.push_back(e);

        if (e > max)
        {
            max = e;
        }
    }

    LineGraph g;

    jgraph::UPSCALING_X = jgraph::UPSCALING_Y = 2;
    jgraph::XMIN = jgraph::YMIN = -1;
    jgraph::XMAX = __graphData.size() - 1;
    jgraph::YMAX = 5;
    jgraph::TICK_SPACING_X = __graphData.size() / 10;

    g.equations.push_back(__graphErrorHelper);
    g.refresh();
    mainLoop(&g);

    return;
}

bool __graphErrorHelper(double &X, double &Y)
{
    // Failsafe
    if (__graphData.size() <= 1)
    {
        X = Y = 0;
        return false;
    }

    if (__graphItem >= __graphData.size())
    {
        __graphItem = 0;
        return false;
    }

    X = __graphItem;
    Y = __graphData[__graphItem];

    __graphItem++;
    return true;
}
