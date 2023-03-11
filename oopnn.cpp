#include <iostream>
#include "resources.hpp"
using namespace std;

int main()
{
    srand(12345);

    vector<int> sizes = {2, 5, 5, 2};

    // 2 5 5 2
    // 3 inp     0-2
    // 5 hidden  3-7
    // 3 out     8-10

    // Create network
    node **nodes;
    nodes = new node *[sizes.size()];
    for (int i = 0; i < sizes.size(); i++)
    {
        nodes[i] = new node[sizes[i]];
    }

    // Link nodes
    for (int i = 1; i < sizes.size(); i++)
    {
        for (int j = 0; j < sizes[i]; j++)
        {
            initNode(nodes[i][j], nodes[i - 1], sizes[i - 1]);
        }
    }

    // Load input activations from cin
    cout << "Please enter " << sizes[0] << " activations: ";
    for (int i = 0; i < sizes[0]; i++)
    {
        double temp;
        cin >> temp;
        nodes[0][i].act = temp;
    }

    // Propogate network
    for (int layer = 1; layer < sizes.size(); layer++)
    {
        for (int i = 0; i < sizes[layer]; i++)
        {
            prop(nodes[layer][i]);
        }
    }

    // Output final layer
    cout << "Outputs:\n";
    for (int i = 0; i < sizes.back(); i++)
    {
        cout << i << ": " << nodes[sizes.size() - 1][i].act << '\n';
    }

    // Delete network
    for (int i = 0; i < sizes.size(); i++)
    {
        delete[] nodes[i];
    }
    delete[] nodes;

    return 0;
}
