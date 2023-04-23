/*
Jordan Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include <jorbnet/jorbNet.hpp>
#include <iostream>
using namespace std;

// a + b = 1
// C_e := | a + b - 1 |
double error(const SafeArray<double> &Obs, const SafeArray<double> &Exp)
{
    // C = C_e + C_o
    double out = __stderr(Obs, Exp);

    out += Obs[0] + Obs[1] - 1;

    if (out < 0)
    {
        out *= -1;
    }
    return out;
}

/*
C_e := | a + b - 1 |
\frac{\partial C_e}{\partial a} = 1
\frac{\partial C_e}{\partial b} = 1

Not very interesting
*/
SafeArray<double> errorDer(const SafeArray<double> &Obs, const SafeArray<double> &Exp)
{
    // \vec{\nabla} C = \vec{\nabla} C_e + \vec{\nabla} C_o
    SafeArray<double> out = __stderrder(Obs, Exp);

    out[0] += 1;
    out[1] += 1;

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

        cout << "(err " << n.err(out, s.output) << ")";

        cout << '\n';
    }

    // Go
    cout << "\nTraining...\n";
    auto start = chrono_now();
    n.train(100);
    auto end = chrono_now();
    cout << "Took " << chrono_ns(end - start) << " ns.\n";

    cout << '\n';
    for (auto s : n.trainingData)
    {
        cout << s.input[0] << ":\t";

        auto out = n.prop(s.input);

        for (auto o : out)
        {
            cout << o << '\t';
        }

        cout << "(err " << n.err(out, s.output) << ' ' << (int)(n.err(out, s.output) + 0.5) << ")";

        cout << '\n';
    }

    return 0;
}
