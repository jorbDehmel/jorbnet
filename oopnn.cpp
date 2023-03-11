#include <iostream>
#include "resources.hpp"
using namespace std;

int main()
{
    srand(12345);

    vector<int> sizes = {2, 5, 5, 2};
    network n(sizes);

    // 2 5 5 2
    // 3 inp     0-2
    // 5 hidden  3-7
    // 3 out     8-10

    // Load input activations from cin
    cout << "Please enter " << sizes[0] << " activations: ";
    vector<double> input;
    for (int i = 0; i < sizes[0]; i++)
    {
        double temp;
        cin >> temp;
        input.push_back(temp);
    }

    cout << "Prop\n";
    auto result = n.propogate(input);
    cout << "End Prop\n";

    vector<double> expected = {1, 0};

    double error = err(result, expected);
    cout << "Error: " << error << '\n';

    return 0;
}
