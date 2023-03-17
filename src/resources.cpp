/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include "resources.hpp"

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
    return Min + (Max - Min) * (double)rand() / RAND_MAX;
}

void initNode(node &Out, node *Links, const int &NumLinks)
{
    Out.act = 0;
    Out.bias = drand(-WEIGHT_VARIATION, WEIGHT_VARIATION);
    Out.weights = nullptr;
    Out.weights = new double[NumLinks];
    assert(Out.weights != nullptr);
    for (int i = 0; i < NumLinks; i++)
    {
        Out.links.push_back(Links + i);
        Out.weights[i] = drand(-WEIGHT_VARIATION, WEIGHT_VARIATION);
    }
    Out.wIndex = -1;
    return;
}

void cloneNode(node &To, const node &From, node *Links, const int &NumLinks)
{
    To.act = From.act;
    To.bias = From.bias;

    To.weights = nullptr;
    To.weights = new double[NumLinks];
    assert(To.weights != nullptr);

    for (int i = 0; i < From.links.size(); i++)
    {
        To.links.push_back(Links + i);
        double temp = From.weights[i];
        To.weights[i] = temp;
    }
    To.wIndex = From.wIndex;
    return;
}

void freeNode(node *What)
{
    if (What->weights != nullptr)
    {
        delete[] What->weights;
    }
    return;
}

double wsum(const node &What)
{
    double out = What.bias;
    for (int i = 0; i < What.links.size(); i++)
    {
        out += What.links[i]->act * What.weights[i];
    }
    return out;
}

#define CSIG_DIV 5.62
#define CSIG_MIN -2.81
#define CSIG_MAX 2.81
double sig(const double &X)
{
    if (X < CSIG_MIN)
    {
        return 0;
    }
    else if (X > CSIG_MAX)
    {
        return 1;
    }
    else
    {
        return 0.5 + X / CSIG_DIV;
    }

    // For reference, the real sig function:
    // return 1 / (1 + pow(M_E, -X));
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
// long int bpropCount = 0;
double bprop(node &What, double *For, map<node *, double> &FoundWeights, map<node *, double> &FoundGenerals)
{
    // bpropCount++;

    // Safeguard against reaching the input without dependancy on the weight
    if (What.links.size() == 0)
    {
        FoundWeights[&What] = 0;
        return 0;
    }

    // This will be multiplied by everything for the chain rule
    // Equal to sigmoid prime via previous activations
    if (FoundGenerals.count(&What) == 0)
    {
        FoundGenerals[&What] = What.act * (1 - What.act);
    }
    double myDer = FoundGenerals[&What];

    if (&What.bias == For)
    {
        FoundWeights[&What] = myDer;
        return FoundWeights[&What];
    }

    // If the weight is connected to this node, the derivative is equal
    // to the previous activation of the node linked to it
    for (int i = 0; i < What.links.size(); i++)
    {
        if (&What.weights[i] == For)
        {
            // Uses traditional sigmoid derivative (even though it really uses quicksig)
            FoundWeights[&What] = myDer * What.links[i]->act;
            return FoundWeights[&What];
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
        if (FoundWeights.count(What.links[i]) != 0)
        {
            out += What.weights[i] * FoundWeights[What.links[i]];
        }

        // Otherwise, compute it for the first time
        else
        {
            // EXPENSIVE CALL
            auto temp = What.weights[i] * bprop(*What.links[i], For, FoundWeights, FoundGenerals);
            out += temp;

            assert(FoundWeights.count(What.links[i]) != 0);
        }
    }

    // Polish up derivative and add to list of found
    out *= myDer;
    FoundWeights[&What] = out;

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
