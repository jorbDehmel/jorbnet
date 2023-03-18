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
