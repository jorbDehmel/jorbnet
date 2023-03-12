#include "resources.hpp"

double drand(const double &Min, const double &Max)
{
    double out;
    out = Min + (Max - Min) * (double)rand() / RAND_MAX;
    return out;
}

void initNode(node &out, node *Links, const int &NumLinks)
{
    out.act = 0;
    out.bias = drand(-WEIGHT_VARIATION, WEIGHT_VARIATION);
    out.weights = new double[NumLinks];
    assert(out.weights != nullptr);
    for (int i = 0; i < NumLinks; i++)
    {
        out.links.push_back(Links + i);
        out.weights[i] = drand(-WEIGHT_VARIATION, WEIGHT_VARIATION);
    }
    out.wIndex = -1;
    return;
}

void freeNode(node *What)
{
    delete[] What->weights;
    return;
}

double wsum(const node &What)
{
    double out = 0;
    for (int i = 0; i < What.links.size(); i++)
    {
        out += What.links[i]->act * What.weights[i];
    }
    out += What.bias;
    return out;
}

double sig(const double &X)
{
    return 1 / (1 + pow(M_E, -X));
}

double prop(node &What)
{
    What.act = sig(wsum(What));
    return What.act;
}

// Sum of squares, not norm
double err(node *Layer, double *Expected, const int &Size)
{
    double out = 0;
    for (int i = 0; i < Size; i++)
    {
        out += pow(Expected[i] - Layer[i].act, 2);
    }
    return out;
}

double err(vector<double> &Obs, vector<double> &Exp)
{
    double out = 0;
    for (int i = 0; i < Obs.size(); i++)
    {
        out += pow(Exp[i] - Obs[i], 2);
    }
    return out;
}

double mag(vector<double> &What)
{
    double out = 0;
    for (auto i : What)
    {
        out += pow(i, 2);
    }
    return sqrt(out);
}

// Get the derivative of a single node with respect to some weight 'For'
double bprop(node &What, double *For, map<node *, double> &Found)
{
    // Safeguard against recalculating
    if (Found.count(&What) != 0)
    {
        // Word salad if I've ever heard it
        throw runtime_error("Auxilery derivative map safeguard failed");
    }

    // Safeguard against reaching the input without dependancy on the weight
    if (What.links.size() == 0)
    {
        Found[&What] = 0;
        return 0;
    }

    // This will be multiplied by everything for the chain rule
    // Equal to sigmoid prime via previous activations
    double myDer = What.act * (1 - What.act);

    // If the weight is connected to this node, the derivative is equal
    // to the previous activation of the node linked to it
    for (int i = 0; i < What.links.size(); i++)
    {
        if (&What.weights[i] == For)
        {
            Found[&What] = myDer * What.links[i]->act;
            return Found[&What];
        }
    }

    // Otherwise, the weight is not directly connected to this node.
    // We will have to do this the hard way.
    // This looks like it could be combined with the above for loop,
    // but it's mad recursive and the other one saves time, so you
    // should NOT do that
    double out = 0;
    for (int i = 0; i < What.links.size(); i++)
    {
        // If this node has already been done, use that result
        if (Found.count(What.links[i]) != 0)
        {
            out += What.weights[i] * Found[What.links[i]];
        }

        // Otherwise, compute it for the first time
        else
        {
            auto temp = What.weights[i] * bprop(*What.links[i], For, Found);
            out += temp;
        }
    }
    out *= myDer;
    Found[&What] = out;

    // cout << "Node " << &What << " has derivative " << out << " wrt " << &For << '\n';

    return out;
}

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
        }
    }

    return;
}

network::~network()
{
    // Delete network
    for (int i = 0; i < sizes.size(); i++)
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

    // Find observed
    vector<double> observed = propogate(data->inputs);

    // Construct gradient error vector w/ respect to each weight
    vector<double> gradient;
    for (double *weight : weights)
    {
        // Get the derivative of the error function
        // with respect to the current weight

        // Sum
        double der = 0;
        map<node *, double> found;
        for (int i = 0; i < observed.size(); i++)
        {
            // Sum of err times derivative of each output node
            der -= 2 * (data->expected[i] - observed[i]) * bprop(nodes[sizes.size() - 1][i], weight, found);
        }

        // cout << "Entry " << gradient.size() << '\t' << der << '\n';

        gradient.push_back(der);
    }

    // Set up for normalization
    double magnitude = mag(gradient);

    // Move away from direction of gradient vector (decr weights)
    for (int i = 0; i < gradient.size(); i++)
    {
        if (magnitude != 0)
        {
            *weights[i] -= (gradient[i] / magnitude) * drand(MIN_STEP_SIZE, MAX_STEP_SIZE);
        }
    }

    passes++;
    return;
}

void network::train(const int NumTimes)
{
    long long totalNS = 0;
    for (int i = 0; i < NumTimes; i++)
    {
        auto start = chrono::high_resolution_clock::now();
        backprop(-1);
        auto end = chrono::high_resolution_clock::now();
        totalNS += chrono::duration_cast<chrono::milliseconds>(end - start).count();

        if (passes % LOG_INTERVAL == 0)
        {
            double error = 0;
            for (auto set : trainingData)
            {
                auto observed = propogate(set.inputs);
                error += err(observed, set.expected);
            }

            errors.push_back(error);
            cout << "(" << passes << ", " << error << ")\n";

            if (error < ERROR_BREAK)
            {
                cout << "Error break activated after " << passes << ".\n";
                break;
            }
        }
    }

    cout << "Total ms: " << totalNS << '\n'
         << "Average ms per pass: " << totalNS / (double)passes << '\n';
    return;
}
