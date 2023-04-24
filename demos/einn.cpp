/*
Jordan Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include <jorbnet/jorbNet.hpp>
#include <iostream>
#include <cmath>
using namespace std;

// C_e := ab + ba^2 + asin(b)
double error(const SafeArray<double> &Obs, const SafeArray<double> &Exp)
{
    // C = C_e + C_o
    double out = __stderr(Obs, Exp);

    // DO STUFF HERE

    double toAdd = Obs[0] * Obs[1] + Obs[1] * pow(Obs[0], 2) + Obs[0] * sin(Obs[1]);

    // END DOING STUFF

    if (toAdd < 0)
    {
        toAdd *= -1;
    }
    return out + toAdd;
}

/*
C_e := ab + ba^2 + asin(b)

C_{ea} = b + 2ab + sin(b)
C_{eb} = a + a^2  + acos(b)
*/
SafeArray<double> errorDer(const SafeArray<double> &Obs, const SafeArray<double> &Exp)
{
    // \vec{\nabla} C = \vec{\nabla} C_e + \vec{\nabla} C_o
    SafeArray<double> out = __stderrder(Obs, Exp);

    // DO STUFF HERE
    // NO CONSTANTS ALLOWED!

    out[0] += Obs[1] + 2 * Obs[0] * Obs[1] + sin(Obs[1]);     // b + 2ab + sin(b)
    out[1] += Obs[0] + pow(Obs[0], 2) + Obs[0] * cos(Obs[1]); // a + a^2 + acos(b)

    // END DOING STUFF

    return out;
}

int main(const int argc, const char *argv[])
{
    srand(time(NULL));

    Network n({2, 5, 2});

    // Set up meta-equations
    n.err = error;
    n.errder = errorDer;

    dataset a;
    a.input = {0, 0};
    a.output = {1, 0};

    dataset b;
    b.input = {0, 1};
    b.output = {0, 1};

    dataset c;
    c.input = {1, 0};
    c.output = {0, 1};

    dataset d;
    d.input = {1, 1};
    d.output = {1, 0};

    n.trainingData = {a, b, c, d};

    cout << '\n';
    for (auto s : n.trainingData)
    {
        cout << s.input[0] << ":\t";
        auto out = n.prop(s.input);
        for (auto o : out)
        {
            cout << o << '\t';
        }
        cout << "(err " << n.err(out, s.output) << ")\n";
    }

    // Go
    cout << "\nTraining...\n";
    auto start = chrono_now();
    n.train(50000);
    auto end = chrono_now();
    cout << "Took " << toTime(chrono_ns(end - start)) << ".\n";

    cout << '\n';
    for (auto s : n.trainingData)
    {
        cout << s.input[0] << ":\t";
        auto out = n.prop(s.input);
        for (auto o : out)
        {
            cout << o << '\t';
        }
        cout << "(err " << n.err(out, s.output) << ' ' << (int)(n.err(out, s.output) + 0.5) << ")\n";
    }

    return 0;
}
